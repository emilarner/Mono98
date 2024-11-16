CC=gcc
CFLAGS=-m32 -nostdlib -ffreestanding -O2
CFLAGS_BOOTLOADER=-m16 -O2 -nostdlib -ffreestanding
OBJS=objs/entry.o objs/kernel.o objs/terminal.o objs/graphics.o objs/stdlib.o objs/string.o objs/interrupts.o objs/keyboard.o objs/audio.o objs/romdisk.o objs/process.o
CFLAGS += $(EXTS)
CFLAGS_BOOTLOADER += $(EXTS)

ROMDISK=romdisk/contents/HELLO romdisk/contents/BACKGROUND

AS=nasm
AS_FLAGS=
AS_FLAGS += $(EXTS)

# final kernel floppy image
kernel.img: bootloader/bootloader.bin kernel.bin
	cat bootloader/bootloader.bin kernel.bin > kernel.img
	utils/pad kernel.img -1

# bootloader 
bootloader/bootloader.bin: bootloader/bootloader.o bootloader/load.o bootloader/bootsector.s
	$(AS) $(AS_FLAGS) -f bin bootloader/bootsector.s -o bootloader/part1.bin
	ld -m elf_i386 -o bootloader/part2.bin bootloader/load.o bootloader/bootloader.o -T bootloader/boot_linker.ld -v
	utils/pad bootloader/part2.bin 4096
	cat bootloader/part1.bin bootloader/part2.bin > bootloader/bootloader.bin

bootloader/bootloader.o: bootloader/bootloader.s
	$(AS) $(AS_FLAGS) -f elf32 bootloader/bootloader.s -o bootloader/bootloader.o

bootloader/load.o: bootloader/load.c
	$(CC) $(CFLAGS_BOOTLOADER) -c bootloader/load.c -o bootloader/load.o

kernel.bin: $(OBJS)
	ld -m elf_i386 -o kernel.bin $(OBJS) -T linker.ld -v

objs/entry.o: src/entry.s
	$(AS) $(AS_FLAGS) -f elf32 src/entry.s -o objs/entry.o

objs/kernel.o: src/kernel.c src/config.h
	$(CC) $(CFLAGS) -c src/kernel.c -o objs/kernel.o

objs/terminal.o: src/terminal.c src/terminal.h src/graphics.h
	$(CC) $(CFLAGS) -c src/terminal.c -o objs/terminal.o

objs/graphics.o: src/graphics.c src/graphics.h
	$(CC) $(CFLAGS) -c src/graphics.c -o objs/graphics.o

objs/string.o: src/std/string.c src/std/string.h
	$(CC) $(CFLAGS) -c src/std/string.c -o objs/string.o

objs/stdlib.o: src/std/stdlib.c src/std/stdlib.h
	$(CC) $(CFLAGS) -c src/std/stdlib.c -o objs/stdlib.o

objs/interrupts.o: src/interrupts.c src/interrupts.h src/asm.h
	$(CC) $(CFLAGS) -c src/interrupts.c -o objs/interrupts.o

objs/keyboard.o: src/keyboard.c src/keyboard.h
	$(CC) $(CFLAGS) -c src/keyboard.c -o objs/keyboard.o

objs/audio.o: src/audio.c src/audio.h
	$(CC) $(CFLAGS) -c src/audio.c -o objs/audio.o

objs/romdisk.o: src/romdisk.c src/romdisk.h
	$(CC) $(CFLAGS) -c src/romdisk.c -o objs/romdisk.o

objs/process.o: src/process.c src/process.h
	$(CC) $(CFLAGS) -c src/process.c -o objs/process.o

clean:
	rm -f objs/*.o
	rm -f utils/demo.jpeg
	rm -f utils/imgconvert
	rm -f resources/image.bin
	rm -f kernel.bin
	rm -f romdisk/contents/BACKGROUND
	rm -f romdisk/ROMDISK
	rm -f bootloader/bootloader.bin
	rm -f bootloader/part1.bin
	rm -f bootloader/part2.bin
	rm -f kernel.img
	rm -f bootloader/bootloader.o
	rm -f bootloader/load.o

	rm -f utils/demo.jpeg
	rm -f utils/imgconvert
	rm -f utils/pad


test: kernel.img
	qemu-system-i386 -cpu qemu32 -no-reboot -d int -hda kernel.img -m 48M -audiodev pa,id=speaker -machine pcspk-audiodev=speaker