/* dlfcn_simple.c
 * Copyright (c) 2002 Peter O'Gorman <ogorman@users.sourceforge.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


/* Just to prove that it is NOT that hard to add Mac calls to your code :)
 * This works with pretty much everything, including kde3 xemacs and the gimp.
 * I would guess that it would work in at least 95% of cases, so use this as
 * your starting point, rather than the mess that is dlfcn.c, assuming that your
 * code does not require ref counting or symbol lookups in dependent libraries.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define DLFCN_SIMPLE_C_NON_AUTOHEADER_BUILD 1
#endif /* HAVE_CONFIG_H */

#if defined(__APPLE__) || defined(HAVE_AVAILABILITY_H)
# include <Availability.h>
#else
# warning "will not be able to do availability checks"
#endif /* __APPLE__ || HAVE_AVAILABILITY_H */

#if defined(__APPLE__) || defined(HAVE_AVAILABILITYMACROS_H)
# include <AvailabilityMacros.h>
#else
# warning "will not be able to do checks with availability macros"
#endif /* __APPLE__ || HAVE_AVAILABILITYMACROS_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <limits.h>
#include <mach-o/dyld.h>
#include "dlfcn.h"

#define ERR_STR_LEN 256
static void *dlsymIntern(void *handle, const char *symbol);
static const char *error(int setget, const char *str, ...);



/* Set and get the error string for use by dlerror */
static const char *error(int setget, const char *str, ...)
{
	static char errstr[ERR_STR_LEN];
	static int err_filled;
	const char *retval;
	NSLinkEditErrors ler;
	int lerno;
	const char *dylderrstr;
	const char *file;
	va_list arg;

	err_filled = 0;

	if (setget <= 0) {
		va_start(arg, str);
		strncpy(errstr, "dlsimple: ", ERR_STR_LEN);
		vsnprintf(errstr + 10, ERR_STR_LEN - 10, str, arg);
		va_end(arg);
	/* We prefer to use the dyld error string if getset is 1*/
		if (setget == 0) {
			NSLinkEditError(&ler, &lerno, &file, &dylderrstr);
			fprintf(stderr,"dyld: %s\n",dylderrstr);
			if (dylderrstr && strlen(dylderrstr)) {
				strncpy(errstr,dylderrstr,ERR_STR_LEN);
			}
		}
		err_filled = 1;
		retval = NULL;
	} else {
		if (!err_filled) {
			retval = NULL;
		} else {
			retval = errstr;
		}
		err_filled = 0;
	}
	return retval;
}

/* dlopen */
void *dlopen(const char *path, int mode)
{
	void *module;
	NSObjectFileImage ofi;
	NSObjectFileImageReturnCode ofirc;
	static int (*make_private_module_public) (NSModule module) = 0;
	unsigned int flags;

	module = 0;
	ofi = 0;
	flags = NSLINKMODULE_OPTION_RETURN_ON_ERROR | NSLINKMODULE_OPTION_PRIVATE;

	/* If we got no path, the app wants the global namespace, use -1 as the marker
	 * in this case */
	if (!path) {
		return (void *)-1;
	}

	/* Create the object file image, works for things linked with the -bundle arg to ld */
	ofirc = NSCreateObjectFileImageFromFile(path, &ofi);
	switch (ofirc) {
		case NSObjectFileImageSuccess:
			/* It was okay, so use NSLinkModule to link in the image */
			if (!(mode & RTLD_LAZY)) flags += NSLINKMODULE_OPTION_BINDNOW;
			module = NSLinkModule(ofi, path,flags);
			/* Do NOT forget to destroy the object file image, unless you like
			 * leaks */
			NSDestroyObjectFileImage(ofi);
			/* If the mode was global, then change the module; this avoids
			 * multiply defined symbol errors to first load private then make
			 * global. Silly, is it not? */
			if ((mode & RTLD_GLOBAL)) {
			  if (!make_private_module_public) {
				  /* see the note in dlfcn.c about casting the paramaters of
				   * _dyld_func_lookup(): */
				   _dyld_func_lookup("__dyld_NSMakePrivateModulePublic",
				   (void**)(unsigned long *)&make_private_module_public);
			  }
			  make_private_module_public(module);
			}
			break;
		case NSObjectFileImageInappropriateFile:
			/* It may have been a dynamic library rather than a bundle, try to load it */
			module = (void *)NSAddImage(path, NSADDIMAGE_OPTION_RETURN_ON_ERROR);
			break;
		case NSObjectFileImageFailure:
			error(0,"Object file setup failure :  \"%s\"", path);
			return 0;
		case NSObjectFileImageArch:
			error(0,"No object for this architecture :  \"%s\"", path);
			return 0;
		case NSObjectFileImageFormat:
			error(0,"Bad object file format :  \"%s\"", path);
			return 0;
		case NSObjectFileImageAccess:
			error(0,"Cannot read object file :  \"%s\"", path);
			return 0;
	}
	if (!module) {
		error(0, "Cannot open \"%s\"", path);
	}
	return module;
}

