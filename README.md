# Mono98
![Screenshot of Mono98](https://i.ibb.co/MNXX1y3/image.webp)
## Overview

Mono98 is a hobbyist x86 operating system I wrote as a learning experience, as well as for the enjoyment of working very close to hardware in C and assembly (although, my reservations for assembly are quite different--I am still getting used to it). Its kernel is a monotasking kernel--that is, it can only run one process, program, or thread at a time, which presents many challenges and severe limitations for practicality, but simplifies the codebase a lot. It is also a protected mode kernel, which means that it can execute 32 bit code and technically could implement segment/paging based kernel memory protections. However, in the spirit of simplicity and allowing free reign, everything (including user programs) is ring 0. It is also specifically programmed as to work on real hardware, namely my Acer Aspire Windows 98 machine. With these design philosophies in mind, the name Mono98 comes from "Monotasking OS for my Windows 98 machine."

## Technical Details

There is more than mere kernel code in this repository, however. Namely, there is the bootloader, which is written to operate in 16 bit real mode--written in both (gnarly) assembly and simple C. The bootloader presents many challenges: virtual machines and modern BIOSes that emulate floppies may perform LBA to CHS conversions internally, but older real hardware may still expect you to use CHS addressing for their INT 13h BIOS routines to load the kernel into memory. Mono98, on my Windows 98 machine, is intended to be loaded via a 3.5'' floppy, so the bootloader is required to load the kernel into memory using the specific geometry of the average 3.5'' floppy when compiled for CHS loading--much easier said than done. As of now, my CHS loading compilation feature for the bootloader for 3.5'' floppies is not working--it is that hard (or I am just lazy).

Aside from the bootloader, there is a small and incomplete C standard library in the works, which must be statically linked with user programs written in C (or assembly, if you really want to push arguments to the stack for function calls, as the standard calling convention for every C function in this codebase is *cdecl*). There are also two simple user programs included: one written in assembly just to demonstrate that you can just barely develop software for this OS in assembly; and, a more complete one written in C which performs some demos of the C library (basic graphics using an inefficient and simple math.h implementation), demos of the program argument system, and demos of the pseudorandom number generator. 

Due to the fact that the kernel is monotasking, the environment for user programs is relatively simple and arguably extremely antiquated. User programs must ultimately be compiled or assembled into a flat binary, with their load address specified to be 0x333999 (completely and utterly arbitrary, which is the point) either by the linker or the assembler. Furthermore, the kernel considers a valid program to be one which contains a 3 byte magic of 0x50, 0x5C, 0xDE at its beginning. User programs/libc functions issue system calls not through software interrupts, but through directly calling a C function (again, using the *cdecl* calling convention) in the kernel address space. This is more optimal than using software interrupts, since it implements program blocking in a fairly straightforward manner, which is acceptable since only one program can execute at a time. The kernel stores the location of the system call handler at address 0x333335, which the libc or a user program can dereference. Additionally, the system call handler may directly set an errno variable at 0x333331 if there are any errors with the system call requested. 

Currently, there is no driver to read from the hard disk or floppy in the protected mode section of this operating system--however, there is a driver to read from something called a ROMDISK (a read-only memory disk). It is a file format based off of the CON file format I developed quite a long time ago (around when I was 13)--various files are put into a folder, then the ROMDISK packer packs it into a ROMDISK file according to the specifications of the format, which is embedded into the kernel via an *incbin* NASM directive. The ROMDISK file format maintains a table of where files are (using their offset in the file and their length) at the very beginning of the ROMDISK file, allowing for faster random accessing of files, as opposed to a file format such as *tar*, where one must continually scan for file headers and move past the data of irrelevant files. 

The kernel is very vaguely UNIX-like: everything is a file, including the keyboard and terminal. Programs can read from the terminal/keyboard in a line-buffered manner by issuing a system call to the SYS_READ system call for file descriptor 0 (stdin), and programs can write to the terminal by issuing SYS_WRITE to file descriptor 1 and 2 (stdout and stderr).  

The terminal driver is 7 bit ASCII compliant (supports \n and \b, though \n performs both a newline and a carriage return as if you were on a UNIX system), though with its own limitations and extensions--it is, however, absolutely not compliant with ANSI or a VT100-like terminal. Bytes 0xF0 through 0xFF that are sent to the terminal driver indicate a colour change to the terminal foreground--the lower 4 bits indicating any of the available default 16 VGA colours to change to. The code for the terminal driver was initially made to be very modular and expandable, due to initial hopes of making different TTYs available to switch to (as one would on something like Linux), but now everything is essentially hardcoded upon realization that Mono98 did not have to be of much grandeur.

The keyboard driver is very simple and straightforward, though it has its own eccentricities. It only supports PS/2 keyboards, which is not much of a problem as my Windows 98 machine does not even have USB ports and most modern BIOSes perform USB/onboard keyboard to PS/2 keyboard emulation internally. The default method of obtaining scancodes from the PS/2 keyboard is by attaching an interrupt handler to IRQ 1 (the keyboard) and then adding said scancodes (albeit with some modifications, the uint8_t scancode is casted to a uint16_t and then higher bits are set for modifiers such as shift) to a circular buffer. Alternatively, if you cannot get IRQ interrupts working for whatever reason (the case for me, with my Windows 98 machine), you may compile the keyboard driver to utilize polling instead--who cares if a hobbyist OS is inefficient, right?

The core graphics driver, if you can even call it a driver, is full of eccentricities and limitations as well. The graphics mode is set very early on in the bootloader, while we are still in real mode (because we need to issue a BIOS interrupt), such that we are in VGA compatible 320x200 256 colour mode, accessible for writing at address 0xA0000. A far more practical approach would have been to use 640x480 16 colour mode or even better, to use VESA modes to achieve 640x480 24 bit colour mode--though for a hobbyist operating system, this is unnecessary. 

The font system is a bit limiting as well (pun intended), but it works fine for Mono98: every character within the font system is a 32 bit integer bitmap, where the first 25 bits (left to right) describe some offset from where the character is placed in a 5x5 grid and whether a pixel should be placed (a 1 for yes, a 0 for no). The last 4 bits indicate the VGA colour that these pixels should be, naturally. Because the resolution of fonts is fixed at 5x5 and because the resolution of the screen is set to 320x200, certain characters or symbols are either severely limited or not feasible--therefore, the choice was made to not support lowercase letters, in addition to certain other symbols. 

Having to work within the confines of 256 colours and the standard VGA colour palette also presents challenges for displaying certain images, such as the background. One must perform a constrained colour quantization algorithm on 24 bit colour images to most optimally reduce them down to 256 colours aligned with the default palette. This sounds advanced and cumbersome, but it really boils down to finding what VGA palette index has the least Euclidean distance to a 24 bit colour pixel in an image you are converting. A program which performs this colour quantization is included in the *utils/* folder of this repository, and it is automatically compiled and ran on an image of your choice for the default OS background upon running the OS build script. Alternatively, one may find an optimal 256 colour palette for a given image using k-means clustering (or some other clustering algorithm) to set early on in the bootloader--however, I decided not to implement this due to laziness. 

Backgrounds are configured by editing the *do.sh* file in *utils/* and setting *IMAGE* to an image of your choice--the image, however, must be a JPEG and it must be exactly (no exceptions) 320x200 resolution. By default, the background is the background *resources/mono98.jpeg* file. 

## Building & Running

Due to how complicated this project is--and how many moving and disparate parts there are--it was difficult to justify using one Makefile to compile and handle everything. Therefore, to build everything (and not just the kernel) and stitch everything together with one command, one uses the build script *build.sh* located in the root of this repository. This will compile everything and additionally will run the final product *kernel.img* through qemu. However, there are some dependencies that must be met prior to compiling this operating system:

 - binutils
	 - Needed for ld
 - gcc
	 - Needed for the C compiler. I am unsure if this project will successfully compile and run using clang, or any other compiler for that matter. 
 - nasm
	 - Needed for the NASM assembler, needless to say. We do not use GNU as, nor an assembly syntax that is not Intel (why would you, anyway?)
 - libjpeg
	 - Needed for *imgconvert*, the constrained colour quantization program, needed for generating compatible backgrounds.
 - qemu
	 - Needed for testing/running the OS through a virtual machine

Once those are satisfied, you will likely want to pass certain build arguments to the build script, such as compilation features. Here are the different build arguments and what they do:

 - -DREAL_HARDWARE
	 - Use this if you want the OS image to be compiled for real hardware. This build option changes certain internal timer functions for sleeping to give adequate sleeping time for certain animations and processes. 
 - -DNO_BACKGROUND
	 - Use this if you do not want a background to be included into the kernel, nor the ROMDISK--this is useful if space is a constraint or you just do not care about backgrounds.
 - -DDEBUG
	 - You probably will not use this, but this executes certain test programs when the kernel loads, to test that program injection, interrupts, system calls, etc are working as they should.
 - -DCHS_LOAD
	 - While this does not currently work, this build feature changes the way the bootloader loads the main kernel image into memory--instead of assuming LBA to CHS conversions, it reads from the disk as if it were a 3.5'' floppy, with the standard CHS geometry for those kinds of disks. 
 - -DKEYBOARD_POLLING
	 - If IRQ interrupts are not being triggered by the PS/2 keyboard controller for whatever reason (certainly the case on certain pieces of hardware I have tested this OS on), then the keyboard driver needs an alternative method of logging scancodes: it will need to poll the keyboard controller continuously until the status bit indicates that the scancode buffer is full. Use this feature if you are unsure. 

After running the build script and qemu runs the image (it may just crash on qemu, depending on your build features), you can just use *dd* to flash the *kernel.img* product to a bootable medium of your choice (e.g., a floppy disk, a USB flash drive, etc) if you intend to use this on real hardware. Chances are, however, that it just will not work on your specific set of real hardware--actual OS development is a clusterfuck and different BIOSes do different things (e.g., my Thinkpad T530 may emulate the USB as a harddisk, but your/a different BIOS may emulate it as a floppy, my BIOS may initialize the PS/2 controller but yours may not, etc). If your piece of real hardware does not support legacy booting or is purely UEFI, this OS will **not** run. Your computer must also be i386 or x86_64, no exceptions there either.

Here is an example:

    $ ./build.sh -DREAL_HARDWARE -DCHS_LOAD -DNO_BACKGROUND -DKEYBOARD_POLLING

If you just want it to work within a virtual machine (i.e., qemu, has not been tested on other virtual machine software), just do:

    $ ./build.sh



## Inspiration

I did not wake up one day, without having done any research or work beforehand, immediately knowing how to write this project. I did not immediately spot and fix every bug or technicality upon first sight. I did not create this project in a mere day, or even a week--it took around 3 weeks, and it is still nowhere to being what I want it to be, let alone an actual practical OS. Furthermore, I did not come out of the womb knowing the intricacies of C or assembly. Instead, while I think I know C pretty well, I had various roadblocks and had to consult outside resources in order to resolve these issues. Certain resources are paramount for creating a similar project:

 - [osdev.org](https://wiki.osdev.org/Expanded_Main_Page)
	 - Without a doubt, a beacon of truth and knowledge for anything and everything OS development. Their forums and especially their wiki should be the first places you consult when you hit a roadblock or need information. Unfortunately, the legibility on certain wiki pages is poor (e.g., the PS/2 keyboard section is a bit hard to read, it takes a lot of strenuous heavy reading to get the I/O port number for the PS/2 keyboard unnecessarily). Nonetheless, it is a platform which deserves the uttermost respect--these people have an entire cornucopia of knowledge regarding OS development.
 - ChatGPT (certain annoying bugs or purely informational problems can be squashed with ease)
	 - Not recommended for complicated issues or if you just genuinely do not know what you are doing. ChatGPT very often makes mistakes and will inadvertently gaslight you as to what the issue is. However, if I need to quickly get the I/O port number for the PS/2 keyboard, ChatGPT will immediately respond with a clear and concise answer. 
 - Stack Overflow
	 - Self-explanatory--some strange bugs and misunderstandings were ameliorated by consulting this. 
