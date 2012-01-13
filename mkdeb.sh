#! /usr/bin/env sh

set -e -x

if [ ! -e ./configure ]; then
    ./autogen.sh
fi

./configure

make distcheck

VERSION="$(./configure --version | head -n1 | cut -d ' ' -f 3)"

ln -f betaradio-${VERSION}.tar.bz2 betaradio_${VERSION}.orig.tar.bz2

tar xf betaradio_${VERSION}.orig.tar.bz2

cp -a ./debian betaradio-${VERSION}

cd betaradio-${VERSION}

debuild -S -sa -us -uc

cd ..

CODENAME="$(lsb_release -c -s)"

if uname -a | grep x86_64; then
    ARCH="amd64"
else
    ARCH="i386"
fi

DSC="$(ls betaradio_${VERSION}-*.dsc | head -n1)"

pbuilder-dist $CODENAME $ARCH build --buildresult $PWD $DSC
