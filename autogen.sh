#!/bin/sh

cp -av /usr/share/gettext/gettext.h src/
autopoint --force
aclocal -I m4
autoheader
libtoolize --automake --copy
automake --add-missing --copy --foreign --include-deps
autoconf
