## automake Makefile for dlcompat testsuite

DEBUG=0
CC=@CC@
OPT=@OPT@
DEBUGOPT=-g -O0
WARN_CFLAGS=-Wall -Wstrict-prototypes -Wmissing-prototypes -Wimplicit
CFLAGS=@CFLAGS@ $(WARN_CFLAGS) $(OPT)
CXXFLAGS=-Wno-deprecated
DEFINES=@DEBUGDEF@ @FINKDEF@
AR=@AR@
RANLIB=@RANLIB@

EXTRA_DIST = atlocal.in

AUTOM4TE = $(SHELL) $(top_srcdir)/build-aux/missing --run autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

EXTRA_DIST += dladdr.c dlmodule.c dlsymtest.c dltest.c libfoo.c

all: dlone.so dltwo.so dllib.dylib dllib.a dltest dladdr dlsymtest

dlone.so: dlmodule.c
	$(CC) $(CFLAGS) -bundle -o dlone.so $(srcdir)/dlmodule.c -DDL_MODULE_NUMBER=\"one\"

dltwo.so: dlmodule.c
	$(CC) $(CFLAGS) -bundle -o dltwo.so $(srcdir)/dlmodule.c -DDL_MODULE_NUMBER=\"two\"

dlmodule.o: dlmodule.c
	$(CC) $(CFLAGS) -c -o dlmodule.o $(srcdir)/dlmodule.c -fno-common -DPIC -DDL_MODULE_NUMBER=\"lib\"

dllib.dylib: dlmodule.o
	$(CC) $(CFLAGS) -dynamiclib -o dllib.dylib dlmodule.o

dllib.a: dlmodule.o
	$(AR) cru dllib.a dlmodule.o
	$(RANLIB) dllib.a

dltest: dltest.c
	$(CC) $(CFLAGS) -o dltest $(srcdir)/dltest.c -I.. -L.. -ldl
dladdr:	dladdr.c
	$(CC) $(CFLAGS) -fleading-underscore -o dladdr $(srcdir)/dladdr.c -I.. -L.. -ldl
		
dlsymtest: dlsymtest.c libfoo.dylib
	$(CC) $(CFLAGS) -o dlsymtest $(srcdir)/dlsymtest.c -I.. -L.. -L. -ldl -lfoo
        			
libfoo.o: libfoo.c
	$(CC) $(CFLAGS) -c -o libfoo.o $(srcdir)/libfoo.c
  
libfoo.dylib: libfoo.o
	$(CC) $(CFLAGS) -dynamiclib -o libfoo.dylib libfoo.o

clean-local:		
	rm -fv *~ *.o *.so *.dylib *.a dltest dladdr dlsymtest
	rm -rf *.dSYM || rmdir *.dSYM

DISTCLEANFILES = .DS_Store Makefile*orig

# use installcheck instead of check because it requires libdl to be
# installed due to the use of the '-install_name' flag:
installcheck-local: atlocal dlone.so dltwo.so dllib.dylib dllib.a dltest dladdr dlsymtest
	test -e ./dladdr && ./dladdr 2>&1
	test -e ./dltest && ./dltest
	test -e ./dlsymtest && ./dlsymtest

# so that we still have a check target, just have it depend on all:
check-local: all
