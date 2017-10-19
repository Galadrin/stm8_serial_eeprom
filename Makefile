DCC=sdcc
SDLD=sdld

OBJECTS=src/main.ihx

.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS)

flash: $(OBJECTS)
	stm8flash -cstlink -pstm8s003k3 -w cmake-build-debug/$(OBJECT)

main.ihx:
	sdcc -o cmake-build-debug -mstm8 --std-c99 main.c
	
%.ihx : %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) $<
