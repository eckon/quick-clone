#!/usr/bin/env bash

# needs to be run in the root folder

# generate the needed files with cmake/make
# install linuxdeploy (in this root dir for execution)
# https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage

VERSION="v0.1.0"
export VERSION

./linuxdeploy-x86_64.AppImage \
  -e build/bin/qc \
  --appdir AppDir/ \
  --desktop-file generate/qc.desktop \
  --icon-file generate/qc.svg \
  --output appimage
