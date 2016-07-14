#!Makefile

# --------------------------------------------------------
#
#    hurlex 这个小内核的 Makefile
#
#    默认使用的C语言编译器是 GCC、汇编语言编译器是 nasm
#
#        qianyi.lh    2014/11/04   12:08:13
# --------------------------------------------------------

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = nasm

C_FLAGS = -std=c99 -c -m32 -Wall -Wextra -ggdb -gstabs+ -ffreestanding \
                 -I. -Iinclude -Iarch/i386 -Iarch/i386/include
LD_FLAGS = -T scripts/kernel.lds -nostdlib  -m elf_i386
ASM_FLAGS = -f elf -g -F stabs  

all: $(S_OBJECTS) $(C_OBJECTS) link 

# The automatic variable `$<' is just the first prerequisite
.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	@echo 编译汇编文件 $< ...
	$(ASM) $(ASM_FLAGS) $<

link:
	@echo 链接内核文件...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o hx_kernel

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) hx_kernel


.PHONY:iso
iso:
	cp hx_kernel isodir/boot/
	grub2-mkrescue -o hurlex.iso isodir

.PHONY:runiso
runiso:
	qemu -m 128 -hda disk.img -cdrom hurlex.iso -boot d

.PHONY:runfd
runfd:
	qemu -m 128 -hda disk.img -fda floppy.img -boot a

.PHONY:qemu
qemu:
	qemu -m 128 -hda disk.img -kernel hx_kernel

.PHONY:debug
debug:
	#gdb -x scripts/gdbinit &
	qemu  -s -m 128 -hda disk.img -cdrom hurlex.iso -boot d 
	sleep 1
	#gdb -x scripts/gdbinit
.PHONY:code_line_count
code_line_count:
	find . -type f -name "*.[c|h|s]" -exec cat {} \; | wc -l
