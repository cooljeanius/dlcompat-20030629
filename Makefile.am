# Copyright (c) 2002 Jorge Acereda  <jacereda@users.sourceforge.net> &
#                    Peter O'Gorman <ogorman@users.sourceforge.net>
#                   
# Portions may be copyright others, see the AUTHORS file included with this
# distribution.    
# Maintained by Peter O'Gorman <ogorman@users.sourceforge.net>
#
# Bug Reports and other queries should go to <ogorman@users.sourceforge.net>
#
#
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = test

prefix=@prefix@
exec_prefix = @exec_prefix@
mandir = @mandir@
includedir = @includedir@
libdir = @libdir@
datarootdir = @datarootdir@
DEBUG=0
CC=@CC@
OPT=@OPT@
DEBUGOPT=-g -O0
AM_CFLAGS=-Wall -Wstrict-prototypes -Wmissing-prototypes -Wimplicit -Wno-multichar $(OPT)
AM_CPPFLAGS=-I.
AM_CXXFLAGS=-Wno-deprecated
AR=@AR@
ARFLAGS=cru
RANLIB=@RANLIB@
INSTALL=@INSTALL@
VERSTRING=@VERSTRING@
LIBVER=@LIBVER@

DEFINES=@DEBUGDEF@ @FINKDEF@
VERSION=@VERSION@
NOTPREPEND=@NOTPREPEND@

DESTDIR=@DESTDIR@

AM_DISTFILES= README \
	configure \
	configure.ac \
	dlfcn.c \
	dlfcn_simple.c \
	dlfcn.h \
	ChangeLog \
	Makefile.in \
	TODO \
	AUTHORS \
	LICENSE \
	install-sh \
	dlopen.3.in \
	dladdr.3 \
	test/Makefile test/dltest.c test/dlmodule.c test/dladdr.c
		
OBJS = dlfcn.o
SIMPLE_OBJS = dlfcn_simple.o

all: libdl.a libdl.dylib libdl_simple.dylib dlopen.3

regenerate_configure: configure.ac
	autoconf --force --warnings=all
.PHONY: regenerate_configure

dist-local: $(DISTFILES)
	tar -cvf dlcompat-$(VERSION).tar $(DISTFILES)
	if test -d dlcompat-$(VERSION); then rm -rfv dlcompat-$(VERSION); fi;
	mkdir dlcompat-$(VERSION)
	tar -C dlcompat-$(VERSION) -xvf dlcompat-$(VERSION).tar
	rm dlcompat-$(VERSION).tar
	tar -cvzf dlcompat-$(VERSION).tar.gz dlcompat-$(VERSION)
	rm -rfv dlcompat-$(VERSION)
.PHONY: dist-local

install-exec-local: install-dlcompat
.PHONY: install-exec-local

install-dlcompat: all install-my-manpage
	$(INSTALL) -d -m 755 $(DESTDIR)/$(libdir)/
	$(INSTALL) -m 644 libdl.a $(DESTDIR)/$(libdir)/
	$(INSTALL) -m 644 libdl.$(LIBVER).dylib $(DESTDIR)/$(libdir)/
	$(LN_S) -fv libdl.$(LIBVER).dylib $(DESTDIR)/$(libdir)/libdl.dylib
	$(INSTALL) -d -m 755 $(DESTDIR)/$(includedir)/
	$(INSTALL) -m 644 $(srcdir)/dlfcn.h $(DESTDIR)/$(includedir)/
.PHONY: install-dlcompat

dist_man_MANS = dladdr.3
nodist_man_MANS = dlopen.3

EXTRA_DIST = dlopen.3.in

dlopen.3: $(srcdir)/dlopen.3.in
	$(SED) -e "s/%%NOTPREPEND%%/$(NOTPREPEND)/" < $(srcdir)/dlopen.3.in > dlopen.3

install-data-local: install-my-manpage
.PHONY: install-data-local

install-my-manpage: dlopen.3
	$(INSTALL) -d -m 755 $(DESTDIR)/$(mandir)/man3/
	$(INSTALL) -m 644 dlopen.3 $(DESTDIR)/$(mandir)/man3/dlopen.3
	$(INSTALL) -m 644 $(srcdir)/dladdr.3 $(DESTDIR)/$(mandir)/man3/dladdr.3
	$(LN_S) -fv dlopen.3 $(DESTDIR)/$(mandir)/man3/dlsym.3
	$(LN_S) -fv dlopen.3 $(DESTDIR)/$(mandir)/man3/dlclose.3
	$(LN_S) -fv dlopen.3 $(DESTDIR)/$(mandir)/man3/dlerror.3

lib_LIBRARIES = libdl.a libdl_simple.a
libdl_a_SOURCES = dlfcn.c
libdl_a_LIBADD = $(LIBOBJS)
## I avoided adding the ALLOCA object, because the header that goes with
## the source for it requires actually using gnulib properly...
libdl_simple_a_SOURCES = dlfcn_simple.c
libdl_simple_a_LIBADD = $(MY_DYLD_LIBOBJS)

## avoid installing gnulib-provided headers:
noinst_HEADERS = error.h progname.h
## headers that actually came with this project can be installed:
include_HEADERS = dlfcn.h

manually_build_libdl.a: $(OBJS) $(LIBOBJS)
	$(AR) $(ARFLAGS) libdl.a $(OBJS) $(LIBOBJS)
	$(RANLIB) libdl.a
.PHONY: manually_build_libdl.a

# I am unsure whether it would be worth it to switch to libtool for the
# dynamic library targets, as by that point you might as well already have
# full dynamic linking capabilities, which would be self-defeating...
libdl.dylib: libdl.$(LIBVER).dylib
	$(LN_S) -fv libdl.$(LIBVER).dylib $@

libdl_simple.dylib: libdl_simple.$(LIBVER).dylib
	$(LN_S) -fv libdl_simple.$(LIBVER).dylib $@

libdl.$(LIBVER).dylib: $(OBJS) $(LIBOBJS)
	$(CC) -dynamiclib -o $@ $(OBJS) $(LIBOBJS) -install_name $(prefix)/lib/$@ $(VERSTRING)

libdl_simple.$(LIBVER).dylib: $(SIMPLE_OBJS) $(MY_DYLD_LIBOBJS)
	$(CC) -dynamiclib -o $@ $(SIMPLE_OBJS) $(MY_DYLD_LIBOBJS) -install_name $(prefix)/lib/$@ $(VERSTRING)

clean-local:
	rm -fv $(OBJS) libdl.* libdl_simple.* *~ *.o *.dylib dlopen.3
.PHONY: clean-local

DISTCLEANFILES = *~ .DS_Store Makefile*orig

distclean-local: clean-local
	rm -rfv autom4te.cache || rmdir autom4te.cache
.PHONY: distclean-local
