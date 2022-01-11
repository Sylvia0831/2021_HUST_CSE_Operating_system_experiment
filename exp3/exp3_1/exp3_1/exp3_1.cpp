
#include <iostream>
#include <ctime>
#include <Windows.h>
using namespace std;
int times = 1;	//倍数
const int row = 2048 ; 	//行数
const int col = 1024 ;	//列数
int myArray[row*10][col*10];	//大数组
int main() {
	cout << "当前进程号为：" << GetCurrentProcessId() << endl;
	int x, y;
	while (1) {

		cout << "1.先列后行遍历（局部性差）2.先行后列遍历（局部性好）3 退出\n";
		cin >> x;
		if (x==3) return 0; 
		cout << "1.数组大（20480*10240） 2.数组小（2048*1024）\n";
		cin >> y;
		if (y == 1) times = 10; else times = 1;
		if (x == 1) {						//先列后行遍历数组
			clock_t s = clock();
			for (int i = 0; i < col*times; ++i) {
				for (int j = 0; j < row*times; ++j) {
					myArray[j][i] = 0;
				}
			}
			clock_t e = clock();
			cout << "花费 " << (double)(e - s) / CLOCKS_PER_SEC << "s\n";	//遍历耗时
		}
		else if (x == 2) {					//先行后列遍历数组
			clock_t s = clock();
			for (int i = 0; i < row*times; ++i) {
				for (int j = 0; j < col*times; ++j) {
					myArray[i][j] = 0;
				}
			}
			clock_t e = clock();
			cout << "花费 " << (double)(e - s) / CLOCKS_PER_SEC << "s\n";	//遍历耗时
		}
		system("pause");
	}
	return 0;
}

