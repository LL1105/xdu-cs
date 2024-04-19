#include <iostream>
#include <windows.h>
#include <string.h>
#include <stdio.h>
using namespace std;

BOOL writePipe(HANDLE hWrite) {
	BOOL isSuccess = FALSE;
	char *str = "HELLO CHILED\n\n";
	DWORD dwWrite;
	while (true) {
		isSuccess = WriteFile(hWrite, str, strlen(str), &dwWrite, NULL);
		if (isSuccess) {
			break;
		}
	}
	CloseHandle(hWrite);
	return isSuccess;
}

BOOL readPipe(HANDLE hRead) {
	BOOL isSuccess = FALSE;
	char str[100] = {0};
	DWORD dwRead;
	while (true) {
		isSuccess = ReadFile(hRead, str, 43, &dwRead, NULL);
		if (isSuccess) {
			break;
		}
	}
	CloseHandle(hRead);
	cout << "从管道中读取内容如下：\n" << str << endl;
	return isSuccess;
}

int main() {
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	STARTUPINFO sui;
	ZeroMemory(&sui, sizeof(STARTUPINFO));
	sui.cb = sizeof(STARTUPINFO);
	sui.dwFlags = STARTF_USESTDHANDLES;
	HANDLE hParentWrite, hParentRead, hChildRead, hChildWrite;
	if (!CreatePipe(&hChildRead, &hParentWrite, &sa, 0)) {
		cout << "父进程写管道创建失败" << endl << endl;
	}
	if (!CreatePipe(&hParentRead, &hChildWrite, &sa, 0)) {
		cout << "父进程读管道创建失败" << endl << endl;
	}
	sui.hStdOutput = hChildWrite;
	sui.hStdInput = hChildRead;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	BOOL isSuccess = FALSE;
	isSuccess = CreateProcess(".\\child.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi);
	if (!isSuccess) {
		cout << "子进程创建失败" << endl << endl;
	} else {
		cout << "子进程创建成功" << endl << endl;
		if (writePipe(hParentWrite)) {
			cout << "父进程向管道写入数据成功" << endl << endl;
		}
		if (readPipe(hParentRead)) {
			cout << "父进程读取管道数据成功" << endl << endl;
		}
		WaitForSingleObject(pi.hProcess, 10000);
		CloseHandle(pi.hProcess);
		CloseHandle(hParentRead);
		CloseHandle(hParentWrite);
		CloseHandle(hChildRead);
		CloseHandle(hChildWrite);
	}
	return 0;
}