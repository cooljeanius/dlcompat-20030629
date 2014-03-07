/* dladdr.c */

#define __BSD_VISIBLE 1

#ifdef HAVE_CONFIG_H
# include "../config.h"
#else
# define DLADDR_C_NON_AUTOHEADER_BUILD 1
#endif /* HAVE_CONFIG_H */

#include "dlfcn.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* for strcmp() */
#include <mach-o/dyld.h>

int main(int argc, const char* argv[])
{
	int retCode;
	/* the underscores here are making this test fail: */
	const char * syms[] = {"_printf","_dlopen","_main",0};
	/* (I assume the "0" at the end is to break out of the "while" loop below...) */
	int i;
	struct dl_info info;
	NSSymbol syml;
	void* addr;

	retCode = 0;
	i = 0;

	/* use argc and argv: */
	printf("This program was invoked with path %s and is running with %i argument(s).\n\n", argv[0], argc);

	while (syms[i]) {
		fprintf(stdout,"dladdr test #%i:\n",(i+1));
		syml = NSLookupAndBindSymbol(syms[i]);
		if (syml) {
			addr = NSAddressOfSymbol(syml);
			dladdr(addr,&info);
			/* the casts in the arguments to fprintf() here silence clang warnings,
			 * but create different GCC ones... */
			fprintf(stdout,
					"Symbol: %s\nN SSym: %x\n Address: %x\n FName: %s\n Base: %x\n Symbol: %s\n Address: %x\n\n\n",
					syms[i], /* value for formatter 1 ("Symbol:") */
					(unsigned int)syml, /* value for formatter 2 ("SSym:") */
					(unsigned int)addr, /* value for formatter 3 ("Address:") */
					info.dli_fname, /* value for formatter 4 ("FName:") */
					(unsigned int)info.dli_fbase, /* value for formatter 5 ("Base:") */
					info.dli_sname, /* value for formatter 6 ("Symbol:") */
					(unsigned int)info.dli_saddr); /* value for formatter 7 ("Address:") */
			if (addr != info.dli_saddr) {
				fprintf(stdout,
						"Address1 (%x) did not match Address2 (%x), incrementing return code...\n\n",
						(unsigned int)addr,
						(unsigned int)info.dli_saddr);
				retCode++;
			}
			if (strcmp(syms[i],info.dli_sname)) {
				fprintf(stdout,
						"Symbol1 (%s) did not match Symbol2 (%s), incrementing return code...\n\n",
						syms[i],
						info.dli_sname);
				retCode++;
			}
		}
		i++;
	}
	if (retCode != 0) {
		fprintf(stderr,"Returning %i...\n", retCode);
	} else {
		/* 0 is good, so use stdout instead of stderr: */
		fprintf(stdout,"Returning %i...\n", retCode);
	}
	return retCode;
}

/* EOF */
