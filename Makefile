obj-m += mymodule.o

mymodule-objs := mymod.o myfops.o

CFLAGS_mymod.o := -O2 -Wall -Wstrict-prototypes -Wmissing-prototypes

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
