#!/bin/bash
VERSION=$1
BUILD_DIR="build"
TARGET_DIR="$BUILD_DIR/waveforms-live-agent-$VERSION"

#Warn user if no version number was passed
if [ -z "$1" ]; then
    echo "Error: Please specifiy the build version"
    exit 1
fi


#Remove any old files
printf "Cleaning build directory\n"
rm -r -f "$BUILD_DIR/"

#Create build directory if it does not exist
if [ ! -d "$TARGET_DIR" ]; then
  printf "Creating build directory\n"
  mkdir -p $TARGET_DIR 
fi

#Copy files into build directory
printf "Copying source files\n"
rsync ../ $TARGET_DIR --exclude="installer/" --exclude="debug/" --exclude="release/" -r

cd $TARGET_DIR

#Generate Makefile
printf "Generating Makefile\n"
qmake -o Makefile earlgrey.pro

#Preparing .deb package
printf "Preparing .deb package\n"
dh_make -s -c gpl -e software@digilent.com --createorig

#Building .deb package
printf "Building .deb package\n"
dpkg-buildpackage

