#include <iostream>
#include <windows.h>
using namespace std;

int main(){
    STARTUPINFO sui;
    ZeroMemory(&sui, sizeof(sui));
    sui.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION pi;
    if(!CreateProcess(".\\child.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sui, &pi)){
        cout << "进程创建失败" << endl;
    }else{
        cout << "这是一个子进程" << endl;
    }
    HANDLE hEvent = CreateEvent(NULL, FALSE, TRUE, "myEvent");
    ResetEvent(hEvnet);
    DWORD flag = WaitForSingleObject(hEvent, 10000);
    if(flag == WAIT_FAILED){
        cout << "等待事件信号失败" << endl;
    }else if(flag == WAIT_OBJECT_0){
        cout << "获得事件信号" << endl;
    }else if(flag == WAIT_TIMEOUT){
        cout << "等待子进程信号超时" << endl;
    }
    return 0;
}