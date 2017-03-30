VERSION=$1

#Copy build files into installer directory
printf "Copying build to data directory...\n"
rm -r ./packages/com.digilent.waveformsliveagent/data/*
cp -r ../release/waveforms-live-agent.app	 packages/com.digilent.waveformsliveagent/data/

printf "Building Installer...\n"
binarycreator -c config/osxConfig.xml -p packages "waveformsliveagent-$VERSION"
