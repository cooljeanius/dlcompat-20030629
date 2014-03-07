/* lookupDyldFunction.c */
/* largely taken from dyldAPIs.cpp in dyld-97.1 */

/* some of these includes might be unnecessary, but whatever: */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <unistd.h>

#include <mach/mach.h>
#include <mach-o/dyld.h>
#include <sys/time.h>
#include <sys/sysctl.h>

#include "dlfcn.h"

#ifndef lookupDyldFunction
/* prototype */
extern bool lookupDyldFunction(const char* name, uintptr_t* address);
#endif /* !lookupDyldFunction */

#ifndef dyld_func
typedef struct {
    const char* name;
	/* we are just looking up the names of functions for this file, so we should
	 * be able to get away with just using strings instead of the actual
	 * implementations: */
    const char*	implementation; /*(was previously "void*" instead of "const char*")*/
} dyld_func;
#endif /* !dyld_func */

#ifndef dyld_funcs
static dyld_func dyld_funcs[] = {
 	{"__dyld_register_thread_helpers",					"registerThreadHelpers" },
    {"__dyld_register_func_for_add_image",				"_dyld_register_func_for_add_image" },
    {"__dyld_register_func_for_remove_image",			"_dyld_register_func_for_remove_image" },
    {"__dyld_make_delayed_module_initializer_calls",	"_dyld_make_delayed_module_initializer_calls" },
    {"__dyld_fork_child",								"_dyld_fork_child" },
	{"__dyld_dyld_register_image_state_change_handler",	"dyld_register_image_state_change_handler" },
    {"__dyld_dladdr",									"dladdr" },
    {"__dyld_dlclose",									"dlclose" },
    {"__dyld_dlerror",									"dlerror" },
    {"__dyld_dlopen",									"dlopen" },
    {"__dyld_dlsym",									"dlsym" },
    {"__dyld_dlopen_preflight",							"dlopen_preflight" },
    {"__dyld_get_image_header_containing_address",		"_dyld_get_image_header_containing_address" },
	{"__dyld_image_count",								"_dyld_image_count" },
    {"__dyld_get_image_header",							"_dyld_get_image_header" },
    {"__dyld_get_image_vmaddr_slide",					"_dyld_get_image_vmaddr_slide" },
    {"__dyld_get_image_name",							"_dyld_get_image_name" },
    {"__dyld__NSGetExecutablePath",						"_NSGetExecutablePath" },

	/* the rest of these are either deprecated or SPIs */
    {"__dyld_lookup_and_bind",						"client_dyld_lookup_and_bind" },
    {"__dyld_lookup_and_bind_with_hint",			"_dyld_lookup_and_bind_with_hint" },
    {"__dyld_lookup_and_bind_fully",				"_dyld_lookup_and_bind_fully" },
    {"__dyld_install_handlers",						"_dyld_install_handlers" },
    {"__dyld_link_edit_error",						"NSLinkEditError" },
    {"__dyld_unlink_module",						"NSUnLinkModule" },
    {"__dyld_bind_objc_module",						"_dyld_bind_objc_module" },
    {"__dyld_bind_fully_image_containing_address",  "_dyld_bind_fully_image_containing_address" },
    {"__dyld_image_containing_address",				"_dyld_image_containing_address" },
    {"__dyld_moninit",								"_dyld_moninit" },
    {"__dyld_register_binding_handler",				"_dyld_register_binding_handler" },
    {"__dyld_NSNameOfSymbol",						"NSNameOfSymbol" },
    {"__dyld_NSAddressOfSymbol",					"NSAddressOfSymbol" },
    {"__dyld_NSModuleForSymbol",					"NSModuleForSymbol" },
    {"__dyld_NSLookupAndBindSymbol",				"NSLookupAndBindSymbol" },
    {"__dyld_NSLookupAndBindSymbolWithHint",		"NSLookupAndBindSymbolWithHint" },
    {"__dyld_NSLookupSymbolInModule",				"NSLookupSymbolInModule" },
    {"__dyld_NSLookupSymbolInImage",				"NSLookupSymbolInImage" },
    {"__dyld_NSMakePrivateModulePublic",			"NSMakePrivateModulePublic" },
    {"__dyld_NSIsSymbolNameDefined",				"client_NSIsSymbolNameDefined" },
    {"__dyld_NSIsSymbolNameDefinedWithHint",		"NSIsSymbolNameDefinedWithHint" },
    {"__dyld_NSIsSymbolNameDefinedInImage",			"NSIsSymbolNameDefinedInImage" },
    {"__dyld_NSNameOfModule",						"NSNameOfModule" },
    {"__dyld_NSLibraryNameForModule",				"NSLibraryNameForModule" },
    {"__dyld_NSAddLibrary",							"NSAddLibrary" },
    {"__dyld_NSAddLibraryWithSearching",			"NSAddLibraryWithSearching" },
    {"__dyld_NSAddImage",							"NSAddImage" },
    {"__dyld_launched_prebound",					"_dyld_launched_prebound" },
    {"__dyld_all_twolevel_modules_prebound",		"_dyld_all_twolevel_modules_prebound" },
    {"__dyld_call_module_initializers_for_dylib",   "_dyld_call_module_initializers_for_dylib" },
    {"__dyld_install_link_edit_symbol_handlers",	"dyld::registerZeroLinkHandlers" },
    {"__dyld_NSCreateObjectFileImageFromFile",			"NSCreateObjectFileImageFromFile" },
    {"__dyld_NSCreateObjectFileImageFromMemory",		"NSCreateObjectFileImageFromMemory" },
    {"__dyld_NSDestroyObjectFileImage",					"NSDestroyObjectFileImage" },
    {"__dyld_NSLinkModule",								"NSLinkModule" },
    {"__dyld_NSHasModInitObjectFileImage",				"NSHasModInitObjectFileImage" },
    {"__dyld_NSSymbolDefinitionCountInObjectFileImage",	"NSSymbolDefinitionCountInObjectFileImage" },
    {"__dyld_NSSymbolDefinitionNameInObjectFileImage",	"NSSymbolDefinitionNameInObjectFileImage" },
    {"__dyld_NSIsSymbolDefinedInObjectFileImage",		"NSIsSymbolDefinedInObjectFileImage" },
    {"__dyld_NSSymbolReferenceNameInObjectFileImage",	"NSSymbolReferenceNameInObjectFileImage" },
    {"__dyld_NSSymbolReferenceCountInObjectFileImage",	"NSSymbolReferenceCountInObjectFileImage" },
    {"__dyld_NSGetSectionDataInObjectFileImage",		"NSGetSectionDataInObjectFileImage" },
    {"__dyld_NSFindSectionAndOffsetInObjectFileImage",  "NSFindSectionAndOffsetInObjectFileImage" },
    {"__dyld_link_module",							"_dyld_link_module" },
	{"__dyld_dlord",									"dlord" },
	{"__dyld_get_all_image_infos",						"_dyld_get_all_image_infos" },
    {NULL, 0}
};
#endif /* !dyld_funcs */

#ifndef unimplemented
static void unimplemented(void);
static void unimplemented(void)
{
	printf("unimplemented dyld function\n");
	exit(1);
}
#endif /* !unimplemented */

bool lookupDyldFunction(const char* name, uintptr_t* address)
{
	const dyld_func* p;

	for (p = dyld_funcs; p->name != NULL; ++p) {
	    if ( strcmp(p->name, name) == 0 ) {
			/* not sure if I can use unimplemented() like this... should I put
			 * quotes around it to make it a string? Or cast it? Or both?
			 * (the current method silences warnings from clang, but NOT from GCC) */
			if( p->implementation == (const char *)((const char *)unimplemented || "unimplemented")) {
				printf("unimplemented dyld function: %s\n", p->name);
			}
			*address = (uintptr_t)p->implementation;
			return true;
	    }
	}
	*address = 0;
	return false;
}

/* EOF */
