#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
int main (){
	long a;
	a=syscall(455,23,4);
	printf("a=%ld\n",a);
	a=syscall(455,12,12);
	printf("a=%ld\n",a);
	a=syscall(456,1,4,2);
	printf("a=%ld\n",a);
	a=syscall(456,4,12,5);
	printf("a=%ld\n",a);
	return 0;
}



