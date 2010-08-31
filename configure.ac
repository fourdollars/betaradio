AC_PREREQ([2.65])
AC_INIT([betaradio], [1.0], [http://code.google.com/p/betaradio])
AC_CONFIG_SRCDIR([src/betaradio.c])
AC_CONFIG_AUX_DIR([build-aux])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_GNU_GETTEXT_VERSION(0.17)
AM_GNU_GETTEXT([external])

AC_PROG_CC

PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.10],, AC_MSG_ERROR([gtk+-2.0 >= 2.10 not found]))
PKG_CHECK_MODULES([gst], [gstreamer-0.10 >= 0.10],, AC_MSG_ERROR([gstreamer-0.10 >= 0.10 not found]))
PKG_CHECK_MODULES([json], [json-glib-1.0],, AC_MSG_ERROR([json-glib-1.0 not found]))
PKG_CHECK_MODULES([soup], [libsoup-2.4],, AC_MSG_ERROR([libsoup-2.4 not found]))

AC_CONFIG_FILES([
                 Makefile
                 po/Makefile.in
                 data/Makefile
                 man/Makefile
                 src/Makefile
                 data/betaradio.desktop
                 ])

AC_OUTPUT
