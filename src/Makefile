TOOLCHAIN_PREFIX = m68k-linux-gnu-
CC     = $(TOOLCHAIN_PREFIX)gcc
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
NM = $(TOOLCHAIN_PREFIX)nm
CFLAGS = -ffreestanding -s -g
LDFLAGS = -nostdlib -nostartfiles
OBJS	= main.o fat_access.o fat_cache.o fat_filelib.o fat_format.o fat_misc.o \
	fat_string.o fat_table.o fat_write.o sd.o spi.o timer.o
#DEPFLAGS      = -MT $$@ -MMD -MP -MF $(call src2dep,$(1))

all: main.elf

main.elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	$(OBJDUMP) -S $@ >main.lst
	$(NM) $@ | sort

clean: 
	rm *.o *.elf
