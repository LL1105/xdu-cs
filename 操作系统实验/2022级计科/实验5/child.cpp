#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

int main() {
	bool rc = 0;
	char inBuffer[50] = "";
	char outBuffer[50] = "";
	DWORD bytesRead;
	int err = 0;

	while (true) {
		strcpy(inBuffer, "");
		strcpy(outBuffer, "");
		cout << "请输入要发送的数据，输入end结束：" << endl;
		cin >> outBuffer;
		rc = strcmp(outBuffer, "end");
		if (rc == 0) {
			rc = CallNamedPipe("\\\\.\\pipe\\myPipe", outBuffer, sizeof(outBuffer), inBuffer, sizeof(inBuffer),
			                   &bytesRead, NMPWAIT_USE_DEFAULT_WAIT);
			break;
		}
		rc = WaitNamedPipe("\\\\.\\pipe\\myPipe", NMPWAIT_WAIT_FOREVER);
		if (rc == 0) {
			err = GetLastError();
			cout << "等待管道失败，err = " << err << endl << endl;
			exit(1);
		} else {
			cout << "等待管道成功，正在等待..." << endl << endl;
		}
		rc = CallNamedPipe("\\\\.\\pipe\\myPipe", outBuffer, sizeof(outBuffer), inBuffer, sizeof(inBuffer),
		                   &bytesRead, NMPWAIT_USE_DEFAULT_WAIT);
		if (rc == 0) {
			err = GetLastError();
			cout << "管道连接失败，err = " << err << endl << endl;
			exit(1);
		} else {
			rc = strcmp(inBuffer, "end");
			if (rc == 0) {
				break;
			}
			cout << "管道连接成功，读取管道数据如下：" << endl;
			cout << inBuffer << endl << endl;
		}
	}
	cout << "结束管道通信" << endl << endl;
	system("pause");
	return 0;
}