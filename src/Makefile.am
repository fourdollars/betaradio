bin_PROGRAMS = betaradio

betaradio_SOURCES = \
					betaradio.c \
					eggtrayicon.c \
					channel.c \
					gstplay.c \
					gstplay.h \
					channel.h \
					eggtrayicon.h \
					sitelist.h \
					gettext.h
AM_CPPFLAGS = \
			  @gtk2_CFLAGS@ \
			  @curl_CFLAGS@ \
			  @gst_CFLAGS@ \
			  -DEGG_COMPILATION \
			  -DDATADIR=\"$(datadir)\" \
			  -DLOCALEDIR=\"$(localedir)\"
AM_LDFLAGS = \
			 @gtk2_LIBS@ \
			 @curl_LIBS@ \
			 @gst_LIBS@

LDADD = $(LIBINTL)
