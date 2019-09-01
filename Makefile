ARCH := i686
CROSSDIR := ~/opt/cross/bin
SYSROOT := sysroot

# Compiler options.
CC := $(CROSSDIR)/i686-elf-gcc
CCSETSYSROOT := --sysroot=$(SYSROOT) -isystem=/usr/include
WARNINGS := -Wall -Wextra -Wpedantic
CFLAGS := -O2 $(WARNINGS) $(CCSETSYSROOT) -ffreestanding -nostdlib -std=gnu11 -g

# Assembler options.
AS := nasm
ASFLAGS := -felf32

# Linker options
LD := $(CROSSDIR)/i686-elf-gcc
LIBS := -lgcc -lc
LDFLAGS := -nostdlib -Tlink.ld

# Archiver options.
AR := $(CROSSDIR)/i686-elf-ar

# Pattern specific compile rules.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $<


####################
#       ALL        #
####################
.PHONY: all
all: sysroot headers libc drivers modules kernel bin os.iso


###################
#     SYSROOT     #
###################
.PHONY: sysroot
sysroot:
	mkdir -p $(SYSROOT)
	mkdir -p $(SYSROOT)/boot
	mkdir -p $(SYSROOT)/boot/grub
	mkdir -p $(SYSROOT)/usr
	mkdir -p $(SYSROOT)/usr/lib
	mkdir -p $(SYSROOT)/usr/include
	mkdir -p $(SYSROOT)/modules
	cp kernel/grub.cfg $(SYSROOT)/boot/grub/


####################
#     HEADERS      #
####################
.PHONY: headers
HEADERS = $(wildcard headers/*.h)
HEADERS = $(wildcard headers/*/*.h)
HEADERS = $(wildcard headers/*/*/*.h)
headers: $(HEADERS)
	cp -r include $(SYSROOT)/usr/


####################
#      LIBC        #
####################
.PHONY: libc
LIBC_OBJS := $(patsubst %.c,%.o,$(wildcard libc/*.c))
libc: $(LIBC_OBJS)
	$(AR) rcs $(SYSROOT)/usr/lib/libc.a $(LIBC_OBJS)



####################
#     DRIVERS      #
####################
.PHONY: drivers
DRIVER_OBJS := $(patsubst %.c,%.o,$(wildcard drivers/*/*.c))
drivers: $(DRIVER_OBJS)


####################
#      KERNEL      #
####################
.PHONY: kernel
KERNEL_OBJS  = $(patsubst %.c,%.o,$(wildcard kernel/*.c))
KERNEL_OBJS += $(patsubst %.c,%.o,$(wildcard kernel/isr/*.c))
KERNEL_OBJS += $(patsubst %.c,%.o,$(wildcard kernel/memory/*.c))
KERNEL_OBJS += $(patsubst %.c,%.o,$(wildcard kernel/memory/heap_allocator/*.c))
KERNEL_OBJS += $(patsubst %.c,%.o,$(wildcard kernel/thread/*.c))
KERNEL_OBJS += $(patsubst %.asm,%.o,$(wildcard kernel/thread/*.asm))
KERNEL_OBJS += $(patsubst %.asm,%.o,$(wildcard kernel/*.asm))
KERNEL_OBJS += $(patsubst %.asm,%.o,$(wildcard kernel/isr/*.asm))
kernel: $(KERNEL_OBJS)

###################
#     MODULES     #
###################
.PHONY: modules
modules:
	nasm -fbin modules/test.asm -o sysroot/modules/test

####################
#     BIN/ELF      #
####################
.PHONY: bin elf
FILENAME  = kernel32.elf
OBJS = $(KERNEL_OBJS) $(DRIVER_OBJS)
elf: sysroot headers kernel drivers libc
	$(LD) $(CCSETSYSROOT) $(LDFLAGS) $(OBJS) $(LIBS) -o $(SYSROOT)/boot/$(FILENAME)
bin: elf

####################
#       ISO        #
####################
os.iso: bin
	grub-mkrescue sysroot/ -o os.iso


####################
#      CLEAN       #
####################
.PHONY: clean clean-all clean-sysroot clean-objs clean-drivers
clean: clean-all
clean-all: clean-sysroot clean-objs clean-drivers clean-libc clean-iso
clean-sysroot:
	@-rm -rvf sysroot
clean-objs:
	@-rm -rv kernel/*.o
	@-rm -rv kernel/isr/*.o
	@-rm -rv kernel/memory/*.o
	@-rm -rv kernel/memory/heap_allocator/*.o
	@-rm -rv kernel/thread/*.o
clean-drivers:
	@-rm -rv drivers/*/*.o
clean-libc:
	@-rm -rv libc/*.o
clean-iso:
	@-rm -rv os.iso
