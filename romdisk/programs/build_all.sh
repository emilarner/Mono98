#!/usr/bin/bash

printf "\x50\x5C\xDE" > HEADER

gcc -m32 -ffreestanding -nostdlib -c test.c -o test.o
#ld -m elf_i386 -o test.com -L../../libc/ -lc test.o -T linker.ld -e __entry
#ld -m elf_i386 -o test.com -L../../libc/ -lc test.o -T linker.ld
# make sure the static library comes after the main object file. 
ld -m elf_i386 -o test.com test.o ../../libc/libc.a -T linker.ld
cat HEADER test.com >> TEST
cp TEST ../contents/TESTC

# cleanup
rm TEST
rm test.o
rm test.com

nasm -f bin -o testasm test.s
cp testasm ../contents/TESTASM