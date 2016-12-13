#!/bin/bash
os=$1

if [ $os = "win" ]; then
    printf "Preparing to build for Windows...\n"
fi
if [ $os = "linux" ]; then
    printf "Preparing to build for Linux...\n"
fi

#Copy build files into installer directory
printf "Copying build to data directory...\n"
rm -r packages/com.digilent.wfl/data/*
cp ../release/* packages/com.digilent.wfl/data/
cp ../config.ini packages/com.digilent.wfl/data/

#Automatically Pull Windows Dependencies
if [ $os = "win" ]; then
    printf "Pulling Component Dependencies...\n"
    cd packages/com.digilent.wfl/data
    windeployqt earlgrey.exe
    #Copy additional dependencies
        #missing depends for win32
        #"C:\Qt\5.7\mingw53_32\bin\libstdc++-6.dll"
        #"C:\Qt\5.7\mingw53_32\bin\libgcc_s_dw2-1.dll"
        #"C:\Qt\5.7\mingw53_32\bin\libwinpthread-1.dll"
    cd ../../..
    cp depends/* packages/com.digilent.wfl/data/
fi

printf "Building Installer...\n"
if [ $os = "win" ]; then
    binarycreator -c config/config.xml -p packages waveforms-live-agent-0.1.2.exe
fi
if [ $os = "linux" ]; then
    binarycreator -c config/config.xml -p packages waveforms-live-agent-0.1.2.run
fi

