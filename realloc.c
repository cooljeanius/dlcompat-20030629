/* realloc.c: realloc() function that is glibc compatible.
 *
 * Copyright (C) 1997, 2003-2004, 2006-2007, 2009-2012 Free Software
 * Foundation, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* written by Jim Meyering and Bruno Haible */

#ifndef _GL_USE_STDLIB_ALLOC
# define _GL_USE_STDLIB_ALLOC 1
#endif /* !_GL_USE_STDLIB_ALLOC */
#include <config.h>

/* Only the AC_FUNC_REALLOC macro defines 'realloc' already in config.h: */
#ifdef realloc
# define NEED_REALLOC_GNU 1
#else
/* Whereas the gnulib module 'realloc-gnu' defines HAVE_REALLOC_GNU: */
# if (defined(GNULIB_REALLOC_GNU) && GNULIB_REALLOC_GNU) && !HAVE_REALLOC_GNU
#  define NEED_REALLOC_GNU 1
# endif /* (GNULIB_REALLOC_GNU && !HAVE_REALLOC_GNU) */
#endif /* realloc */

/* Infer the properties of the system's malloc function.
 * The gnulib module 'malloc-gnu' should define HAVE_MALLOC_GNU for us: */
#if (defined(GNULIB_MALLOC_GNU) && GNULIB_MALLOC_GNU) && HAVE_MALLOC_GNU
# define SYSTEM_MALLOC_GLIBC_COMPATIBLE 1
#endif /* GNULIB_MALLOC_GNU && HAVE_MALLOC_GNU */

#include <stdlib.h>

#include <errno.h>

/* Change the size of an allocated block of memory P to N bytes,
 * with error checking. If N is zero, change it to 1. If P is NULL,
 * use malloc. */

#ifndef rpl_realloc
/* prototype: */
void * rpl_realloc(void *p, size_t n);
#endif /* !rpl_realloc */

void *rpl_realloc(void *p, size_t n)
{
  void *result;

#if (defined(NEED_REALLOC_GNU) && NEED_REALLOC_GNU)
  if (n == 0) {
      n = 1;

      /* In theory realloc might fail, so do NOT rely on it to free: */
      free(p);
      p = NULL;
  }
#endif /* NEED_REALLOC_GNU */

  if (p == NULL) {
#if (defined(GNULIB_REALLOC_GNU) && GNULIB_REALLOC_GNU) && !NEED_REALLOC_GNU && !SYSTEM_MALLOC_GLIBC_COMPATIBLE
      if (n == 0) {
		  n = 1;
	  }
#endif /* GNULIB_REALLOC_GNU && !NEED_REALLOC_GNU && !SYSTEM_MALLOC_GLIBC_COMPATIBLE */
      result = malloc(n);
  } else {
	  result = realloc(p, n);
  }

#if !defined(HAVE_REALLOC_POSIX) || !HAVE_REALLOC_POSIX
  if (result == NULL) {
	  errno = ENOMEM;
  }
#endif /* !HAVE_REALLOC_POSIX */

  return result;
}

/* EOF */
