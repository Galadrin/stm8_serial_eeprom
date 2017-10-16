DCC=sdcc
SDLD=sdld

OBJECTS=main.ihx

.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)

flash: $(OBJECT).ihx
	stm8flash -cstlink -pstm8l150 -w build/$(OBJECT).ihx

main.ihx:
	sdcc -o build -mstm8 --std-c99 main.c
	
%.ihx : %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) $<
