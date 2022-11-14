# EasyFlash 3 BootImage Color mod

This is a fork of easyflash3-bootimage that has been color modded.
The original version can be found at https://github.com/easyflash-mirror/easyflash3-bootimage .

![alt text](https://github.com/AtomicRPM/easyflash3-bootimage-colormod/blob/main/EF3main.jpg?raw=true)

![alt text](https://github.com/AtomicRPM/easyflash3-bootimage-colormod/blob/main/EPabout.jpg?raw=true)

![alt text](https://github.com/AtomicRPM/easyflash3-bootimage-colormod/blob/main/EPflash.jpg?raw=true)

![alt text](https://github.com/AtomicRPM/easyflash3-bootimage-colormod/blob/main/EPname.jpg?raw=true)

![alt text](https://github.com/AtomicRPM/easyflash3-bootimage-colormod/blob/main/EPsuccess.jpg?raw=true)

## Introduction

This is EasyProg, a Commodore 64/128 tool to write data to the EasyFlash and
EasyFlash 3 cartridges.        Cancel changes


## Website, Repository and Issue Tracker

EasyFlash website: https://skoe.de/easyflash/

Official repository and issue tracker:
https://gitlab.com/easyflash/easyflash3-bootimage/

Other EasyFlash related repositories:
https://gitlab.com/easyflash/

Source repository mirror:
https://github.com/easyflash-mirror/easyflash3-bootimage/

## License

(C) Thomas 'skoe' Giesel

Refer to [LICENSE.md](./LICENSE.md).

## Dependencies

TODO: Document this

sudo apt install make acme

## How to update the EasyFlash3 menu

This chapter describes how to update the EasyFlash3 software. The EasyFlash3
software consists of the EF3 menu and built it EasyProg. It is saved in EF3
cartridge's System Area (EasyFlash Slot 0). The software can be updated
without erasing any existing EasyFlash, KERNAL or freezer cartridge slots.

There are two images you can use to update the menu:

### ef3-menu.crt

This image contains:

- The menu
- EasyProg

This is the file you should use to update an existing cartridge which has
already software on it.

### ef3-init.crt

This image contains:

- The menu
- EasyProg
- Example KERNALs
- A directory which contains the KERNALs but is empty otherwise

This is the file you should use to initialize a new EasyFlash 3. If your
System Area got erased or overwritten, you can use this image to repair it.

## How to write a Menu Image to Flash

Use EasyProg to write ef3-menu.crt or ef3-init.crt. Select Slot 0, confirm the
warning message and select the file. See Write a CRT to EF/EF3 with EasyProg.

In case the EF3 menu is not accessible (e.g. the System Area has been erased),
EasyProg can be loaded from a disk drive.
