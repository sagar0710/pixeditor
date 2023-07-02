# Pixel Editor

This is a bitmap editor available for Linux, Windows and Mac. You can:

- Edit images directly
- Use layers
- Create seamless 2D tilesets
- Preview sprite sheet animations

This was built for pixel art, and its goal is to simplify the creation of sprites and tilesets by offering a user-friendly, customisable interface.

<!--
    Note: generate the table of contents for each file with the following commands:

    cd ~/dev/slate
    doctoc . --github --title '# Contents'
-->

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
# Contents

  - [Screenshots](#screenshots)
  - [Downloads](#downloads)
  - [Installing](#installing)
  - [Documentation](#documentation)
  - [Discussion](#discussion)
  - [Donations](#donations)
- [Building From Source](#building-from-source)
  - [Dependencies](#dependencies)
  - [Cloning](#cloning)
  - [Building](#building)
    - [Qt Creator](#qt-creator)
    - [Command Line](#command-line)
      - [Qbs](#qbs)
      - [CMake](#cmake)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

### Screenshots

![Screenshot on Linux](https://github.com/sagar0710/pixeditor/blob/main/doc/images/slate-v0.1.0-tileset-linux.png "Screenshot on Linux")

![Screenshot on Mac](https://github.com/sagar0710/pixeditor/blob/main/doc/images/slate-v0.4.0-layers-mac.png "Screenshot on Mac")

![Screenshot on Mac](https://github.com/sagar0710/pixeditor/blob/main/doc/images/slate-v0.4.0-animation-mac.png "Screenshot on Mac")

### Installing ###

All builds are portable, meaning that no installation is necessary. On Windows, however, it may be necessary to run the vc_redist.x64.exe file that comes with the download in order to install certain Microsoft C++ libraries if you've never installed any applications that require this prior to running Slate.

## Building From Source ##

### Dependencies ###

* Qt 6.5

Note that the dependencies above are for the current branch that you are viewing, and GitHub shows the master branch by default.

#### Qt Creator ####

It's recommended to build using Qt Creator for convenience.

Open [slate.qbs](https://github.com/sagar0710/pixeditor/blob/release/slate.qbs) or [CMakeLists.txt](https://github.com/sagar0710/pixeditor/blob/master/CMakeLists.txt) in Qt Creator, configure the project using a kit with a Qt version that meets the requirement listed in the [Dependencies](#dependencies) section, and then build it.

#### Command Line ####

##### Qbs

First, follow Qbs' [setup guide](http://doc.qt.io/qbs/setup.html). Once you've set it up, choose one of the following approaches.

In-source builds:

    cd /path/to/source-dir
    qbs

Shadow builds:

    mkdir pixeditor-build
    cd pixeditor-build
    qbs /path/to/source-dir/slate.qbs

For more information about building Qbs applications, see [this page](http://doc.qt.io/qbs/building-applications.html).

##### CMake

In-source builds:

    cd /path/to/source-dir
    cmake .
    cmake --build .

Shadow builds:

    mkdir pixeditor-build
    cd pixeditor-build
    cmake /path/to/source-dir
    cmake --build .

To run all test cases:

    cd pixeditor-build
    ctest

---

List of assets used in the screenshots:

- https://opengameart.org/content/lpc-tile-atlas
- https://opengameart.org/content/pixel-explosion-12-frames

OS icons taken from [font-os](https://github.com/JeyKeu/font-os).