/* dlsymIntern is used by dlsym to find the symbol */
void *dlsymIntern(void *handle, const char *symbol)
{
	NSSymbol *nssym;

	nssym = 0;

	/* If the handle is -1, if is the app global context */
	if (handle == (void *)-1) {
#ifdef __MAC_OS_X_VERSION_MIN_REQUIRED || 1
		/* The "|| 1" is there because even though this case is deprecated,
		 * it was the original and the only one actually tested properly: */
# if (__MAC_OS_X_VERSION_MIN_REQUIRED < 1040) || 1
		/* Global context, use NSLookupAndBindSymbol (deprecated in 10.4+) */
		/* (NSIsSymbolNameDefined is also deprecated in 10.4+) */
		if (NSIsSymbolNameDefined(symbol)) {
			nssym = (NSSymbol *)NSLookupAndBindSymbol(symbol);
		}
# elif (__MAC_OS_X_VERSION_MIN_REQUIRED < 1050)
		/* not sure if this works as a 1-for-1 substitution of the above: */
		if ((bool)NSLookupSymbolInImage(NULL,symbol,(uint32_t)NULL)) {
			nssym = (NSSymbol *)NSLookupSymbolInImage(NULL,symbol,(uint32_t)NULL);
		}
# else
#  if __GNUC__ && !__STRICT_ANSI__
#   warning "no case for your OS implemented yet"
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* 10.3- || 10.4 */
#else
# if __GNUC__ && !__STRICT_ANSI__
#  warning "cannot make assumptions based on your OS"
# endif /* __GNUC__ && !__STRICT_ANSI__ */
#endif /* __MAC_OS_X_VERSION_MIN_REQUIRED */
	}
	/* Now see if the handle is a struch mach_header* or not, use NSLookupSymbol
	 * in image for libraries, and NSLookupSymbolInModule for bundles */
	else {
		/* Check for both possible magic numbers depending on x86/ppc byte order */
		if ((((struct mach_header *)handle)->magic == MH_MAGIC) ||
			(((struct mach_header *)handle)->magic == MH_CIGAM)) {
			if (NSIsSymbolNameDefinedInImage((struct mach_header *)handle, symbol)) {
				nssym = (NSSymbol *)NSLookupSymbolInImage((struct mach_header *)handle, symbol,
														  NSLOOKUPSYMBOLINIMAGE_OPTION_BIND
														  | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR);
			}

		} else {
			nssym = (NSSymbol *)NSLookupSymbolInModule(handle, symbol);
		}
	}
	if (!nssym) {
		error(0, "Symbol \"%s\" Not found", symbol);
		return NULL;
	}
	return NSAddressOfSymbol((NSSymbol)nssym);
}

const char *dlerror(void)
{
	return error(1, (char *)NULL);
}

int dlclose(void *handle)
{
	if ((((struct mach_header *)handle)->magic == MH_MAGIC) ||
		(((struct mach_header *)handle)->magic == MH_CIGAM)) {
		error(-1, "Cannot remove dynamic libraries on darwin");
		return 0;
	}
	if (!NSUnLinkModule(handle, 0)) {
		error(0, "unable to unlink module %s", NSNameOfModule(handle));
		return 1;
	}
	return 0;
}


/* dlsym, prepend the underscore and call dlsymIntern */
void *dlsym(void *handle, const char *symbol)
{
	static char undersym[257];	/* Saves calls to malloc(3) */
	int sym_len;
	void *value;
	char *malloc_sym;

	sym_len = (int)strlen(symbol);
	value = NULL;
	malloc_sym = NULL;

	if (sym_len < 256) {
		snprintf(undersym, 256, "_%s", symbol);
		value = dlsymIntern(handle, undersym);
	} else {
		malloc_sym = malloc(sym_len + 2);
		if (malloc_sym) {
			sprintf(malloc_sym, "_%s", symbol);
			value = dlsymIntern(handle, malloc_sym);
			free(malloc_sym);
		} else {
			error(-1, "Unable to allocate memory");
		}
	}
	return value;
}

/* EOF */
