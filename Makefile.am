ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS = -Wall -Wextra $(LIBCRYPTO_CFLAGS)

SUBDIRS = . test

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libqxh.la
libqxh_la_LIBADD = $(LIBCRYPTO_LIBS)
libqxh_la_SOURCES = quickxorhash.c quickxorhash.h

include_HEADERS = quickxorhash.h

bin_PROGRAMS = quickxorhash
quickxorhash_LDADD = libqxh.la
quickxorhash_SOURCES = cli.c

nodist_pkgconfig_DATA = packaging/pkgconfig/quickxorhash.pc

EXTRA_DIST = VERSION packaging/rpm/quickxorhash.spec

rpm: dist-xz
	rpmbuild -ta $(distdir).tar.xz
