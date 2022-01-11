//LRU OPT淘汰算法
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <map>

using namespace std;

const int pageSize = 16;	//页面大小
const int instrCnt = pageSize * 150;		//指令数目
const int pageFrameCnt = 3;		//页框数

int orderCnt;		//访问次数
int cnt = 0;	//当前页框数
int pageMissCnt = 0;	//缺页数
int processArr[instrCnt];		//进程数组
int pageFrame[pageFrameCnt][pageSize];		//页框数组
int orderArr[100];		//指令访问序列
int pageIdx[pageFrameCnt];	//页框中存的页号

//访问序列

const int order2[] = {
	89,119,489,490,499,489,1000,1101,89,119,489,114,115,
	456,499,45,2002,2001,2002,119,112,113,455,2001,600,601,
	1100,1101,1102,2189,2002,210
};

//输出访问序列
inline void showOrderArray() {
	cout << "[页号]访问序列:\n";
	for (int i = 0; i < orderCnt; ++i)
		cout << '[' << orderArr[i] / pageSize << ']'
		<< orderArr[i] << endl;
	putchar('\n');
}
//初始化进程
inline void initProcess() {
	for (int i = 0; i < instrCnt; ++i) {
		processArr[i] = rand() % 1000;
	}
}


//将页号为pageNo页面的数据复制到第pfIdx的页框中
inline void copyPage(int pfIdx, int pageNo) {
	//复制数据
	memcpy(pageFrame[pfIdx],
		processArr + pageNo * pageSize, pageSize * sizeof(int));
	pageIdx[pfIdx] = pageNo;	//记录页号
}

void showPageFrame();
void LRU();
void OPT();
void initInstrOrder(const int* srcArr = nullptr, int size = 0);

int main() {
	srand(time(nullptr));
	initProcess();	
	//initInstrOrder(nullptr,0);
	initInstrOrder(order2, sizeof(order2) / sizeof(int));
	showOrderArray();

	LRU();
	//OPT();
	return 0;
}

//初始化访问序列
void initInstrOrder(const int* srcArr, int size) {
	//若有传入访问序列
	if (srcArr) {
		orderCnt = size;
		for (int i = 0; i < orderCnt; ++i) orderArr[i] = srcArr[i];
		return;
	}
	//否则随机生成
	orderCnt = 1000;
	for (int i = 0; i < orderCnt; ++i) {
		orderArr[i] = rand() % 2400;
	}
	return;
}

//输出页框状态
void showPageFrame() {
	cout << "页框状态:\n";
	int i;
	for (i = 0; i < cnt; ++i) {
		cout << "页框" << i + 1 << "  页号:" << pageIdx[i] << "  内容:";
		for (int j = 0; j < pageSize; ++j) {
			cout << pageFrame[i][j] << ' ';
		}
		putchar('\n');
	}
	for (; i < pageFrameCnt; ++i) {
		cout << "页框" << i + 1 << " 空\n";
	}
	putchar('\n');
}


//LRU
void LRU() {
	int timer[pageFrameCnt];
	memset(timer, 0, sizeof(timer));
	for (int i = 0; i < orderCnt; ++i) {
		auto pageNo = orderArr[i] / pageSize;	//页号
		auto offset = orderArr[i] % pageSize;	//页内偏移
		cout << "当前待访问序号: " << orderArr[i]
			<< "\t所在页: " << pageNo
			<< "\t值: " << processArr[orderArr[i]] << endl;

		int j;
		//命中
		for (j = 0; j < cnt; ++j) {
			if (pageIdx[j] == pageNo) {
				cout << "命中 " << pageFrame[j][offset] << endl;
				timer[j] = 0;
				break;
			}
		}
		//未命中
		if (j == cnt) {
			cout << "缺页 ";
			++pageMissCnt;	//缺页次数+1
			//若页框已全占满
			if (cnt == pageFrameCnt) {
				auto maxT = 0;
				//找到未使用时间最长的页框进行淘汰
				for (int k = 0; k < pageFrameCnt; ++k) {
					if (timer[k] > timer[maxT]) maxT = k;
				}
				copyPage(maxT, pageNo);
				timer[maxT] = 0;
				cout << pageFrame[maxT][offset] << endl;
			}
			//页框未全部占满则直接将页复制到空页框
			else {
				copyPage(cnt, pageNo);
				cout << pageFrame[cnt][offset] << endl;
				++cnt;
			}
		}
		for (int j = 0; j < cnt; ++j) ++timer[j];
		showPageFrame();
	}
	cout << "访问总次数:" << orderCnt << " 缺页次数:" << pageMissCnt
		<< " LRU缺页率:" << float(pageMissCnt) / orderCnt *100 <<"%" << endl;
	return;
}

//OPT
void OPT() {
	//待用信息表: 键名为待访问的页号,键值为一个存有该页号访问次序的堆栈
	map<int, stack<int>> ms;
	//逆序变量访问序列
	for (int i = orderCnt - 1; i >= 0; --i) {
		auto pageNo = orderArr[i] / pageSize;	//页号
		//若该页未被访问
		if (ms.count(pageNo) == 0) {
			stack<int> tmp;		//创建堆栈
			tmp.push(i);	//在堆栈中添加访问次序
			ms.insert(pair<int, stack<int>>(pageNo, tmp));
		}
		else {
			ms.at(pageNo).push(i);	//在堆栈中添加访问次序
		}
	}
	//顺序执行
	for (int i = 0; i < orderCnt; ++i) {
		auto pageNo = orderArr[i] / pageSize;	//页号
		auto offset = orderArr[i] % pageSize;	//页内偏移
		cout << "当前待访问序号: " << orderArr[i]
			<< "\t所在页: " << pageNo
			<< "\t值: " << processArr[orderArr[i]] << endl;

		//每次执行完将该页栈顶的待用信息出栈
		ms.at(pageNo).pop();
		int j;
		//遍历页框若命中
		for (j = 0; j < cnt; ++j) {
			if (pageIdx[j] == pageNo) {
				cout << "命中 " << pageFrame[j][offset] << endl;
				break;
			}
		}
		//若未命中
		if (j == cnt) {
			cout << "缺页 ";
			++pageMissCnt;	//缺页次数+1
			//若页框已全占满
			if (cnt == pageFrameCnt) {
				auto maxT = 0;
				//遍历页框根据待用信息寻找不在需要或最远的将来才用的页面
				for (int k = 0; k < pageFrameCnt; ++k) {
					if (ms.at(pageIdx[k]).size() == 0) {
						maxT = k;
						break;
					}
					else if (ms.at(pageIdx[k]).top() > ms.at(pageIdx[maxT]).top()) {
						maxT = k;
					}
				}
				//淘汰页面复制新数据
				copyPage(maxT, pageNo);
				cout << pageFrame[maxT][offset] << endl;
			}
			//页框未全部占满则直接将页复制到空页框
			else {
				copyPage(cnt, pageNo);
				cout << pageFrame[cnt][offset] << endl;
				++cnt;
			}
		}
		//输出每次的页框信息
		showPageFrame();
	}
	cout << "访问总次数:" << orderCnt << " 缺页次数:" << pageMissCnt
		<< " OPT缺页率:" << float(pageMissCnt) / orderCnt *100<<"%"<< endl;
}

