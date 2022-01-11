#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEV_NAME "/dev/lxy2"

int main()
{
	char buffer[64];
	int fd,a,b;

	fd = open(DEV_NAME, O_RDWR | O_CREAT);
	if (fd < 0) {
		printf("open device %s failded\n", DEV_NAME);
		return -1;
	}
	printf("请输入向缓冲区写的内容：");
	scanf("%s",buffer);
	write(fd,buffer,strlen(buffer));
	read(fd, buffer, 64);
	printf("从缓冲区读：%s\n", buffer);
	close(fd);
	
	return 0;
}

