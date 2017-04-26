#!/bin/bash
TARGET=$1
VERSION=$2
BUILDTYPE=$3

if [ -z "$1" ]; then
    echo "Error: Please specifiy the traget platform"
    exit 1
fi

if [ $TARGET = "win" ]; then
    printf "Preparing to build for Windows...\n"
    ./winBuildScript.sh $VERSION $BUILDTYPE
elif [ $TARGET = "osx" ]; then
    printf "Preparing to build for OSX...\n"
    ./osxBuildScript.sh $VERSION $BUILDTYPE
elif [ $TARGET = "deb" ]; then
    printf "Preparing to build for Linux...\n"
    ./debBuildScript.sh $VERSION $BUILDTYPE
else
    printf "Unknown target platform $TARGET"
fi


