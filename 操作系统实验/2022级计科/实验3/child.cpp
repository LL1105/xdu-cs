#include <iostream>
#include <windows.h>
using namespace std;

int main(){
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS,TRUE,"myEvent");
    Sleep(1000);
    char ch;
    cout << "Signal the event to Parent[y/n]" << endl;
    cin >> ch;
    if(ch == 'y'){
        SetEvent(hEvent);
    }
    Sleep(1000);
    system("pause");
    return 0;
}