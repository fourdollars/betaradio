AC_INIT(betaradio, 0.1.0, Shih-Yuan Lee (Fourdollars) <fourdollars@gmail.com>)

AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC

PKG_CHECK_MODULES([gtk2], [gtk+-2.0 >= 2.6])
PKG_CHECK_MODULES([curl], [libcurl >= 7.15])
PKG_CHECK_MODULES([gst], [gstreamer-0.10 >= 0.10])

AC_CONFIG_FILES([
Makefile
data/Makefile
src/Makefile
data/betaradio.desktop
])

AC_OUTPUT
