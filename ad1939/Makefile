KDIR ?= /home/soos/Desktop/lab9/linux-socfpga-suhaib-qasem

default:
	$(MAKE) -C $(KDIR) ARCH=arm M=$(CURDIR) CROSS_COMPILE=arm-linux-gnueabihf-

clean:
	$(MAKE) -C $(KDIR) ARCH=arm M=$(CURDIR) clean

help:
	$(MAKE) -C $(KDIR) ARCH=arm M=$(CURDIR) help
