# We're assuming POSIX conformance
.POSIX:

.PHONY: all clean flash

PNAME=lt200_dongle

CC = sdcc

# In case you ever want a different name for the main source file
MAINSRC = src/$(PNAME).c

# These are the sources that must be compiled to .rel files:
EXTRASRCS = \
    src/crc16.c		\
    src/eeprom.c	\
    stm8_hal/stm8s_clk.c \
    stm8_hal/stm8s_gpio.c \
    stm8_hal/stm8s_tim4.c \
    stm8_hal/stm8s_uart1.c \
    stm8_hal/stm8s_flash.c

# The list of .rel files can be derived from the list of their source files
RELS = $(EXTRASRCS:.c=.rel)

INCLUDES = -Iinc -Istm8_hal
CFLAGS = --std-sdcc99 -DSTM8S003= -DHSE_VALUE=\(\(u32\)10000000\) -mstm8 --out-fmt-ihx --opt-code-size
LIBS = -lstm8

.PHONY: all clean flash_leroydp flash_discovery

# This just provides the conventional target name "all"; it is optional
all: $(PNAME)

clean:
	rm -rf *.asm *.adb *.cdb *.lk *.lst *.map *.rel *.rst *.sym
	rm -rf **/*.asm **/*.adb **/*.cdb **/*.lk **/*.lst **/*.map **/*.rel **/*.rst **/*.sym

flash_discovery: $(PNAME)
	@stm8flash -cstlink -pstm8s003k3 -w $(PNAME).ihx

flash_leroydp: $(PNAME)
	@stm8flash -cstlink -pstm8s003f3 -w $(PNAME).ihx

# How to build the overall program
$(PNAME): $(MAINSRC) $(RELS)
	@$(CC) $(INCLUDES) $(CFLAGS) $(MAINSRC) $(RELS) $(LIBS)

# How to build any .rel file from its corresponding .c file
# GNU would have you use a pattern rule for this, but that's GNU-specific
.c.rel:
	$(CC) -o $(dir $<) -c $(INCLUDES) $(CFLAGS) $<

# Suffixes appearing in suffix rules we care about.
# Necessary because .rel is not one of the standard suffixes.
.SUFFIXES: .c .rel
