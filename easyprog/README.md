# EasyProg

This is a fork of easyflash3-bootimage that has been color modded. The original version can be found at https://github.com/easyflash-mirror/easyflash3-bootimage .  FC3 capabilities derived from https://github.com/adrianglz64/easyflash3-fc3 .

## Introduction

This is EasyProg, a Commodore 64/128 tool to write data to the EasyFlash and
EasyFlash 3 cartridges.

The official repository is located at https://gitlab.com/easyflash/easyflash3-core/.
Look at https://gitlab.com/easyflash/ for other EasyFlash related repositories.

Older versions and the history can be found at https://bitbucket.org/skoe/easyflash/.

## License

(C) Thomas 'skoe' Giesel

Refer to [LICENSE.md](./LICENSE.md).

## Dependencies

EasyProg depends from libEF3usb (https://gitlab.com/easyflash/libef3usb) to read
data and EAPI (https://gitlab.com/easyflash/eapi). These two libraries are
referenced as git submodules. To clone this project be sure to use:

`git clone --recurse-submodules https://gitlab.com/easyflash/easyprog.git`

It is recommended to compile this software on Linux (testet on Ubuntu 20.04.LTS), but it may work on other platforms too.

Tools needed:

- GNU make
- cc65 2.16 or later
- Exomizer 3.0.0 or later

## Installation

Just use:

`make`

And copy the resulting `easyprog` (uncompressed) or `easyprog.prg` (compressed) to your Commodore 64/128.
