#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#define UL unsigned long
#define U64 uint64_t
char buf[100];
const int a = 20;
void fun(char* str, UL pid, UL viraddress, UL* phyaddress)
{
    U64 temp = 0;
    int pageSize = getpagesize();
    UL vir_pageIndex = viraddress / pageSize;
    UL vir_offset = vir_pageIndex * sizeof(U64);
    UL page_offset = viraddress % pageSize;//虚拟地址在页面中的偏移量
    sprintf(buf, "%s%lu%s", "/proc/", pid, "/pagemap");
    int fd = open(buf, O_RDONLY);//只读打开
    lseek(fd, vir_offset, SEEK_SET);//游标移动
    read(fd, &temp, sizeof(U64));//读取对应项的值
    U64 phy_pageIndex = (((U64)1 << 55) - 1) & temp;//物理页号
    *phyaddress = (phy_pageIndex * pageSize) + page_offset;//加页内偏移量得物理地址
    printf("<%s>  pid = %lu, Virtual address = 0x%lx, Page Number= %lu, Physical Page Frame Number = %lu physical Address = 0x%lx\n ", str, pid, viraddress, vir_pageIndex,phy_pageIndex, *phyaddress);
	sleep(1);
	return;
}
int main()
{
    int b = 1;
    const int d = 3;
    UL phy = 4;
    int pid = fork();
    fun("Local variable", getpid(), (UL)&b, &phy);
    fun("Local constant", getpid(), (UL)&d, &phy);
    fun("Global constant", getpid(), (UL)&a, &phy);
    return 0;
}
