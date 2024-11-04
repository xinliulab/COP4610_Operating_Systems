# Makefile for nuke kernel module

# Define module name
obj-m := nuke.o

# Set kernel directory (change this to your kernel source directory)
KDIR := /lib/modules/$(shell uname -r)/build

# Define current directory
PWD := $(shell pwd)

# Default make command
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean command
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
