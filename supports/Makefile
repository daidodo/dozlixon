
#########################
# Makefile for DozLixon #
#########################


# Entry point of DozLixon
# It must be as same as 'KernelEntryPointPhyAddr' in load.inc!!!
ENTRYPOINT	= 0x30400

# Offset of entry point in kernel file
# It depends on ENTRYPOINT
ENTRYOFFSET	=   0x400


# Programs, flags, etc.
ASM		= nasm
DASM		= ndisasm
CC		= gcc
LD		= ld
ASMBFLAGS	= -I boot/include
ASMKFLAGS	= -I include -f elf
CFLAGS		= -I include -c #-fno-builtin
LDFLAGS		= -s -Ttext $(ENTRYPOINT)
DASMFLAGS	= -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)

# This Program
DOZLIXONBOOT	= boot/boot.bin boot/loader.bin
DOZLIXONKERNEL	= kernel.bin
OBJS		= kernel/kernel.o lib/klib.o lib/klibc.o kernel/syscall.o kernel/start.o kernel/main.o kernel/system.o\
			kernel/i8259.o kernel/global.o kernel/protect.o kernel/proc.o lib/string.o kernel/keyboard.o \
			kernel/clock.o kernel/tty.o kernel/console.o kernel/usrlibrary.o
DASMOUTPUT	= kernel.bin.asm

# All Phony Targets
.PHONY : everything final image clean realclean disasm all buildimg

# Default starting position
everything : $(DOZLIXONBOOT) $(DOZLIXONKERNEL)

all : realclean everything

final : all clean

image : final buildimg

clean :
	rm -f $(OBJS)

realclean :
	rm -f $(OBJS) $(DOZLIXONBOOT) $(DOZLIXONKERNEL)

disasm :
	$(DASM) $(DASMFLAGS) $(DOZLIXONKERNEL) > $(DASMOUTPUT)

# Write "boot.bin" & "loader.bin" into floppy image "bootsector.flp"
# We assume that "bootsector.flp" exists in current folder
buildimg :
	mount bootsector.flp /mnt/floppy -o loop
	cp -f boot/loader.bin /mnt/floppy/
	cp -f kernel.bin /mnt/floppy
	umount  /mnt/floppy

boot/boot.bin : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

boot/loader.bin : boot/loader.asm boot/include/load.inc boot/include/fat12hdr.inc boot/include/pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(DOZLIXONKERNEL) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(DOZLIXONKERNEL) $(OBJS)

kernel/kernel.o : kernel/kernel.asm include/sconst.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<
	
kernel/syscall.o : kernel/syscall.asm include/sconst.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<
	
kernel/start.o: kernel/start.cpp include/type.h include/protect.h include/const.h include/proc.h \
			include/global.h include/proto.h include/kernellibrary.h  include/system.h\
			include/keyboard.h include/clock.h include/BufferTemplate.h include/usrlibrary.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/main.o: kernel/main.cpp include/type.h include/proto.h include/kernellibrary.h include/usrlibrary.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/i8259.o: kernel/i8259.cpp include/type.h include/protect.h include/const.h include/proc.h \
			include/global.h include/proto.h include/kernellibrary.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/global.o: kernel/global.cpp include/type.h include/protect.h include/const.h include/proc.h \
			include/global.h include/proto.h include/kernellibrary.h include/system.h include/tty.h\
			include/console.h include/BufferTemplate.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/protect.o: kernel/protect.cpp include/type.h include/protect.h include/const.h include/proc.h \
			include/global.h include/proto.h include/kernellibrary.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/proc.o: kernel/proc.cpp include/type.h include/protect.h include/const.h include/proc.h \
			include/global.h include/proto.h include/kernellibrary.h include/usrlibrary.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/system.o: kernel/system.cpp include/type.h include/protect.h include/const.h\
			include/proc.h include/global.h include/keyboard.h include/tty.h include/console.h\
			include/system.h include/BufferTemplate.h
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/keyboard.o: kernel/keyboard.cpp include/type.h include/protect.h include/const.h \
			include/proto.h include/kernellibrary.h include/keyboard.h include/keymap.h include/tty.h\
			include/console.h include/BufferTemplate.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/clock.o: kernel/clock.cpp include/type.h include/protect.h include/const.h \
			include/proc.h include/global.h include/proto.h include/kernellibrary.h\
			include/system.h include/clock.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/console.o: kernel/console.cpp include/type.h include/protect.h include/const.h include/proc.h \
			include/global.h include/proto.h include/kernellibrary.h include/console.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/tty.o: kernel/tty.cpp include/type.h include/tty.h include/console.h include/const.h include/proto.h\
			include/kernellibrary.h include/keyboard.h include/protect.h include/proc.h include/BufferTemplate.h\
			include/global.h 
	$(CC) $(CFLAGS) -o $@ $<

kernel/usrlibrary.o: kernel/usrlibrary.cpp include/type.h include/proto.h include/kernellibrary.h include/usrlibrary.h
	$(CC) $(CFLAGS) -o $@ $<

lib/klibc.o: lib/klib.cpp include/type.h include/protect.h include/const.h include/proc.h \
			include/global.h include/proto.h include/kernellibrary.h include/usrlibrary.h
	$(CC) $(CFLAGS) -o $@ $<

lib/klib.o : lib/klib.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

lib/string.o : lib/string.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<
