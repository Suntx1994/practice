CC=gcc
all:
	$(CROSS_COMPILE)$(CC) -o lkmhello lkmhello.c
	make -C ../../.. M=$(PWD) module

clean:
	make -C ../../.. M=$(PWD) clean
