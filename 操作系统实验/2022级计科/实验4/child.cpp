#include <iostream>
#include <windows.h>
using namespace std;

FILE *fp;

BOOL writePipe(HANDLE hWrite) {
	BOOL isSuccess = FALSE;
	char *str = "HELLO FATHER";
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
	if (fp) {
		fwrite(str, strlen(str), 1, fp);
	}
	return isSuccess;
}

int main() {
	fp = fopen(".\\child.txt", "w");
	char *str = "�ӽ��̿�ʼִ��\n\n";
	if (fp) {
		fwrite(str, strlen(str), 1, fp);
	}
	HANDLE hRead = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hWrite = GetStdHandle(STD_OUTPUT_HANDLE);
	if (readPipe(hRead)) {
		str = "�ӽ��̶�ȡ���ݽ���\n\n";
		fwrite(str, strlen(str), 1, fp);
	}
	if (writePipe(hWrite)) {
		str = "�ӽ���д��ܵ��ɹ�\n\n";
		fwrite(str, strlen(str), 1, fp);
	}
	str = "�ӽ���ִ�н���\n\n";
	fwrite(str, strlen(str), 1, fp);
	fclose(fp);
	return 0;
}