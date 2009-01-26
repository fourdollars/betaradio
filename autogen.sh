#!/bin/sh

cp -a $(locate gettext.h) src/
autopoint --force
aclocal -I m4
autoheader
libtoolize --automake --copy
automake --add-missing --copy --foreign --include-deps
autoconf
