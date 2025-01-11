#obj-m := main.o 
obj-m += dddd.o 
dddd-objs := main.o voter.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
