#!/bin/sh

set -e

cp -av /usr/share/gettext/gettext.h src/
autoreconf -ifs
