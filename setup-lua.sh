#!/bin/sh

mkdir ./lib/lua
cd ./lib/lua

tar zxf lua-5.4.4.tar.gz
cd lua-5.4.4

make linux test
sudo make install