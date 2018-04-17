obj-m += mem.o

all: 
	make -C /usr/src/kernels/4.8.6-300.fc25.i686 M=$(PWD) modules
clean: 
	make -C /usr/src/kernels/4.8.6-300.fc25.i686 M=$(PWD) clean
