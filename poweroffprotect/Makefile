export ARCH=arm
export CROSS_COMPILE=arm-buildroot-linux-uclibcgnueabihf-
AR=$(CROSS_COMPILE)ar
LD=$(CROSS_COMPILE)ld
CC=$(CROSS_COMPILE)gcc
EXTRA_CFLAGS := -O0 -g -DEXPORT_SYMTAB
LOCAL_STRIP_MODULE := false
DEBFLAGS=-02
PWD := $(shell pwd)
KERNELDIR?=/home/tao/work/sama5d3/linux-3.15/
obj-m := powerprotect.o
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)
	cp powerprotect.ko /nfs/
	sync
clean:
	rm -rf *.o *.ko *.mod.c  *.mod.o *.order  *.symvers
