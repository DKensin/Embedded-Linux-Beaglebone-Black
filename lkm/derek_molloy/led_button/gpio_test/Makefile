obj-m += gpio_test.o

all:
	make ARCH=arm CROSS_COMPILE=/home/toan/bbb/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- -C /home/toan/bbb/bb-kernel/KERNEL M=$(PWD) modules

clean:
	make -C /home/toan/bbb/bb-kernel/KERNEL M=$(PWD) clean
