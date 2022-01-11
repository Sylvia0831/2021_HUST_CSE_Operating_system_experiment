//生产者-消费者问题
#include <windows.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
//缓冲区长度
#define BufLen 10 
//2个输入线程
#define ProducerCnt 2
//3个输出线程
#define ConsumerCnt 3
//每次输出后的休眠时间
#define sleepTime 1000


int produnctId1 = 1000, produnctId2 = 2000;	//2个生产者的生产产品起始号，1000-1999，2000-2999
int consumeId;	//消费的产品号
int buf[BufLen];	//10个元素的缓冲区
int pIdx = 0, cIdx = 0;	//分别记录生产的序号和消费的序号
bool exitflag = true;	//用于退出的标志
HANDLE sFull, sEmpty;	//信号量
CRITICAL_SECTION cse;	//临界区

void produce(int no);
void consume();
DWORD WINAPI producer(LPVOID lpPara);
DWORD WINAPI consumer(LPVOID lpPara);//函数声明

//生产
void produce(int no) {
	auto produnctId = no == 0 ? produnctId1++ : produnctId2++;
	cout << "生产产品: " << ++produnctId << endl;
	cout << "将产品放入缓冲区\n";
	buf[pIdx] = produnctId;
	cout << "缓冲区状态:";
	for (int i = 0; i < BufLen; ++i) {
		if (buf[i] != 0) cout << " (" << i + 1 << ')' << buf[i];
		else cout << " (" << i + 1 << ')' << "NULL";
		if (i == pIdx) cout << "<-生产";
	}
	cout << endl << endl;
	pIdx = (pIdx + 1) % BufLen;
	Sleep(sleepTime);
}
void consume() {
	cout << "从缓冲区取出产品\n";
	consumeId = buf[cIdx];
	cout << "缓冲区状态:";
	for (int i = 0; i < BufLen; ++i) {
		if (buf[i] != 0) cout << " (" << i + 1 << ')' << buf[i];
		else cout << " (" << i + 1 << ')' << "NULL";
		if (i == cIdx) cout << "<-消费";
	}
	cout << endl;
	buf[cIdx] = 0;
	cIdx = (cIdx + 1) % BufLen;
	cout << "消费产品: " << consumeId << endl << endl;
	Sleep(sleepTime);
}

//生产者
DWORD WINAPI producer(LPVOID lpPara) {
	int no = int(lpPara);	//生产者编号
	while (exitflag) {
		auto step = rand() % 900 + 100;//随机一个100ms-1s的间隔时间
		WaitForSingleObject(sEmpty, INFINITE);	//要求缓冲区有空位
		//INFINITE:对象被触发信号后函数才会返回
		EnterCriticalSection(&cse);		//互斥
		Sleep(step);
		produce(no);
		LeaveCriticalSection(&cse);
		ReleaseSemaphore(sFull, 1, NULL);	//数据+1
	}
	return 0;
}

//消费者
DWORD WINAPI consumer(LPVOID lpPara) {
	while (exitflag) {
		auto step = rand() % 900 + 100; //随机一个100ms-1s的间隔时间
		WaitForSingleObject(sFull, INFINITE);
		EnterCriticalSection(&cse);
		Sleep(step);
		consume();
		LeaveCriticalSection(&cse);
		ReleaseSemaphore(sEmpty, 1, NULL);
	}
	return 0;
}

int main() {
	srand(unsigned(time(nullptr)));
	//创建信号量和临界区
	sFull = CreateSemaphore(NULL, 0, BufLen, NULL);		//缓冲区数据个数
	sEmpty = CreateSemaphore(NULL, BufLen, BufLen, NULL);	//缓冲区空位个数
	//sMutex = CreateSemaphore(NULL, 1, 1, NULL);
	InitializeCriticalSection(&cse);
	//创建生产者线程
	HANDLE hThread[ProducerCnt + ConsumerCnt];
	DWORD producers[ProducerCnt], consumers[ConsumerCnt];
	for (int i = 0; i < ProducerCnt; ++i) {
		hThread[i] = CreateThread(NULL, 0, producer, LPVOID(i), 0, &producers[i]);
		if (!hThread[i]) return -1;
	}
	//创建消费者线程
	for (int i = 0; i < ConsumerCnt; ++i) {
		hThread[ProducerCnt + i]
			= CreateThread(NULL, 0, consumer, NULL, 0, &consumers[i]);
		if (!hThread[ProducerCnt + i]) return -1;
	}
	//输入任意字符终止
	while (exitflag) {
		if (getchar()) exitflag = false;
	}
	return 0;
}

