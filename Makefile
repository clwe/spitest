# Toolchain Path
CROSS_COMPILE := /usr/bin/arm-linux-gnueabihf-
# Linux Kernel header
KERNEL := /lib/modules/$(shell uname -r)/build
# Architecture
ARCH := arm


MODULE_NAME=spitest

#EXTRA_CFLAGS += -I$(PWD)/include -I$(PWD) -DEBUG

# module_objects += helloworld.o

PWD := $(shell pwd)

obj-m := $(MODULE_NAME).o
#$(MODULE_NAME)-y := helloworld.o # only for multiple input files!



all: 
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL) M=$(PWD) modules

clean:
	rm -rf *.o *.ko
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL) M=$(PWD) clean
