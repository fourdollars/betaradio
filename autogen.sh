#!/bin/sh

ln -snf $(locate gettext.h) gettext.h

autopoint -f
aclocal -I m4
autoheader
libtoolize --automake --copy
automake --add-missing --foreign
autoconf
