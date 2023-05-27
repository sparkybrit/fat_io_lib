#TOOLCHAIN = m68k-linux-gnu-
TOOLCHAIN = ~/x-tools/m68k-unknown-elf/bin/m68k-unknown-elf-
CC = $(TOOLCHAIN)gcc
AS = $(TOOLCHAIN)as
LD = $(TOOLCHAIN)ld
OBJDUMP = $(TOOLCHAIN)objdump
OBJCOPY = $(TOOLCHAIN)objcopy
NM = $(TOOLCHAIN)nm

ASFLAGS = -m68030 -g --bitwise-or
CFLAGS = -ffreestanding -m68030 -g 
LDFLAGS = -static -T fat-io-lib.ld -Map=fat-io-lib.map

OBJS = cpu.o crt0.o fat-io-lib.o uart-68681.o spi.o spi-68681.o fat_filelib.o fat_string.o fat_access.o sd.o \
	timer.o fat_cache.o fat_format.o fat_misc.o fat_table.o	fat_write.o syscalls.o

LIBS = ~/x-tools/m68k-unknown-elf/m68k-unknown-elf/lib/libc.a

all: fat-io-lib.bin

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

fat-io-lib.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(OBJDUMP) -S $@ >fat-io-lib.lst
	$(NM) -n $@ >fat-io-lib.nm

fat-io-lib.bin: fat-io-lib.elf
	$(OBJCOPY) -O binary $^ $@

clean: 
	rm *.bin *.elf *.o *.lst *.nm *.map *.hex *.hexdump