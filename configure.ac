# Process this file with autoconf to produce a configure script.

AC_INIT(quickxorhash, m4_esyscmd([bash version.sh]), paul.arthur@flowerysong.com)
AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-xz])
AC_PREREQ(2.63)
AC_CONFIG_HEADER(config.h)
AC_COPYRIGHT([Copyright (c) 2018 Paul Arthur MacIain])

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_SYS_LARGEFILE

PKG_CHECK_MODULES([LIBCRYPTO], [libcrypto])

AC_CONFIG_FILES(Makefile test/Makefile packaging/pkgconfig/quickxorhash.pc packaging/rpm/quickxorhash.spec)
AC_OUTPUT
