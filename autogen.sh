#!/bin/sh

aclocal
autoheader
automake --add-missing
autoconf
rm -fr autom4te.cache
