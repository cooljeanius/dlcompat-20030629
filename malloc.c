/* malloc.c malloc() function that is glibc compatible.
 *
 * Copyright (C) 1997-1998, 2006-2007, 2009-2012 Free Software Foundation, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/* written by Jim Meyering and Bruno Haible */

#ifndef _GL_USE_STDLIB_ALLOC
# define _GL_USE_STDLIB_ALLOC 1
#endif /* !_GL_USE_STDLIB_ALLOC */
#include <config.h>
/* Only the AC_FUNC_MALLOC macro defines 'malloc' already in config.h: */
#ifdef malloc
# define NEED_MALLOC_GNU 1
# undef malloc
#else
/* Whereas the gnulib module 'malloc-gnu' defines HAVE_MALLOC_GNU: */
# if (defined(GNULIB_MALLOC_GNU) && GNULIB_MALLOC_GNU) && !HAVE_MALLOC_GNU
#  define NEED_MALLOC_GNU 1
# endif /* (GNULIB_MALLOC_GNU && !HAVE_MALLOC_GNU) */
#endif /* malloc */

#include <stdlib.h>

#include <errno.h>

/* Allocate an N-byte block of memory from the heap.
 * If N is zero, allocate a 1-byte block.  */

#ifndef rpl_malloc
/* prototype */
void * rpl_malloc (size_t n);
#endif /* !rpl_malloc */

void * rpl_malloc(size_t n)
{
	void *result;

#if (defined(NEED_MALLOC_GNU) && NEED_MALLOC_GNU)
	if (n == 0) {
		n = 1;
	}
#endif /* NEED_MALLOC_GNU */

	result = malloc(n);

#if !defined(HAVE_MALLOC_POSIX) || !HAVE_MALLOC_POSIX
	if (result == NULL) {
		errno = ENOMEM;
	}
#endif /* !HAVE_MALLOC_POSIX */

	return result;
}

/* EOF */
