obj-m := pen_driver.o
KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)
default:
	$(MAKE)	-C $(KDIR) M=$(shell pwd) modules
clean:
	rm -rfv *.o *.ko *.mod *.mod.c *.order *.symvers
