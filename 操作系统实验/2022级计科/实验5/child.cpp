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
		cout << "������Ҫ���͵����ݣ�����end������" << endl;
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
			cout << "�ȴ��ܵ�ʧ�ܣ�err = " << err << endl << endl;
			exit(1);
		} else {
			cout << "�ȴ��ܵ��ɹ������ڵȴ�..." << endl << endl;
		}
		rc = CallNamedPipe("\\\\.\\pipe\\myPipe", outBuffer, sizeof(outBuffer), inBuffer, sizeof(inBuffer),
		                   &bytesRead, NMPWAIT_USE_DEFAULT_WAIT);
		if (rc == 0) {
			err = GetLastError();
			cout << "�ܵ�����ʧ�ܣ�err = " << err << endl << endl;
			exit(1);
		} else {
			rc = strcmp(inBuffer, "end");
			if (rc == 0) {
				break;
			}
			cout << "�ܵ����ӳɹ�����ȡ�ܵ��������£�" << endl;
			cout << inBuffer << endl << endl;
		}
	}
	cout << "�����ܵ�ͨ��" << endl << endl;
	system("pause");
	return 0;
}