CONFIG_MODULE_SIG=n
obj-m:=lxy2.o
CURRENT_PATH:=$(shell pwd)

all :   
	make -C /lib/modules/$(shell uname -r)/build M=$(CURRENT_PATH) modules   
clean :   
	make -C /lib/modules/$(shell uname -r)/build M=$(CURRENT_PATH) clean
