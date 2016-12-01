#!/bin/bash
printf "Copying build to data directory...\n"
rm -r packages/com.digilent.wfl/data/*
cp ../release/* packages/com.digilent.wfl/data/
cp ../config.ini packages/com.digilent.wfl/data/

#missing depends for win32
#"C:\Qt\5.7\mingw53_32\bin\libstdc++-6.dll"
#"C:\Qt\5.7\mingw53_32\bin\libgcc_s_dw2-1.dll"
#"C:\Qt\5.7\mingw53_32\bin\libwinpthread-1.dll"

printf "Building Component...\n"
cd packages/com.digilent.wfl/data
windeployqt.exe earlgrey.exe

echo "Building Installer...\n"
cd ../../..
binarycreator.exe -c config/config.xml -p packages waveforms-live-agent-0.1.0.exe
