KERNEL_SOURCE ?= /lib/modules/$(shell uname -r)/build

obj-m := ddb.o
ddb-y := init.o stub.o

modules clean:
	make -C $(KERNEL_SOURCE) M=$(PWD) $@

.PHONY: modules clean
