$ErrorActionPreference = "Stop"

# # Clean
# Remove-Item -Recurse -Force build,EU06_OYCN,EU06_OYCN.zip -ErrorAction SilentlyContinue
# 
# # Make dirs
# mkdir build
# mkdir EU06_OYCN
# 
# cp -r res .\build\res

# Use Cmake to build
cd build
cmake --version
cmake ..
cmake --build . --config Release
cd ..
 
# Copy everything into dir
cp build\Release\OceanEngine.exe EU06_OYCN\EU06_OYCN.exe
# Cannot copy the base res dir because build process involves copying over scripts
cp -r build\res EU06_OYCN\res
cp -r packaging\* EU06_OYCN\

# Zip it up
# Compress-Archive -Path EU06_OYCN -DestinationPath EU06_OYCN.zip