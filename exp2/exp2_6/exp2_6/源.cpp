//������-����������
#include <windows.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
//����������
#define BufLen 10 
//2�������߳�
#define ProducerCnt 2
//3������߳�
#define ConsumerCnt 3
//ÿ������������ʱ��
#define sleepTime 1000


int produnctId1 = 1000, produnctId2 = 2000;	//2�������ߵ�������Ʒ��ʼ�ţ�1000-1999��2000-2999
int consumeId;	//���ѵĲ�Ʒ��
int buf[BufLen];	//10��Ԫ�صĻ�����
int pIdx = 0, cIdx = 0;	//�ֱ��¼��������ź����ѵ����
bool exitflag = true;	//�����˳��ı�־
HANDLE sFull, sEmpty;	//�ź���
CRITICAL_SECTION cse;	//�ٽ���

void produce(int no);
void consume();
DWORD WINAPI producer(LPVOID lpPara);
DWORD WINAPI consumer(LPVOID lpPara);//��������

//����
void produce(int no) {
	auto produnctId = no == 0 ? produnctId1++ : produnctId2++;
	cout << "������Ʒ: " << ++produnctId << endl;
	cout << "����Ʒ���뻺����\n";
	buf[pIdx] = produnctId;
	cout << "������״̬:";
	for (int i = 0; i < BufLen; ++i) {
		if (buf[i] != 0) cout << " (" << i + 1 << ')' << buf[i];
		else cout << " (" << i + 1 << ')' << "NULL";
		if (i == pIdx) cout << "<-����";
	}
	cout << endl << endl;
	pIdx = (pIdx + 1) % BufLen;
	Sleep(sleepTime);
}
void consume() {
	cout << "�ӻ�����ȡ����Ʒ\n";
	consumeId = buf[cIdx];
	cout << "������״̬:";
	for (int i = 0; i < BufLen; ++i) {
		if (buf[i] != 0) cout << " (" << i + 1 << ')' << buf[i];
		else cout << " (" << i + 1 << ')' << "NULL";
		if (i == cIdx) cout << "<-����";
	}
	cout << endl;
	buf[cIdx] = 0;
	cIdx = (cIdx + 1) % BufLen;
	cout << "���Ѳ�Ʒ: " << consumeId << endl << endl;
	Sleep(sleepTime);
}

//������
DWORD WINAPI producer(LPVOID lpPara) {
	int no = int(lpPara);	//�����߱��
	while (exitflag) {
		auto step = rand() % 900 + 100;//���һ��100ms-1s�ļ��ʱ��
		WaitForSingleObject(sEmpty, INFINITE);	//Ҫ�󻺳����п�λ
		//INFINITE:���󱻴����źź����Ż᷵��
		EnterCriticalSection(&cse);		//����
		Sleep(step);
		produce(no);
		LeaveCriticalSection(&cse);
		ReleaseSemaphore(sFull, 1, NULL);	//����+1
	}
	return 0;
}

//������
DWORD WINAPI consumer(LPVOID lpPara) {
	while (exitflag) {
		auto step = rand() % 900 + 100; //���һ��100ms-1s�ļ��ʱ��
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
	//�����ź������ٽ���
	sFull = CreateSemaphore(NULL, 0, BufLen, NULL);		//���������ݸ���
	sEmpty = CreateSemaphore(NULL, BufLen, BufLen, NULL);	//��������λ����
	//sMutex = CreateSemaphore(NULL, 1, 1, NULL);
	InitializeCriticalSection(&cse);
	//�����������߳�
	HANDLE hThread[ProducerCnt + ConsumerCnt];
	DWORD producers[ProducerCnt], consumers[ConsumerCnt];
	for (int i = 0; i < ProducerCnt; ++i) {
		hThread[i] = CreateThread(NULL, 0, producer, LPVOID(i), 0, &producers[i]);
		if (!hThread[i]) return -1;
	}
	//�����������߳�
	for (int i = 0; i < ConsumerCnt; ++i) {
		hThread[ProducerCnt + i]
			= CreateThread(NULL, 0, consumer, NULL, 0, &consumers[i]);
		if (!hThread[ProducerCnt + i]) return -1;
	}
	//���������ַ���ֹ
	while (exitflag) {
		if (getchar()) exitflag = false;
	}
	return 0;
}

