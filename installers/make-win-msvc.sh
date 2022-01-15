#!/bin/bash


echo "XentuSDK Windows (MSVC) packaging script."
echo "-----------------------------------------"


# Setup some constants.
INST_PATH="$PWD"
XEN_VERSION=`cat ../version.txt`
XEN_ARCH="all"
XEN_BASE="${INST_PATH}/../out/win-msvc"


# remove the old build folder.
sudo rm -rf $XEN_BASE


# Create the directory structure (if it doesnt already exist)
echo "Making directory structure..."
mkdir -p $XEN_BASE/templates


# Copy the engine files into the directories
cp ../out/build/gcc/xentu $XEN_BASE/
cp ../out/build/gcc/xentusdk $XEN_BASE/
cp ../out/build/msvc/glew32.dll $XEN_BASE/
cp ../out/build/msvc/glfw3.dll $XEN_BASE/
cp ../data/logo.png $XEN_BASE/
cp ../LICENSE.txt $XEN_BASE/


# Copy the examples into the build directory
cd "$INST_PATH/../data/examples/platformer/"; zip -r $XEN_BASE/templates/platformer.zip *
cd "$INST_PATH/../data/examples/pong/"; zip -r $XEN_BASE/templates/pong.zip *
cd "$INST_PATH/../data/examples/space-hopper/"; zip -r $XEN_BASE/templates/space-hopper.zip *
cd "$INST_PATH/../data/examples/top-down/"; zip -r $XEN_BASE/templates/top-down.zip *
cd "$INST_PATH"


echo "---------------------------------"
echo "XentuSDK package prepared!"