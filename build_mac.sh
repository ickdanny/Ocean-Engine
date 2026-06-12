#!/bin/bash

# Clean
rm -rf build
rm -rf EU06_OYCN
rm -f EU06_OYCN.zip

# Make dirs
mkdir build
mkdir EU06_OYCN

cp -r res ./build/res

# Use Cmake to build
cd build
cmake --version
cmake ..
cmake --build . --parallel --config Release
cd ..

# make app in dir
mkdir -p EU06_OYCN/EU06_OYCN.app/Contents/MacOS
mkdir -p EU06_OYCN/EU06_OYCN.app/Contents/Resources

# put actual binary inouter dir; we use a wrapper script
cp build/OceanEngine EU06_OYCN/executable_mac
chmod +x EU06_OYCN/executable_mac

cp Info.plist EU06_OYCN/EU06_OYCN.app/Contents/
cp packaging/icon.icns EU06_OYCN/EU06_OYCN.app/Contents/Resources/

# copy wrapper script
cp launcher_mac.sh EU06_OYCN/EU06_OYCN.app/Contents/MacOS
chmod +x EU06_OYCN/EU06_OYCN.app/Contents/MacOS/launcher_mac.sh

# Copy everything else
# Cannot copy the base res dir because build process
# involves copying over scripts
cp -r build/res EU06_OYCN/res
cp -r packaging/* EU06_OYCN/