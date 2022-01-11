CONFIG_MODULE_SIG=n
obj-m += lixinyu.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=/home/sylvia/exp4 modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=/home/sylvia/exp4 clean
