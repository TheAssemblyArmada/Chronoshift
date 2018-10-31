# Chronoshift

An Open Source Re-Implementation of Command and Conquer: Red Alert

This project is a bottom-up rewrite of Command and Conquer: Red Alert, a real time
strategy game.
This project uses the original binary to provide functions that have not been implemented yet.
The intention is to allow the fixing of bugs, implementation of new
features and porting to platforms unsupported by the original.

## Building

In order to generate a working binary, currently you must build the project using
a version of the Watcom compiler. Building using MinGW-w64 and Microsoft Visual Studio
is also tested periodically, but because of differences in the compiler ABI, dll's that it generates
will not work correctly with the original game binary. Once the reimplementation is complete
it should be possible to build with any C++11 compiler.
 
In order to build, first download and install [Open Watcom](http://openwatcom.org/download.php) and [CMake](https://cmake.org/download/) for windows.
Make sure you add CMake to at least your users path when prompted to during the install.
Next, open a command prompt window and run the following commands:

* `C:\WATCOM\owsetenv.bat` Sets the environment variables for OpenWatcom, assumes default install directory.
* `git clone https://github.com/TheAssemblyArmada/Chronoshift.git` Clone the repository.
* `cd Chronoshift` Change to the cloned repository.
* `mkdir build` Make a build directory.
* `cd build` Change to the build directory.
* `cmake -G "Watcom WMake" -DCMAKE_BUILD_TYPE="Release" ..` Configures the watcom build environment.
* `wmake` Compiles both the launcher and the Chronoshift .dll file with Open Watcom.

Chronoshift will only work against the [english 3.03 beta patch](http://nyerguds.arsaneus-design.com/cncstuff/patches/ra1/ra303eng.zip).
Simply copy the launcher and .dll into your Red Alert directory and run the launcher.

### Linux and macOS

Native support for Linux and macOS is planned for the future, but because of how
the project is developed, a native binary will not be possible for some time.
In the mean time, using Wine on Linux and macOS, should be possible but
is currently untested.

## Licence

The source code provided in this repository for
Chronoshift is licenced under the [GNU General Public License version 2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html)
or later with an exception allowing the resulting code to be linked against a closed source
binary which will be in place until Chronoshift no longer relies on the original.

## Other Open-Game Projects

Below is a list of similar projects and their respective original games:

 * [OpenRCT2](https://github.com/OpenRCT2/OpenRCT2) - RollerCoaster Tycoon 2
 * [OpenTTD](https://www.openttd.org/) - Transport Tycoon Deluxe
 * [OpenMC2](https://github.com/LRFLEW/OpenMC2) - Midnight Club 2
 * [OpenDUNE](https://github.com/OpenDUNE/OpenDUNE) - Dune 2
 * [OpenFodder](https://github.com/OpenFodder/openfodder) - Cannon Fodder
 * [Devilution](https://github.com/diasurgical/devilution) - Diablo
 * [Thyme](https://github.com/TheAssemblyArmada/Thyme) - C&C Generals: Zero Hour

There is also the [Wikipedia page for open source games](https://en.wikipedia.org/wiki/List_of_open-source_video_games).

## Contributing

If you are interested in contributing to Chronoshift, you will need some knowledge of C++ as a minimum requirement. Join the developer chat listed below for more information on what else you will need such as the idc file for the current map of the binary.

You can also check the wiki for more information.

## Contact

You can discuss development of this project on our [Discord server](https://discord.gg/UnWK2Tw).