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
AM_CFLAGS=-Wall -Wstrict-prototypes -Wmissing-prototypes -Wimplicit $(OPT)
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

DESTDIR=

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

all: libdl.a libdl.dylib dlopen.3

regenerate_configure: configure.ac
	autoconf --force --warnings=all
.PHONY: regenerate_configure

dist-local: $(DISTFILES)
	tar -cvf dlcompat-$(VERSION).tar $(DISTFILES)
	if test -d dlcompat-$(VERSION); then  rm -rf dlcompat-$(VERSION); fi;
	mkdir dlcompat-$(VERSION)
	tar -C dlcompat-$(VERSION) -xvf dlcompat-$(VERSION).tar
	rm dlcompat-$(VERSION).tar
	tar -cvzf dlcompat-$(VERSION).tar.gz dlcompat-$(VERSION)
	rm -rf dlcompat-$(VERSION)
.PHONY: dist-local

install-dlcompat: all man
	$(INSTALL) -d -m 755 $(DESTDIR)/$(libdir)/
	$(INSTALL) -m 644 libdl.a $(DESTDIR)/$(libdir)/
	$(INSTALL) -m 644 libdl.$(LIBVER).dylib $(DESTDIR)/$(libdir)/
	ln -fs libdl.$(LIBVER).dylib $(DESTDIR)/$(libdir)/libdl.dylib
	$(INSTALL) -d -m 755 $(DESTDIR)/$(includedir)/
	$(INSTALL) -m 644 dlfcn.h $(DESTDIR)/$(includedir)/
.PHONY: install-dlcompat

dlopen.3: dlopen.3.in
	$(SED) -e "s/%%NOTPREPEND%%/$(NOTPREPEND)/" < dlopen.3.in > dlopen.3

man: dlopen.3
	$(INSTALL) -d -m 755 $(DESTDIR)/$(mandir)/man3/
	$(INSTALL) -m 644 dlopen.3 $(DESTDIR)/$(mandir)/man3/dlopen.3
	$(INSTALL) -m 644 dladdr.3 $(DESTDIR)/$(mandir)/man3/dladdr.3	
	ln -fs dlopen.3 $(DESTDIR)/$(mandir)/man3/dlsym.3
	ln -fs dlopen.3 $(DESTDIR)/$(mandir)/man3/dlclose.3
	ln -fs dlopen.3 $(DESTDIR)/$(mandir)/man3/dlerror.3
		
.c.o:
	$(CC) $(AM_CFLAGS) $(CFLAGS) $(AM_CPPFLAGS) $(CPPFLAGS) $(DEFS) $(DEFINES) -o $@ -c $<

libdl.a: $(OBJS) $(LIBOBJS)
	$(AR) $(ARFLAGS) libdl.a $(OBJS) $(LIBOBJS)
	$(RANLIB) libdl.a

libdl.dylib: libdl.$(LIBVER).dylib
	$(LN_S) -f libdl.$(LIBVER).dylib $@

libdl.$(LIBVER).dylib: $(OBJS) $(LIBOBJS)
	$(CC) -dynamiclib -o $@ $(OBJS) $(LIBOBJS) -install_name $(prefix)/lib/$@ $(VERSTRING)

clean-local:
	rm -f $(OBJS) libdl.* *~ *.o dlopen.3
.PHONY: clean-local

distclean-local: clean-local
	rm -rf autom4te.cache || rmdir autom4te.cache
.PHONY: distclean-local