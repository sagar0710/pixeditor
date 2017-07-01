# Slate - Pixel Art Editor

Slate is a bitmap editor available for Linux, Windows and Mac. Using slate, you can edit images directly, use layers and create seamless 2D tilesets for games. Slate was built for pixel art,  and its goal is to simplify the creation of sprites and tilesets by offering a user-friendly, customisable interface.  

![Slate Screenshot](https://github.com/mitchcurtis/slate/blob/master/slate.png "Slate")

### Downloads ###

See the [releases](https://github.com/mitchcurtis/slate/releases) page for the available downloads. Currently only Windows binaries are available, but other platforms (Linux, Mac) will show up eventually.

### Documentation ###
- [Getting Started](https://github.com/mitchcurtis/slate/blob/master/doc/getting-started.md)

### Notes ###
- This is a work in progress, but should already be usable. Feel free to report any bugs you find. :)
- The UI is currently designed for the [Material style](http://doc.qt.io/qt-5/qtquickcontrols2-material.html), and will probably look bad when run with others.

### Dependencies ###

* Qt >= 5.10
* https://github.com/mitchcurtis/qt-undo
* https://github.com/mitchcurtis/pickawinner

qt-undo and pickawinner can be cloned and built separately, but they are also available as submodules of Slate's repo, and can therefore be built automatically when
Slate is built, by first running the following commands:

    cd <slate-source-dir>
    git submodule init
    git submodule update

---

The tileset in the screenhot was taken from the following page:

http://opengameart.org/content/lpc-tile-atlas
