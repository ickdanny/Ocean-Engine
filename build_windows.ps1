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
 
# # Copy everything into dir
# cp build\Release\ProcessEngine.exe EU05_POTUK\EU05_POTUK.exe
# cp build\*.cso EU05_POTUK\
# # Cannot copy the base res dir because build process involves copying over scripts
# cp -r build\Release\res EU05_POTUK\res
# cp -r packaging\* EU05_POTUK\
# 
# # Zip it up
# Compress-Archive -Path EU05_POTUK -DestinationPath EU05_POTUK.zip