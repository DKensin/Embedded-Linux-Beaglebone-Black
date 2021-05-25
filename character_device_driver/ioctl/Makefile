obj-m += misc.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) ioctl.c -o ioctl_app

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -r ioctl_app
