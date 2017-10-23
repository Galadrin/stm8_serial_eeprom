# We're assuming POSIX conformance
.POSIX:

PNAME=lt200_dongle

CC = sdcc

# In case you ever want a different name for the main source file
MAINSRC = src/$(PNAME).c

# These are the sources that must be compiled to .rel files:
EXTRASRCS = \
    src/clock.c		\
    src/crc16.c		\
    src/eeprom.c	\
    src/gpio.c    

# The list of .rel files can be derived from the list of their source files
RELS = $(EXTRASRCS:.c=.rel)

INCLUDES = -Iinc
CFLAGS   = -mstm8 --debug
LIBS     = -lstm8

clean:
	rm -rf *.asm *.adb *.cdb *.lk *.lst *.map *.rel *.rst *.sym
	rm -rf **/*.asm **/*.adb **/*.cdb **/*.lk **/*.lst **/*.map **/*.rel **/*.rst **/*.sym

# This just provides the conventional target name "all"; it is optional
# Note: I assume you set PNAME via some means not exhibited in your original file
all: $(PNAME)

# How to build the overall program
$(PNAME): $(MAINSRC) $(RELS)
	$(CC) $(INCLUDES) $(CFLAGS) $(MAINSRC) $(RELS) $(LIBS)

# How to build any .rel file from its corresponding .c file
# GNU would have you use a pattern rule for this, but that's GNU-specific
.c.rel:
	$(CC) -o $(dir $<) -c $(INCLUDES) $(CFLAGS) $<

# Suffixes appearing in suffix rules we care about.
# Necessary because .rel is not one of the standard suffixes.
.SUFFIXES: .c .rel
