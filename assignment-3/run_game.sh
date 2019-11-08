#!/bin/bash

echo Clean game:
ptxdist clean game

echo Compile game:
ptxdist compile game

echo Target game:
ptxdist targetinstall game

echo Make image:
ptxdist image root.romfs

echo Prog to mcu:
ptxdist test flash-rootfs

echo Start miniterm. Press enter and write "game":

miniterm.py -b 115200 -p /dev/ttyUSB0
