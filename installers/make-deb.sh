#!/bin/bash


echo "XentuSDK debian packaging script."
echo "---------------------------------"


# Setup some constants.
INST_PATH="$PWD"
XEN_VERSION=`cat ../version.txt`
XEN_ARCH="all"
XEN_BASE="${INST_PATH}/../out/deb"
XEN_PATH="${XEN_BASE}/xentusdk_${XEN_VERSION}_${XEN_ARCH}"


# remove the old build folder.
sudo rm -rf $XEN_BASE


# Create the directory structure (if it doesnt already exist)
echo "Making directory structure..."
mkdir -p $XEN_PATH/DEBIAN
mkdir -p $XEN_PATH/usr/bin
mkdir -p $XEN_PATH/usr/share/xentusdk/templates


# Copy the engine files into the directories
cp ../out/build/gcc/xentu $XEN_PATH/usr/bin
cp ../out/build/gcc/xentusdk $XEN_PATH/usr/bin
cp ../data/logo.png $XEN_PATH/usr/share/xentusdk


# Copy the examples into the build directory
cd "$INST_PATH/../data/examples/platformer/"; pwd; zip -r $XEN_PATH/usr/share/xentusdk/templates/platformer.zip *
cd "$INST_PATH/../data/examples/pong/"; pwd; zip -r $XEN_PATH/usr/share/xentusdk/templates/pong.zip *
cd "$INST_PATH/../data/examples/space-hopper/"; pwd; zip -r $XEN_PATH/usr/share/xentusdk/templates/space-hopper.zip *
cd "$INST_PATH/../data/examples/top-down/"; pwd; zip -r $XEN_PATH/usr/share/xentusdk/templates/top-down.zip *
cd "$INST_PATH"


# Create the control file.
echo "Package: xentusdk
Version: $XEN_VERSION
Architecture: $XEN_ARCH
Maintainer: Kodaloid<kodaloid@xentu.net>
Depends: wget, libopengl0,libglew2.1,libglfw3,liblua5.3-0
Homepage: https://xentu.net
Description: A slim, fast and easy to learn framework for creating 2D games scripted in Lua.
" >> $XEN_PATH/DEBIAN/control


# Create the preinst.sh file.
echo "#!/bin/bash
# file name preinst
# Pre-install script for tera. This removes old versions o tera.
echo \"Looking for old versions of XentuSDK ...\"

if [ -f \"/usr/bin/xentusdk\" ];then
    sudo rm -f usr/bin/xentusdk
    echo "Removed old XentuSDK from /usr/bin ..."
fi

if [ -d \"/usr/share/xentusdk\" ];then
    sudo rm -rf /usr/share/xentusdk
    echo "Removed old XentuSDK from /usr/share ..."
fi" >> $XEN_PATH/DEBIAN/preinst.sh


# Set needed ownerships.
sudo chown root:root -R $XEN_PATH
sudo chmod 755 $XEN_PATH/usr/bin/xentu
sudo chmod 755 $XEN_PATH/usr/bin/xentusdk
sudo chmod 755 $XEN_PATH/DEBIAN/preinst.sh


# Build the debian package.
dpkg-deb --build $XEN_PATH


echo "---------------------------------"
echo "XentuSDK package prepared!"