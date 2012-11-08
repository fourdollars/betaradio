AM_CPPFLAGS = $(LTDLINCL)

bin_PROGRAMS = betaradio

noinst_HEADERS = any_tray_icon.h

betaradio_SOURCES = betaradio.vala \
                    gst_player.vala \
                    json_soup.vala \
                    any_tray_icon.c

betaradio_CPPFLAGS = --include config.h \
                     -DDATADIR=\"$(datadir)\" \
                     -DLOCALEDIR=\"$(localedir)\" \
                     @VALA_CFLAGS@

betaradio_LDFLAGS = @VALA_LIBS@

betaradio_LDADD = $(LIBLTDL)
betaradio_DEPENDENCIES = $(LTDLDEPS)

betaradio_VALAFLAGS = @VALA_FLAGS@ --vapidir=$(top_srcdir)/vapi --pkg config --pkg any_tray_icon
