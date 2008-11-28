AC_INIT(betaradio, 0.1.2, Shih-Yuan Lee (FourDollars) <fourdollars@gmail.com>)

AM_INIT_AUTOMAKE([foreign])

AM_CONFIG_HEADER(config.h)

AC_PROG_CC

PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.6],, AC_MSG_ERROR([gtk+-2.0 >= 2.6 not found]))
PKG_CHECK_MODULES([curl], [libcurl >= 7.15],, AC_MSG_ERROR([libcurl >= 7.15 not found]))
PKG_CHECK_MODULES([gst], [gstreamer-0.10 >= 0.10],, AC_MSG_ERROR([gstreamer-0.10 >= 0.10 not found]))

AC_ARG_ENABLE([tv],
              AS_HELP_STRING([--enable-tv], [enable live TV channel (default: "no")]),
              [case "${enableval}" in
               ("yes") AC_DEFINE_UNQUOTED([TV], [1], ['Live TV Channel']) ;;
               (*) ;;
               esac],)

AC_CONFIG_FILES([
Makefile
data/Makefile
src/Makefile
data/betaradio.desktop
])

AC_OUTPUT
