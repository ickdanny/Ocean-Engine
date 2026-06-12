
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

# Copy everything else
# Cannot copy the base res dir because build process
# involves copying over scripts
cp build/OceanEngine EU06_OYCN/EU06_OYCN
cp -r build/res EU06_OYCN/res
cp -r packaging/* EU06_OYCN/