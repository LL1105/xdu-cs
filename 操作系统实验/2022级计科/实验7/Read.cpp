// ShmRead.cpp
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>
using namespace std;

#define BUFFER 4096
#define PERM S_IRUSR|S_IWUSR

int main(int argc,char **argv)
{
    int shmid = shmget((key_t)123456, BUFFER, PERM);
    if (shmid == -1)
    {
        cout << "获取共享内存失败，失败原因如下：" << endl; 
        perror("shmget");
        exit(1);
    }else{
        cout << "获去共享内存成功" << endl << endl;
    }
    char* shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1)
    {
        cout << "加载到虚拟内存错误，失败原因如下：" << endl;
        perror("shmat");
        exit(1);
    }else{
        cout << "加载到虚拟内存成功" << endl << endl;
    }
    // 输出从另一个进程传过来的数据
    cout << "读取共享内存数据如下：" << endl;
    cout << shmaddr << endl << endl; 
    if (shmdt(shmaddr) == -1)
    {
        cout << "取消虚拟内存映射失败，失败原因如下：" << endl;
        perror("shmdt");
        exit(1);
    }else{
        cout << "取消虚拟内存映射成功" << endl << endl;

    }
    return 0;
}
