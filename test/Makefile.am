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

all: dlone.so dltwo.so dllib.dylib dllib.a dltest dladdr dlsymtest

dlone.so: dlmodule.c
	$(CC) $(CFLAGS) -bundle -o dlone.so dlmodule.c -DDL_MODULE_NUMBER=\"one\"

dltwo.so: dlmodule.c
	$(CC) $(CFLAGS) -bundle -o dltwo.so dlmodule.c -DDL_MODULE_NUMBER=\"two\"

dlmodule.o: dlmodule.c
	$(CC) $(CFLAGS) -c -o dlmodule.o dlmodule.c -fno-common -DPIC -DDL_MODULE_NUMBER=\"lib\"

dllib.dylib: dlmodule.o
	$(CC) $(CFLAGS) -dynamiclib -o dllib.dylib dlmodule.o

dllib.a: dlmodule.o
	$(AR) cru dllib.a dlmodule.o
	$(RANLIB) dllib.a

dltest: dltest.c
	$(CC) $(CFLAGS) -o dltest dltest.c -I.. -L.. -ldl
dladdr:	dladdr.c
	$(CC) $(CFLAGS) -o dladdr dladdr.c -I.. -L.. -ldl
		
dlsymtest: dlsymtest.c libfoo.dylib
	$(CC) $(CFLAGS) -o dlsymtest  dlsymtest.c -I.. -L.. -L. -ldl -lfoo
        			
libfoo.o: libfoo.c
	$(CC) $(CFLAGS) -c -o libfoo.o libfoo.c
  
libfoo.dylib: libfoo.o
	$(CC) $(CFLAGS) -dynamiclib -o libfoo.dylib libfoo.o

clean-local:		
	rm -f *~ *.o *.so *.dylib *.a dltest dladdr dlsymtest
	rm -rf *.dSYM || rmdir *.dSYM

DISTCLEANFILES = .DS_Store

# use installcheck instead of check because it requires libdl to be
# installed due to the use of the '-install_name' flag:
installcheck-local: atlocal dlone.so dltwo.so dllib.dylib dllib.a dltest dladdr dlsymtest
	test -e ./dladdr && ./dladdr 2>&1
	test -e ./dltest && ./dltest
	test -e ./dlsymtest && ./dlsymtest
