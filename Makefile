DEBUG = y

EXTRA_CFLAGS += -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter
ifeq ($(DEBUG),y)
	EXTRA_CFLAGS += -O -g -DLEDPANEL_DEBUG
endif

ifneq ($(KERNELRELEASE),)
	obj-m := ledpanel.o
else
	KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers

endif