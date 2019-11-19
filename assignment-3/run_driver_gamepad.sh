
#!/bin/bash

echo Clean game:
ptxdist clean driver-gamepad

echo Compile game:
ptxdist compile driver-gamepad

echo Target game:
ptxdist targetinstall driver-gamepad

echo Make image:
ptxdist image root.romfs

echo Prog to mcu:
ptxdist test flash-rootfs

echo Start miniterm. Press enter and write "modprobe driver-gamepad":

miniterm.py -b 115200 -p /dev/ttyUSB0
