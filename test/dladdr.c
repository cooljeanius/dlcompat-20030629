/* dladdr.c */

#define __BSD_VISIBLE 1
#include "dlfcn.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* for strcmp() */
#include <mach-o/dyld.h>

int main(int argc, const char* argv[])
{
	int retCode;
	const char * syms[] = {"_printf","_dlopen","_main",0};
	int i;
	struct dl_info info;
	NSSymbol syml;
	void* addr;

	retCode = 0;
	i = 0;

	printf("This program has the path %s and is running with %i arguments", argv[0], argc); /* use argc and argv */

	while (syms[i]) {
		syml = NSLookupAndBindSymbol(syms[i]);
		if (syml) {
			addr = NSAddressOfSymbol(syml);
			dladdr(addr,&info);
			/* the casts in the arguments to fprintf() here silence clang warnings,
			 * but create new GCC ones... */
			fprintf(stdout,"Symbol: %s\nN SSym: %x\n Address: %x\n FName: %s\n Base: %x\n Symbol: %s\n Address: %x\n\n\n",
					syms[i], /* value for formatter 1 ("Symbol:") */
					(unsigned int)syml, /* value for formatter 2 ("SSym:") */
					(unsigned int)addr, /* value for formatter 3 ("Address:") */
					info.dli_fname, /* value for formatter 4 ("FName:") */
					(unsigned int)info.dli_fbase, /* value for formatter 5 ("Base:") */
					info.dli_sname, /* value for formatter 6 ("Symbol:") */
					(unsigned int)info.dli_saddr); /* value for formatter 7 ("Address:") */
			if (addr != info.dli_saddr) {
				retCode++;
			}
			if (strcmp(syms[i],info.dli_sname)) {
				retCode++;
			}
		}
		i++;
	}
	return retCode;
}

/* EOF */
