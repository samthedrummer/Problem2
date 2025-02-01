obj-m += hello.o

all:
	# The line below must begin with a tab.
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	# Again, this line must begin with a tab.
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

