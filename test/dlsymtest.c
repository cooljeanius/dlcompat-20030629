/* dlsymtest.c */

#include "../dlfcn.h"

/* taken from "../dlfcn.c": */
#ifndef RTLD_SELF
# define RTLD_SELF ((void *) -3)
#endif /* !RTLD_SELF */

int main(int argc, const char* argv[])
{
	dlsym(dlopen("dllib.dylib", RTLD_LAZY | RTLD_GLOBAL), "_init");
	dlsym(RTLD_DEFAULT, "_init");
	dlsym(RTLD_NEXT, "_init");
	dlsym(RTLD_SELF, "_init");
}

/* EOF */
