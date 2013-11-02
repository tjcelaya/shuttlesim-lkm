ifneq ($(KERNELRELEASE),)
	obj-m := shuttle.o
else
	KERNELDIR ?= /lib/modules/`uname -r`/build/
	PWD := `pwd`
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules && gcc -o Test_driver test.c
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
endif
