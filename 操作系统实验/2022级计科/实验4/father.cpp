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
	cout << "�ӹܵ��ж�ȡ�������£�\n" << str << endl;
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
		cout << "������д�ܵ�����ʧ��" << endl << endl;
	}
	if (!CreatePipe(&hParentRead, &hChildWrite, &sa, 0)) {
		cout << "�����̶��ܵ�����ʧ��" << endl << endl;
	}
	sui.hStdOutput = hChildWrite;
	sui.hStdInput = hChildRead;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	BOOL isSuccess = FALSE;
	isSuccess = CreateProcess(".\\child.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi);
	if (!isSuccess) {
		cout << "�ӽ��̴���ʧ��" << endl << endl;
	} else {
		cout << "�ӽ��̴����ɹ�" << endl << endl;
		if (writePipe(hParentWrite)) {
			cout << "��������ܵ�д�����ݳɹ�" << endl << endl;
		}
		if (readPipe(hParentRead)) {
			cout << "�����̶�ȡ�ܵ����ݳɹ�" << endl << endl;
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