#!/bin/bash
TARGET=$1
VERSION=$2

if [ -z "$1" ]; then
    echo "Error: Please specifiy the traget platform"
    exit 1
fi

if [ $TARGET = "win" ]; then
    printf "Preparing to build for Windows...\n"
    ./winBuildScript.sh $VERSION
fi
if [ $TARGET = "deb" ]; then
    printf "Preparing to build for Linux...\n"
    ./debBuildScript.sh $VERSION
fi


