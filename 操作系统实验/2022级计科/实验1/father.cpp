#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;

int main(){
    STARTUPINFO sui; //创建 STARTUPINEO
    ZeroMemory(&sui, sizeof(sui)); //将SUARUTPINFO初始化为0
    sui.cb=sizeof(STARTUPINFO); //将cb成 员设西 为 STARTUPINFO结构体的大小
    PROCESS_INFORMATION pi; //存储进程相关信息
    int sum =0;
    char content[100]=""; //初始化content字符数组用来存放文件内容
    if(CreateProcess(".\\child.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sui, &pi)){
        cout<<"子进程创建成功！\n"<<endl;
        for(int i=1;i<=100;i++){
            sum += i; //求1-100之和
            Sleep(5); //延迟时间5ms
        }
        cout << sum << endl;
        WaitForSingleObject(pi.hProcess,5000); //等待进程结束，直到5000ms终止
        FILE *fp = fopen(".\\test.txt","r");
        fread(content, sizeof(char), 100,fp); //读取文件内容
        cout << "文件内容：\n" << content << endl;
        fclose(fp);
    }else{
        cout << "创建子进程失败！\n" << endl;
    }
    cout<<"实验结束！"<<endl;
    return 0;
}
    
