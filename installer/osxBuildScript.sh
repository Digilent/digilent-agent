VERSION=$1
BUILDTYPE=$2
DATE=`date +%Y-%m-%d`

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR=$DIR/working
TARGET_APP=$BUILD_DIR"/Digilent Agent.app"

#Remove any old files
printf "Cleaning build directory\n"
rm -r -f "$BUILD_DIR/"

#Create build directory if it does not exist
if [ ! -d "$BUILD_DIR" ]; then
  printf "Creating build directory\n"
  mkdir -p $BUILD_DIR
fi

printf "Copying files to build directory\n"
cp -r ../Release/digilent-agent.app "$TARGET_APP"
ln -s /Users/Shared working/Shared
mkdir -p working/digilent/digilent-agent/www
cp -r waveforms-live-offline/ working/digilent/digilent-agent/www


printf "Packaging Dependencies...\n"
macdeployqt "$TARGET_APP" -codesign="Mac Developer: software@" -always-overwrite

printf "Signing...\n"
codesign --force --verify --verbose --sign "Mac Developer: software" "$TARGET_APP"
find "$TARGET_APP" -name *.dylib | xargs -I $ codesign --force --verify --verbose --sign "Mac Developer: software" $
find "$TARGET_APP" -name Qt* -type f | xargs -I $ codesign --force --verify --verbose --sign "Mac Developer: software" $

printf "Creating .dmg...\n"
./create-dmg/create-dmg.sh \
--volname "Digilent Agent $VERSION" \
--background ../images/digilent-agent-dmg-background.png \
--window-size 640 430 \
--icon-size 64 \
--icon "Digilent\ Agent.app" 240 160 \
--app-drop-link 400 160 \
--icon "digilent" 240 300 \
--icon "Shared" 400 300 \
digilent-agent-$VERSION.dmg \
working/ \
\