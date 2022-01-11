#include<unistd.h>
#include<stdio.h>
//父进程先结束
int main()
{
	printf("\n'Father:'代表父进程；'Son:'代表子进程。\n\n");
	pid_t p1=fork();
	if(p1){
		printf("Father: 1. 父进程进程号：%d\n",getpid());
        printf("Father: 1. 创建的子进程进程号：%d\n",p1);
		sleep(5);
		printf("Father: 2. 父进程结束\n");
	}else{
		printf("Son: 1. 子进程进程号：%d\n",getpid());
		printf("Son: 1. 父进程未结束时，父进程进程号：%d\n",getppid());
		printf("Son: 3. 暂时挂起子进程7s,ps检查结果：\n");
		sleep(7);
		printf("Son: 2. 父进程结束后，子进程的父进程进程号：%d\n",getppid());
		printf("Son: 3. 子进程继续挂起7s，同时再用ps检查结果：\n");
		sleep(7);       
		printf("Son: 3. 子进程结束\n");
	}
	return 0;
}
