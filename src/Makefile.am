noinst_HEADERS = gstplay.h json_cat.h

bin_PROGRAMS = betaradio

betaradio_SOURCES = \
                    betaradio.c \
                    json_cat.c \
                    gstplay.c

betaradio_CPPFLAGS = \
                     @gtk2_CFLAGS@ \
                     @json_CFLAGS@ \
                     @soup_CFLAGS@ \
                     @gst_CFLAGS@ \
                     -DDATADIR=\"$(datadir)\" \
                     -DLOCALEDIR=\"$(localedir)\"

betaradio_LDFLAGS = \
                     @gtk2_LIBS@ \
                     @json_LIBS@ \
                     @soup_LIBS@ \
                     @gst_LIBS@

LDADD = $(LIBINTL)
