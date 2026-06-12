# On Yellow Crystal Nights v1.1

On Yellow Crystal Nights (OYCN) is the sixth entry in the Eucatastrophe series of vertical bullet hell shoot-em-up games. This repository includes all resources needed to compile and build the game locally. The underlying engine is called the Ocean Engine, and it is written using C and uses archetype-based ECS. The game can be built and ran on Windows, Mac, and Linux.

Instructions on how to play the game can be found in `packaging/README.txt`.

## Requirements

Building OYCN will require GLFW and GLEW. These will be downloaded by Cmake during the build process.

### Windows

To run the game, the following minimum requirements are stated:
- Windows 10
- Direct3D 11
- 2GB RAM

To build the game, the following are used:
- Powershell
- Cmake 3.30.2
- MSVC 19.40.33813
- Windows SDK 10.x

### Mac

To run the game, the following minimum requirements are stated:
- macOS 14
- Apple silicon

To build the game, the following are used:
- Cmake 3.27.4
- Clang 15.x

### Linux

To run the game, the following minimum requirements are stated:
- X11
- 2GB RAM
- (Honestly, just give it a try)

To build the game, the following are used:
- Cmake 4.3.2 (earlier is likely fine)
- GCC 15.2.0

## Installation

### Windows

Install OYCN as follows on Windows using Powershell:

```
git clone https://www.github.com/ickdanny/todo
cd todo
.\build.ps1
```

### Mac

Install OYCN as follows on Mac:

```
git clone https://www.github.com/ickdanny/todo
cd todo
./build_mac.sh
```

### Linux

Install OYCN as follows on Linux:
```
git clone https://www.github.com/ickdanny/todo
cd todo
./build_linux.sh
```

## Usage

The build script should create the directory `EU06_OYCN`. Navigate to the directory `EU06_OYCN` to run the game.

### Windows

To launch the game on Windows, run the `EU06_OYCN.exe` executable.

### Mac

On Mac, run the app called `EU06_OYCN`.

### Linux

On Linux, run the executable titled `EU06_OYCN`. Because Linux does not offer a standard MIDI synth, users are recommended to have a separate Timidity process running at the same time as the game. To run Timidity as an ALSA server, try the following:

```
timidity -iA
```

## Modding

Users may easily mod OYCN without rebuilding the project by altering the various scripts comprising the game. In the `EU06_OYCN` directory, the scripts may be found under the `res/script` directory. The scripts are written in Unknown, a custom interpreted language which does not depend on the user's platform. There is currently no documentation for Unknown.
