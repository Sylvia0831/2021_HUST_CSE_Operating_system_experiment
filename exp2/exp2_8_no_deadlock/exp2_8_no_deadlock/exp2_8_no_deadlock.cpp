//哲学家问题-无死锁
#include <windows.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>
using namespace std;

const unsigned PhiCnt = 5;	//哲学家数量
const unsigned sleepTime = 3000;

bool exxitflag = true;	//用于退出的标志
int phiS[PhiCnt];	//每个哲学家的筷子(0:思考,1:一只筷子,2:进餐)
int diningCnt = 0;
CRITICAL_SECTION cse;//临界区
HANDLE s[PhiCnt];	//每根筷子对应的信号量

inline void beforeDining(int i, int k) {
	printf("哲学家%d 拿起%d号筷子 现有%d支筷子 ", i, k, ++phiS[i]);
	if (phiS[i] == 1) printf("不能进餐\n");
	else if (phiS[i] == 2) printf("开始进餐\n");
}

inline void afterDining(int i, int k) {
	printf("哲学家%d 放下%d号筷子\n", i, k);
	--phiS[i];
}

inline void dining(int i) {
	printf("哲学家%i 就餐\n", i);
	printf("NOTES:状态0为哲学家正在思考，状态1表示持有一只筷子，状态2表示正在进餐\n哲学家状态：\n");
	printf("[哲学家0号:%d] [哲学家1号:%d] [哲学家2号:%d] [哲学家3号:%d] [哲学家4号:%d]\n\n",
		phiS[0], phiS[1], phiS[2], phiS[3], phiS[4]);
	Sleep(sleepTime);
}

DWORD WINAPI philosopher(LPVOID lpPara) {
	int i = int(lpPara);
	while (exxitflag) {
		auto stop = rand() % 400 + 100;
		Sleep(stop);
		if (diningCnt == PhiCnt - 1) continue; //最多4个人同时去拿筷子
		WaitForSingleObject(s[i], INFINITE);	//等待左侧筷子可用

		EnterCriticalSection(&cse);
		++diningCnt;	//拿筷子人数+1
		LeaveCriticalSection(&cse);

		beforeDining(i, i);
		WaitForSingleObject(s[(i + PhiCnt - 1) % PhiCnt], INFINITE);	//等待右侧筷子可用
		beforeDining(i, (i + PhiCnt - 1) % PhiCnt);
		dining(i);
		ReleaseSemaphore(s[(i + PhiCnt - 1) % PhiCnt], 1, NULL);	//放下右侧筷子
		afterDining(i, (i + PhiCnt - 1) % PhiCnt);
		ReleaseSemaphore(s[i], 1, NULL);	//放下左侧筷子
		afterDining(i, i);

		EnterCriticalSection(&cse);
		--diningCnt;	//拿筷子人数-1
		LeaveCriticalSection(&cse);
	}
	return 0;
}


int main() {
	srand(unsigned(time(nullptr)));
	for (int i = 0; i < PhiCnt; ++i) {
		s[i] = CreateSemaphore(NULL, 1, 1, NULL);
	}
	InitializeCriticalSection(&cse);
	HANDLE hThread[PhiCnt];
	DWORD phs[PhiCnt];
	for (int i = 0; i < PhiCnt; ++i) {
		hThread[i] = CreateThread(NULL, 0, philosopher, LPVOID(i), 0, &phs[i]);
		if (!hThread[i]) return -1;
	}
	while (exxitflag) {
		if (getchar()) exxitflag = false;
	}
	return 0;
}

