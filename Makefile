ARCH := i686
CROSS := ~/opt/cross/bin
SYSROOT := sysroot

# Compiler options.
CC := $(CROSS)/i686-elf-gcc
CCSYSROOT := --sysroot=./$(SYSROOT) -isystem=/usr/include
WARNINGS := -Wall -Wextra -Wpedantic
CFLAGS := -O2 $(WARNINGS) $(CCSYSROOT) -ffreestanding -nostdlib -std=gnu11

# Assembler options.
AS := nasm
ASFLAGS := -felf32

# Linker options
LD := $(CROSS)/i686-elf-gcc
LIBS = -nostdlib -lgcc
LDFLAGS := -T linker/link.ld

# Pattern specific rules
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.asm
	$(AS) $(ASFLAGS) $<

# Targets.
.PHONY: all headers libc iso

all: sysroot headers kernel libc kernel iso

sysroot:
	mkdir -p $(SYSROOT)
	mkdir -p $(SYSROOT)/boot
	mkdir -p $(SYSROOT)/usr
	mkdir -p $(SYSROOT)/usr/lib
	mkdir -p $(SYSROOT)/usr/include

headers:
	cp -r include $(SYSROOT)/usr/

####################
#      KERNEL      #
####################
.PHONY: kernel

KERNEL_OBJS  = $(patsubst %.c,%.o,$(wildcard kernel/*.c))
KERNEL_OBJS += $(patsubst %.asm,%.o,$(wildcard kernel/*.asm))

kernel: $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) $(LIBS) $(KERNEL_OBJS) -o $(SYSROOT)/boot/kernel.elf32



####################
#      LIBC        #
####################
.PHONY: libc



####################
#      CLEAN       #
####################
.PHONY: clean clean-all

clean: clean-all

clean-all: clean-sysroot clean-objs

clean-sysroot:
	rm -rvf sysroot

clean-objs:
	rm -rv kernel/*.o


