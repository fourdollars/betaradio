#!/bin/sh

autopoint --force
aclocal -I m4
autoheader
libtoolize --automake --copy
automake --add-missing --copy --foreign --include-deps
autoconf
