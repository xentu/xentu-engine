#!/bin/bash


echo "XentuSDK debian packaging script."
echo "---------------------------------"


# remove the old build folder.
sudo rm -rf ../out/deb


# Setup some constants.
XEN_VERSION=`cat ../version.txt`
XEN_ARCH="all"
XEN_BASE="../out/deb"
XEN_PATH="${XEN_BASE}/xentusdk_${XEN_VERSION}_${XEN_ARCH}"


# Create the directory structure (if it doesnt already exist)
echo "Making directory structure..."
mkdir -p $XEN_PATH/DEBIAN
mkdir -p $XEN_PATH/usr/bin
mkdir -p $XEN_PATH/usr/share/xentusdk


# Copy the engine files into the directories
cp ../out/build/gcc/xentu $XEN_PATH/usr/bin
cp ../out/build/gcc/xentusdk $XEN_PATH/usr/bin
cp ../data/logo.png $XEN_PATH/usr/share/xentusdk


# Create the control file.
echo -e "Package: xentusdk
Version: $XEN_VERSION
Architecture: $XEN_ARCH
Maintainer: Kodaloid<kodaloid@xentu.net>
Depends: wget
Homepage: https://xentu.net
Description: A slim, fast and easy to learn framework for creating 2D games scripted in Lua.
" >> $XEN_PATH/DEBIAN/control


# Create the preinst.sh file.
echo -e "#!/bin/bash
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