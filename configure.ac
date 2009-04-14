AC_INIT([betaradio], [0.1.2], [http://code.google.com/p/betaradio])
AC_PREREQ([2.61])
AC_CONFIG_SRCDIR([src/betaradio.c])
AC_CONFIG_AUX_DIR([build-aux])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign dist-bzip2])

AM_GNU_GETTEXT_VERSION(0.17)
AM_GNU_GETTEXT([external])

AC_PROG_CC

PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.6],, AC_MSG_ERROR([gtk+-2.0 >= 2.6 not found]))
PKG_CHECK_MODULES([curl], [libcurl >= 7.15],, AC_MSG_ERROR([libcurl >= 7.15 not found]))
PKG_CHECK_MODULES([gst], [gstreamer-0.10 >= 0.10],, AC_MSG_ERROR([gstreamer-0.10 >= 0.10 not found]))

AC_CONFIG_FILES([
                 Makefile
                 po/Makefile.in
                 data/Makefile
                 src/Makefile
                 data/betaradio.desktop
                 ])

AC_OUTPUT
