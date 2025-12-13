# Install dependencies
#
# sudo apt install gcc-avr binutils-avr avr-libc avrdude
#

DEVICE = attiny85
CLOCK = 1000000

SRCS = main.c ../Common/usi_uart.c ../Common/timing.c
OBJS = main.o usi_uart.o timing.o
TARGET = sram_single_write_continuous_read_attiny85

all: build

build: 
	avr-gcc -Wall -Os -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) -c $(SRCS)
	avr-gcc -Wall -Os -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) -o $(TARGET).elf $(OBJS)
	avr-objcopy -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex

	rm ${OBJS}
	rm $(TARGET).elf 

flash: build
	avrdude -c atmelice_isp -p $(DEVICE) -P usb -U flash:w:$(TARGET).hex:i

read:
	mkdir -p readbacks
	avrdude -c atmelice_isp -p $(DEVICE) -P usb -U flash:r:$(TARGET)_readback.hex:i
	mv $(TARGET)_readback.hex readbacks/$(TARGET)_$(NAME)_$$(date --iso=seconds).hex

clean: 
	rm ${OBJS}
	rmm main.elf
	rm $(TARGET).hex

format: 
	clang-format -i -style=Google ${SRCS}