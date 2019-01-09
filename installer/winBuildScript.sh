VERSION=$1
BUILDTYPE=$2
DATE=`date +%Y-%m-%d`

#Generate config files from templates
printf "Generating Config Files"
sed "s/VERSION_DUMMY/$VERSION/g" config/winConfigTemplate.xml > config/winConfig.xml

cp packages/com.digilent.agent/meta/packageTemplate.xml packages/com.digilent.agent/meta/package.xml 
sed -i "s/VERSION_DUMMY/$VERSION/g" packages/com.digilent.agent/meta/package.xml 
sed -i "s/DATE_DUMMY/$DATE/g" packages/com.digilent.agent/meta/package.xml 

#Copy build files into installer directory
printf "Copying build to data directory...\n"
rm -r ./packages/com.digilent.agent/data/*

if [ $BUILDTYPE = "debug" ]; then
    cp ../debug/* packages/com.digilent.agent/data/
    echo "debug build $VERSION" > debug-build.txt
else
    cp ../release/* packages/com.digilent.agent/data/
fi

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
if [ $BUILDTYPE = "debug" ]; then
    binarycreator --offline-only -c config/winConfig.xml -p packages "digilent-agent-$VERSION-debug.exe"
else
    binarycreator --offline-only -c config/winConfig.xml -p packages "digilent-agent-$VERSION.exe"
fi


if [ $BUILDTYPE = "debug" ]; then
    printf "Skipping repos for debug build"
else
    printf "Building repositories"
    rm -r repo/
    repogen.exe -p packages repo
fi

