/* dlsymtest.c */

#include <stdio.h>

#include "../dlfcn.h"

/* taken from "../dlfcn.c": */
#ifndef RTLD_SELF
# define RTLD_SELF ((void *) -3)
#endif /* !RTLD_SELF */

int main(int argc, const char* argv[])
{
	printf("This program has the path %s and is running with %i arguments", argv[0], argc); /* use argc and argv */
	dlsym(dlopen("dllib.dylib", RTLD_LAZY | RTLD_GLOBAL), "_init");
	dlsym(RTLD_DEFAULT, "_init");
	dlsym(RTLD_NEXT, "_init");
	dlsym(RTLD_SELF, "_init");
	return 0;
}

/* EOF */
