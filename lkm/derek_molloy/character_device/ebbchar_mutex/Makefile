obj-m += ebbchar_mutex.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	$(CC) test_ebbchar_mutex.c -o test_ebbchar_mutex

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -r test_ebbchar_mutex
