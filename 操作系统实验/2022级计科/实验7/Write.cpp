// ShmWrite.cpp
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
using namespace std;

#define BUFFER 4096
#define PERM S_IRUSR|S_IWUSR // 表示文件所有者有读权限和写权限

int main(int argc, char *argv[])
{
    // 创建一个新的共享内存段或者取得一个已有共享内存段的标识符
    int shmid = shmget((key_t)123456, BUFFER, 
    					PERM | IPC_CREAT);
    if (shmid == -1)
    {
        cout << "创建或获取共享内存失败，失败原因如下：" << endl; 
        perror("shmget");
        exit(1);
    }else{
        cout << "创建或获去共享内存成功" << endl << endl;
    }
    // 将共享内存段加载到调用进程的虚拟地址空间中
    char* shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1)
    {
        cout << "加载到虚拟内存错误，失败原因如下：" << endl;
        perror ("shmat");
        exit(1);
    }else{
        cout << "加载到虚拟内存成功" << endl << endl;
    }
    while(true){
        int flag;
        cout << "请输入您的选择[1/2], 1表示向共享内存写入数据,0表示退出程序并删除共享内存" << endl;
        cin >> flag;
        if(flag == 1){
            cout << "请输入要写入的数据：" << endl;
            cin >> shmaddr;
        }else if(flag == 2){
            break;
        }else{
            cout << "无效的输入，请重新输入" << endl;
        }
    }
    cout << endl;
    // 取消进程的虚拟内存映射共享内存段
    if (shmdt(shmaddr) == -1)
    {
        cout << "取消虚拟内存映射失败，失败原因如下：" << endl;
        perror("shmdt");
        exit(1);
    }else{
        cout << "取消虚拟内存映射成功" << endl << endl;
    }
    // 删除这块共享内存
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        cout << "删除共享内存失败，失败原因如下：" << endl;
        perror("shmctl");
        exit(1);
    }else{
        cout << "删除共享内存成功" << endl << endl;
    }
    return 0;
}
