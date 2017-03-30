VERSION=$1

#Copy build files into installer directory
printf "Copying build to data directory...\n"
rm -r ./packages/com.digilent.agent/data/*
cp ../release/* packages/com.digilent.agent/data/

#Automatically Pull Windows Dependencies
printf "Pulling Component Dependencies...\n"
cd packages/com.digilent.agent/data
windeployqt digilent-agent.exe
#Copy additional dependencies
    #missing depends for win32
    #"C:\Qt\5.7\mingw53_32\bin\libstdc++-6.dll"
    #"C:\Qt\5.7\mingw53_32\bin\libgcc_s_dw2-1.dll"
    #"C:\Qt\5.7\mingw53_32\bin\libwinpthread-1.dll"
cd ../../..
cp depends/* packages/com.digilent.agent/data/


printf "Building Installer...\n"
binarycreator -c config/winConfig.xml -p packages "digilent-agent-$VERSION.exe"
