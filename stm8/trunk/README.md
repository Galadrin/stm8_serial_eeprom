The dataplug is a little device plugged on the serial interface of a LT200.

building the software
##

Install the sdcc compiler and stm8flash tool (read the README in tools directory)

in the source directory, type 'make' command to obtain the .ihx file.
The .ihx file is an intelhex binary used by stm8flash to program stm8.


If you are using a stm8 discovery board, type the command 'make flash_discovery'.
else, if you are using a real dataplug, type the 'make flash_leroydp' command to flash the device.
