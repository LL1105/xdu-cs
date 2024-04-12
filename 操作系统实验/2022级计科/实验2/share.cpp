#include <iostream>
#include <stdio.h>
#include <windows.h>
using namespace std;
static int count;

DWORD WINAPI ThreadProc(LPVOID lpParameter){
    cout << "新线程运行\n" << endl;
    for(count; count <= 10; count+=2){
        cout << "线程：count = " << count << endl;
    }
    cout << "等待线程3秒" << endl << endl;
    Sleep(3000);
    return 0;
}

int main(){
    count = 0;
    cout << "count 初始值为：" << count << endl << endl;
    HANDLE hEevent = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    WaitForSingleObject(hEvent, 5000);
    CloseHandle(hEvent);
    cout << "新线程结束\n" << endl;
    cout << "now, count = " << count << endl << endl;
    cout << "进程结束\n" << endl;
    return 0;
}