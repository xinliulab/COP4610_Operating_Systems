obj-m += nuke.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) EXTRA_CFLAGS=-Wno-error modules

clean:
	make -C $(KDIR) M=$(PWD) clean
