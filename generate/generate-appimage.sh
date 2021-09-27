#!/usr/bin/env bash

# needs to be run in the root folder

# generate the needed files with cmake/make
# install linuxdeploy (in this root dir for execution)
# https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage

# this is done by the CI/CD, keeping it here for manual work
# VERSION="vX.X.X"
# export VERSION

./linuxdeploy-x86_64.AppImage \
  --executable build/bin/qc \
  --appdir AppDir/ \
  --desktop-file generate/qc.desktop \
  --icon-file generate/qc.svg \
  --output appimage
