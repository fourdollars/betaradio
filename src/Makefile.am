include_HEADERS = \
                  streamplayer.h \
                  channel.h \
                  eggtrayicon.h \
                  sitelist.h \
                  gettext.h

bin_PROGRAMS = betaradio

betaradio_SOURCES = \
					betaradio.c \
					eggtrayicon.c \
					channel.c \
					gstplay.c

betaradio_CPPFLAGS = \
                     @gtk2_CFLAGS@ \
                     @curl_CFLAGS@ \
                     @gst_CFLAGS@ \
                     -DEGG_COMPILATION \
                     -DDATADIR=\"$(datadir)\" \
                     -DLOCALEDIR=\"$(localedir)\"

betaradio_LDFLAGS = \
                     @gtk2_LIBS@ \
                     @curl_LIBS@ \
                     @gst_LIBS@

LDADD = $(LIBINTL)
