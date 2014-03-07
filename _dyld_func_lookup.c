/* _dyld_func_lookup.c */
/* asm taken from dyldStartup.s from dyld-97.1, which was the last version of
 * dyld in which _dyld_func_lookup was NOT deprecated. */

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# define NON_AUTOHEADER_BUILD 1
#endif /* HAVE_CONFIG_H */

#if __ppc__ || __ppc64__
# if defined(HAVE_ARCHITECTURE_PPC_MODE_INDEPENDENT_ASM_H) || __APPLE__
#  include <architecture/ppc/mode_independent_asm.h>
# endif /* HAVE_ARCHITECTURE_PPC_MODE_INDEPENDENT_ASM_H || __APPLE__ */
#endif /* __ppc__ || __ppc64__ */

#if defined(HAVE_MACH_O_DYLD_H) || __APPLE__
# include <mach-o/dyld.h>
#elif !defined(_dyld_func_lookup)
/* prototype */
extern int _dyld_func_lookup(const char* dyld_func_name, void **address);
#endif /* HAVE_MACH_O_DYLD_H || __APPLE__ */

#ifdef __GNUC__
# define ASMCALL __asm__
#elif defined _MSC_VER
# define ASMCALL __asm
#endif /* __GNUC__ || _MSC_VER */

#ifndef INLINECALL
# ifdef __STRICT_ANSI__
#  ifdef __inline__
#   define INLINECALL __inline__
#  else
#   if defined(__NO_INLINE__) && defined(__GNUC__) && !defined(__STDC__)
#    warning "INLINECALL will be unavailable when using the '-ansi' compiler flag"
#   endif /* __NO_INLINE__ && __GNUC__ && !__STDC__ */
#   define INLINECALL /* nothing */
#  endif /* __inline__ */
# else
#  define INLINECALL inline
# endif /* __STRICT_ANSI__ */
#endif /* !INLINECALL */

INLINECALL int _dyld_func_lookup(const char* dyld_func_name, void **address)
{
	/* TODO: figure out how to get this function's parameters into the
	 * assembly part */
#if __GNUC__
/* not sure if __volatile__ is needed or not? */
# ifdef __i386__
	__asm__ __volatile__ ("\n\t"
	".data\n\t"
	"\n\t"
	".globl	_dyld_func_lookup\n\t"
"_dyld_func_lookup:\n\t"
	"jmp	_lookupDyldFunction\n\t" /* demangled C++ name: removed "_Z18" and "PKcPm" */
	"\n\t"
	".text\n\t"
	".align	4, 0x90\n\t");
# endif /* __i386__ */

# if __x86_64__
	__asm__ __volatile__ ("\n\t"
	".data\n\t"
	".align 3\n\t"
	"\n\t"
	".globl	_dyld_func_lookup\n\t"
"_dyld_func_lookup:\n\t"
	"jmp	_lookupDyldFunction\n\t" /* demangled C++ name: removed "_Z18" and "PKcPm" */
	"\n\t"
	".text\n\t"
	".align 2,0x90\n\t"
			 "\n\t");
# endif /* __x86_64__ */

# if __ppc__ || __ppc64__
	__asm__ __volatile__ ("\n\t"
	"\n\t"
	".data\n\t"
	".align 2\n\t"
	"\n\t"
	".globl	_dyld_func_lookup\n\t"
"_dyld_func_lookup:\n\t"
	"b	_lookupDyldFunction\n\t" /* demangled C++ name: removed "_Z18" and "PKcPm" */
	"\n\t"
	"\n\t"
	"\n\t"
	".text\n\t"
	".align 2\n\t"
			 "\n\t");
# endif /* __ppc__ || __ppc64__ */
#else
# warning "only GCC-style inline assembly is recognized so far."
#endif /* __GNUC__ */
	return 0;
}

/* EOF */
