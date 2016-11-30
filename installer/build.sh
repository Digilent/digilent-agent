#!/bin/bash
printf "Copying build to data directory...\n"
rm -r packages/com.digilent.wfl/data/*
cp ../release/* packages/com.digilent.wfl/data/
cp ../config.ini packages/com.digilent.wfl/data/

printf "Building Component...\n"
cd packages/com.digilent.wfl/data
windeployqt.exe earlgrey.exe

echo "Building Installer...\n"
cd ../../..
binarycreator.exe -c config/config.xml -p packages waveforms-live-agent-0.1.0.exe
