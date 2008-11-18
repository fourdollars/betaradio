#!/bin/sh

aclocal
autoheader
libtoolize --automake --copy
automake --add-missing --foreign
autoconf
