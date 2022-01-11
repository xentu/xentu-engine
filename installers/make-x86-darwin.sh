#!/bin/bash


echo "XentuSDK MacOS packaging script."
echo "--------------------------------"


# Setup some constants.
XEN_VERSION=`cat ../version.txt`
XEN_ARCH="all"
XEN_BASE="../out/darwin"
XEN_PATH="${XEN_BASE}/xentusdk_${XEN_VERSION}_${XEN_ARCH}"


# remove the old build folder.
sudo rm -rf $XEN_BASE


# Create the directory structure (if it doesnt already exist)
echo "Making directory structure..."
mkdir -p $XEN_PATH/usr/bin
mkdir -p $XEN_PATH/usr/share/xentusdk


# Copy the engine files into the directories
cp ../out/build/clang/xentu $XEN_PATH/usr/bin
cp ../out/build/clang/xentusdk $XEN_PATH/usr/bin
cp ../data/logo.png $XEN_PATH/usr/share/xentusdk


# Create control scripts & set ownerships.

# Build the package.


echo "---------------------------------"
echo "XentuSDK package prepared!"