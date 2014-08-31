/* dlsymtest.c */

#ifdef HAVE_CONFIG_H
# include "../config.h"
#else
# ifndef DLSYMTEST_C_NON_AUTOHEADER_BUILD
#  define DLSYMTEST_C_NON_AUTOHEADER_BUILD 1
# endif /* !DLSYMTEST_C_NON_AUTOHEADER_BUILD */
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

#include "../dlfcn.h"

/* taken from "../dlfcn.c": */
#ifndef RTLD_SELF
# define RTLD_SELF ((void *)-3)
#endif /* !RTLD_SELF */

int main(int argc, const char* argv[])
{
	/* use argc and argv: */
	printf("This program was invoked with path %s and is running with %i argument(s).\n\n", argv[0], argc);
	dlsym(dlopen("dllib.dylib", RTLD_LAZY | RTLD_GLOBAL), "_init");
	dlsym(RTLD_DEFAULT, "_init");
	dlsym(RTLD_NEXT, "_init");
	dlsym(RTLD_SELF, "_init");
	/*TODO: make return code tied to result of dlsym tests instead of hardcoding it:*/
	return 0;
}

/* EOF */
