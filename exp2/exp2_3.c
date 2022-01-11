//Linux父子进程同步
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#define EX_OK           0       /* successful termination */
int main(){
	pid_t pid,p;
	int status,i;
	pid=fork();
	if(pid==0){	//子进程
		printf("Son: This is Child Process pid=%d\nSleep 5s ing....\n",getpid());
		sleep(5);	//休眠5s结束
		printf("Son: Child Process will stop\n\n");
		exit(EX_OK);	//结束进程
	}
	else if(pid>0){
		p=wait(&status);	//等待子进程结束，返回子进程进程号
		i=WEXITSTATUS(status);	//分析子进程返回信息
		printf("Father:This is Parent Process pid=%d\nChild's pid=%d    exit status=%d\n\n",getpid(),p,i);
		printf("NOTES:  status=0   successful termination\n\tstatus!=0  Failure termination\n");
	}
	return 0;
}
