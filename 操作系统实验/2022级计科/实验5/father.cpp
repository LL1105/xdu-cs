#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

int main() {
	int err;
	bool rc;
	HANDLE hPipeHandle1;
	char inBuffer[50] = "";
	char outBuffer[50] = "";
	DWORD bytesRead, bytesWrite;

	hPipeHandle1 = CreateNamedPipe("\\\\.\\pipe\\myPipe",
	                               PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED | WRITE_DAC,
	                               PIPE_TYPE_MESSAGE | PIPE_READMODE_BYTE | PIPE_WAIT,
	                               1, 20, 30,
	                               NMPWAIT_USE_DEFAULT_WAIT,
	                               (LPSECURITY_ATTRIBUTES)NULL);

	if ((hPipeHandle1 == INVALID_HANDLE_VALUE) || (hPipeHandle1 == NULL)) {
		err = GetLastError();
		cout << "�����ܵ�ʧ�ܣ�err = " << err << endl << endl;
		exit(1);
	} else {
		cout << "�����ܵ��ɹ�����ʼ�ܵ�ͨ�ţ�����end����" << endl << endl;
	}

	while (true) {
		rc = ConnectNamedPipe(hPipeHandle1, NULL);
		if (rc == 0) {
			err = GetLastError();
			cout << "�ܵ�����ʧ��, err = " << err << endl << endl;
			exit(1);
		} else {
			cout << "�ܵ����ӳɹ�" << endl << endl;
		}
		strcpy(inBuffer, "");
		strcpy(outBuffer, "");
		rc = ReadFile(hPipeHandle1, inBuffer, sizeof(inBuffer), &bytesRead, NULL);
		if (rc == 0 && bytesRead == 0) {
			err = GetLastError();
			cout << "�ܵ���ȡʧ��, err = " << err << endl << endl;
			exit(2);
		} else {
			rc = strcmp(inBuffer, "end");
			if (rc == 0) {
				break;
			}
			cout << "�ܵ���ȡ�ɹ�����������:" << endl;
			cout << inBuffer << endl << endl;
		}

		cout << "������Ҫ���͵�����" << endl;
		cin >> outBuffer;
		rc = WriteFile(hPipeHandle1, outBuffer, sizeof(outBuffer), &bytesWrite, NULL);
		if (rc == 0) {
			cout << "�ܵ�д��ʧ��" << endl << endl;
		} else {
			cout << "�ܵ�д��ɹ�" << endl << endl;
		}
		DisconnectNamedPipe(hPipeHandle1);
		rc = strcmp(outBuffer, "end");
		if (rc == 0) {
			break;
		}
	}
	CloseHandle(hPipeHandle1);
	cout << "�ܵ��ѹر�" << endl << endl;
	system("pause");
	return 0;
}