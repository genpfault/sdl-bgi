#!/bin/sh

VERSION=$(cat VERSION)

mkdir -p build && cd build/
cmake -DCMAKE_INSTALL_PREFIX=/usr ..
PACKAGE_DONE=0

# is this system .deb based?
DEB=$(which dpkg 2> /dev/null)
if [ $? = 0 ] ; then
  DEB_ARCH=$(dpkg --print-architecture)
  printf "\n*** This system looks Debian-based.\n\n"
  cpack -G "DEB" ..
  DEB_NAME="sdl_bgi_"$VERSION"-1_"$DEB_ARCH".deb"
  /bin/mv *deb $DEB_NAME
  printf "\n*** Package $DEB_NAME created in subdirectory build/.\n\n"
  PACKAGE_DONE=1
fi

# is this system .rpm based?
RPM=$(which rpm 2> /dev/null)
if [ $? = 0 ] ; then
  RPM_ARCH=$(uname -m)
  printf "\n*** This system looks RPM-based.\n\n"
  cpack -G "RPM" ..
  RPM_NAME="SDL_bgi-"$VERSION"-1."$RPM_ARCH".rpm"
  /bin/mv *rpm $RPM_NAME
  printf "\n*** Package $RPM_NAME created in subdirectory build/.\n\n"
  PACKAGE_DONE=1
fi

if [ $PACKAGE_DONE = 0 ] ; then
  printf "\n*** I don't know what package to build.\n\n"
fi
