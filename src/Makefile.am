noinst_HEADERS = gettext.h gstplay.h

bin_PROGRAMS = betaradio

betaradio_SOURCES = \
					betaradio.c \
					gstplay.c

betaradio_CPPFLAGS = \
                     @gtk2_CFLAGS@ \
                     @json_CFLAGS@ \
                     @gst_CFLAGS@ \
                     -DDATADIR=\"$(datadir)\" \
                     -DLOCALEDIR=\"$(localedir)\"

betaradio_LDFLAGS = \
                     @gtk2_LIBS@ \
                     @json_LIBS@ \
                     @gst_LIBS@

LDADD = $(LIBINTL)
