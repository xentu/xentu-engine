#!/bin/bash


echo "XentuSDK MacOS packaging script."
echo "--------------------------------"


# Setup some constants.
INST_PATH="$PWD"
XEN_VERSION=`cat ../version.txt`
XEN_ARCH="all"
XEN_BASE="${INST_PATH}/../out/darwin"
XEN_INST="${XEN_BASE}/xentusdk_${XEN_VERSION}_${XEN_ARCH}-mac.run"


# remove the old build folder.
sudo rm -rf $XEN_BASE


# Create the directory structure (if it doesnt already exist)
echo "Making directory structure..."
mkdir -p $XEN_BASE/files
mkdir -p $XEN_BASE/files/templates


# Copy the engine files into the directories
cp ../out/build/clang/xentu $XEN_BASE/files
cp ../out/build/clang/xentusdk $XEN_BASE/files
cp ../data/logo.png $XEN_BASE/files


# Copy the examples into the build directory
cd "$INST_PATH/../data/examples/platformer/"; pwd; zip -r $XEN_BASE/files/templates/platformer.zip *
cd "$INST_PATH/../data/examples/pong/"; pwd; zip -r $XEN_BASE/files/templates/pong.zip *
cd "$INST_PATH/../data/examples/space-hopper/"; pwd; zip -r $XEN_BASE/files/templates/space-hopper.zip *
cd "$INST_PATH/../data/examples/top-down/"; pwd; zip -r $XEN_BASE/files/templates/top-down.zip *
cd "$INST_PATH"


# Create the install script.
echo -e "#!/bin/bash
echo \"XentuSDK Installer for MacOS\"
echo \"----------------------------\"
mkdir -p /usr/local/share/xentusdk/templates
echo \"Copying files...\"
cp ./xentu /usr/local/share/xentusdk
cp ./xentusdk /usr/local/share/xentusdk
cp ./logo.png /usr/local/share/xentusdk
cp ./templates/platformer.zip /usr/local/share/xentusdk/templates
cp ./templates/pong.zip /usr/local/share/xentusdk/templates
cp ./templates/space-hopper.zip /usr/local/share/xentusdk/templates
cp ./templates/top-down.zip /usr/local/share/xentusdk/templates
echo \"Creating aliases...\"
ln -s /usr/local/share/xentusdk/xentu /usr/local/bin/xentu
ln -s /usr/local/share/xentusdk/xentusdk /usr/local/bin/xentusdk
echo \"Install complete! type xentusdk to get started.\"
" >> $XEN_BASE/files/install.sh


# Set needed ownerships.
sudo chown root:root -R $XEN_BASE
sudo chmod 755 $XEN_BASE/files/xentu
sudo chmod 755 $XEN_BASE/files/xentusdk
sudo chmod 755 $XEN_BASE/files/install.sh


# Call makeself to generate the 
# makeself.sh [args] archive_dir file_name label startup_script [script_args]
makeself $XEN_BASE/files/ $XEN_INST "Xentu Game Engine SDK" ./install.sh


echo "---------------------------------"
echo "XentuSDK package prepared!"