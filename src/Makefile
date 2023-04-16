CC     = m68k-linux-gnu-gcc
CFLAGS = -s -g
CFLAGS = -ffreestanding -s -g -m68030 -nostdlib -nostartfiles
OBJS	= main.o fat_access.o fat_cache.o fat_filelib.o fat_format.o fat_misc.o \
	fat_string.o fat_table.o fat_write.o sd.o spi.o timer.o
#DEPFLAGS      = -MT $$@ -MMD -MP -MF $(call src2dep,$(1))

all: main.elf

main.elf: $(OBJS)
	$(CC) -o $@ $^

%.o: %.c 
	$(CC) -c $(CFLAGS) -o $@ $^

clean: 
	rm $(OBJS) main.elf
