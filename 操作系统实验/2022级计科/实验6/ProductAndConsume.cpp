#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <iostream>
using namespace std;

sem_t *full, *empty0, *mutex;

void *createSharedMemory(size_t size) {
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    if (addr == MAP_FAILED) {
        cout << "调用共享内存失败" << endl << endl;
        return NULL;
    }
    return addr;
}

int main(){
	full = (sem_t*)createSharedMemory(sizeof(sem_t));
	empty0 = (sem_t*)createSharedMemory(sizeof(sem_t));
	mutex = (sem_t*)createSharedMemory(sizeof(sem_t));
	if (full == NULL || empty0 == NULL || mutex == NULL) {
        cout << "程序调用共享内存失败，程序即将退出" << endl << endl;
        return 0;
    }
	sem_init(full, 1, 0);
	sem_init(empty0, 1, 3); 
	sem_init(mutex, 1, 1);
	pid_t pid1 = fork();
	if (pid1 > 0) {
		while(1) {
			//生产 
			sem_wait(empty0);
			sem_wait(mutex);
			sem_post(full);
			int value;
			sem_getvalue(full, &value);
            cout << "生产者进程(pid=" << getpid() << "):" << "生产了一个产品，产品数量：" << value << endl << endl;
			sem_post(mutex);
			sleep(2);
		}
	} else if (pid1 == 0) {
			//消费 
			while(1){
				sleep(2);
				sem_wait(full);
				sem_wait(mutex);
				int value;
				sem_getvalue(full, &value);
                cout << "消费者进程(pid=" << getpid() << "):" << "消费了一个产品，还剩" << value << "个产品" << endl << endl;
				sem_post(mutex);
				sem_post(empty0);
				sleep(2);
			}
		}
	return 0;	
}
