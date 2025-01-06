# Compiler and tools
CC=avr-gcc
OBJCOPY=avr-objcopy
AVRDUDE=avrdude

# MCU and programmer settings
MCU=attiny13
PORT=/dev/ttyACM0
BAUD=19200
PROGRAMMER=avrisp

# File names
SRC=main.c
ELF=main.elf
HEX=main.hex

# Compiler flags
CFLAGS=-Wall -Os -mmcu=$(MCU)

# Targets
all: $(HEX)

$(ELF): $(SRC)
	$(CC) $(CFLAGS) -o $(ELF) $(SRC)

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $(ELF) $(HEX)

upload: $(HEX)
	$(AVRDUDE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -p $(MCU) -U flash:w:$(HEX)

clean:
	rm -f $(ELF) $(HEX)

# Phony targets
.PHONY: all upload clean
