#!/bin/bash
VERSION=$1

DEBEMAIL="software@digilent.com"
DEBFULLNAME="Sam Kristoff"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR=$DIR/working
#TARGET_NAME="digilent-agent"
TARGET_NAME="digilent-agent"
TARGET_DIR="$BUILD_DIR/$TARGET_NAME-$VERSION"

echo $BUILD_DIR

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

#cp -R ../../debian .
#tar -czvf $BUILD_DIR"/"$TARGET_NAME"_"$VERSION.orig.tar.gz ../$TARGET_NAME-$VERSION
dh_make -s --createorig -c gpl3
cp ../../debian/* ./debian/
rm ./debian/*.ex
rm ./debian/*.EX

#Building .deb package
printf "Building .deb package\n"
dpkg-buildpackage -uc -us

