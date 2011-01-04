#! /usr/bin/env sh

set -e -x

if [ ! -e ./configure ]; then
    ./autogen.sh
fi

make dist

VERSION="$(./configure --version | head -n1 | cut -d ' ' -f 3)"

ln betaradio-${VERSION}.tar.bz2 betaradio_${VERSION}.orig.tar.bz2

tar xf betaradio_${VERSION}.orig.tar.bz2

cp -a ./debian betaradio-${VERSION}

cd betaradio-${VERSION}

case "$*" in
    ('-S')
        debuild -S -sa
        ;;
    ('-P')
        pdebuild
        ;;
    ('-PS')
        pdebuild --auto-debsign
        ;;
    (*)
        debuild -us -uc -tc
        ;;
esac

cd ..
