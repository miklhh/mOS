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
LIBS := -nostdlib -lgcc
LDFLAGS := -T linker/link.ld

# Pattern specific compile rules.
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.asm
	$(AS) $(ASFLAGS) $<

####################
#     TARGETS      #
####################
.PHONY: all sysroot headers libc kernel iso

all: sysroot headers libc drivers kernel iso

sysroot:
	mkdir -p $(SYSROOT)
	mkdir -p $(SYSROOT)/boot
	mkdir -p $(SYSROOT)/usr
	mkdir -p $(SYSROOT)/usr/lib
	mkdir -p $(SYSROOT)/usr/include

headers:
	cp -r include $(SYSROOT)/usr/


####################
#      LIBC        #
####################
.PHONY: libc



####################
#     DRIVERS      #
####################
.PHONY: drivers

DRIVER_OBJS := $(patsubst %.c,%.o,$(wildcard drivers/*/*.c))

drivers: $(DRIVER_OBJS)


####################
#      KERNEL      #
####################
.PHONY: kernel kernel-objs

KERNEL_OBJS  = $(patsubst %.c,%.o,$(wildcard kernel/*.c))
KERNEL_OBJS += $(patsubst %.asm,%.o,$(wildcard kernel/*.asm))
KERNEL_OBJS += $(DRIVER_OBJS)
KERNEL_NAME  = kernel32.elf

kernel-objs: $(KERNEL_OBJS)

kernel: kernel-objs drivers
	$(LD) $(LDFLAGS) $(LIBS) $(KERNEL_OBJS) -o $(SYSROOT)/boot/$(KERNEL_NAME)


####################
#      CLEAN       #
####################
.PHONY: clean clean-all clean-sysroot clean-objs clean-drivers

clean: clean-all

clean-all: clean-sysroot clean-objs clean-drivers

clean-sysroot:
	-rm -rvf sysroot

clean-objs:
	-rm -rv kernel/*.o

clean-drivers:
	-rm -rv drivers/*/*.o
