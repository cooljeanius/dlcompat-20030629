/* dlmodule.c
 * Copyright (c) 2002 Francis James Franklin <fjf@alinameridon.com>
 *
 * Maintained by Peter O'Gorman <ogorman@users.sourceforge.net>
 *
 * Bug Reports and other queries should go to <ogorman@users.sourceforge.net>
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

#ifdef HAVE_CONFIG_H
# include "../config.h"
#else
# ifndef DLMODULE_C_NON_AUTOHEADER_BUILD
#  define DLMODULE_C_NON_AUTOHEADER_BUILD 1
# endif /* !DLMODULE_C_NON_AUTOHEADER_BUILD */
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

extern void _init(void);
extern void _fini(void);
extern void test(int number);

/* DL_MODULE_NUMBER needs to be defined with a compiler flag
 * (see the Makefile for examples) */
void _init(void)
{
  fprintf(stderr, "dlcompat: dl%s: _init()\n", DL_MODULE_NUMBER);
}

void _fini(void)
{
  fprintf(stderr, "dlcompat: dl%s: _fini()\n", DL_MODULE_NUMBER);
}

void test(int number)
{
  fprintf(stderr, "dlcompat: dl%s: test(%d)\n", DL_MODULE_NUMBER, number);
}

/* EOF */
