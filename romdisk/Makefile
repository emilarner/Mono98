CC=gcc
CFLAGS=-O2

packer: packer.c ../src/romdisk.c ../src/romdisk.h
	$(CC) $(CFLAGS) -o packer packer.c ../src/romdisk.c

clean:
	rm packer