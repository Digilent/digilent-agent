#!/bin/bash
TARGET=$1
VERSION=$2

if [ -z "$1" ]; then
    echo "Error: Please specifiy the traget platform"
    exit 1
fi

if [ $TARGET = "win" ]; then
    printf "Preparing to build for Windows...\n"
fi
if [ $TARGET = "linux" ]; then
    printf "Preparing to build for Linux...\n"
fi

#Copy build files into installer directory
printf "Copying build to data directory...\n"
rm -r packages/com.digilent.waveformsliveagent/data/*
cp ../release/* packages/com.digilent.waveformsliveagent/data/
#cp ../config.ini packages/com.digilent.wfl/data/

#Automatically Pull Windows Dependencies
if [ $TARGET = "win" ]; then
    printf "Pulling Component Dependencies...\n"
    cd packages/com.digilent.waveformsliveagent/data
    windeployqt waveforms-live-agent.exe
    #Copy additional dependencies
        #missing depends for win32
        #"C:\Qt\5.7\mingw53_32\bin\libstdc++-6.dll"
        #"C:\Qt\5.7\mingw53_32\bin\libgcc_s_dw2-1.dll"
        #"C:\Qt\5.7\mingw53_32\bin\libwinpthread-1.dll"
    cd ../../..
    cp depends/* packages/com.digilent.waveformsliveagent/data/
fi

printf "Building Installer...\n"
if [ $TARGET = "win" ]; then
    binarycreator -c config/config.xml -p packages "waveformsliveagent-$VERSION.exe"
fi
if [ $TARGET = "linux" ]; then
    binarycreator -c config/config.xml -p packages waveformsliveagent-0.1.2.run
fi

