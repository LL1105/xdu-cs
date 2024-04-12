#include <stdio.h>
#include <windows.h>
#include <iostream>
using namespace std;

int main(){
    cout << "子进程开始运行...\n" << endl;
    const char *sentence_1 = "不知江月待何人\n";
    const char *sentence_2 = "但见长江送流水\n";
    FILE *fp;
    //以可读写方式打开文件
    if(fp = fopen(".\\test.txt", "w+")){
        cout << "文件打开成功\n" << endl;
        fwrite(sentence_1, strlen(sentence_1), 1, fp); //将sentence_1写入文件
        fwrite(sentence_2, strlen(sentence_2), 1, fp); //将sentence_1写入文件
        cout << "写入数据成功\n" << endl;
        fclose(fp); //关闭文件
        fp = fopen("\\test.txt", "r") //以只读的方式打开文件
        cout << "当前文件中的内容如下：\n" << endl;
        char ch = fgetc(fp); //读取第一个字符
        while(ch!=EOF){ //当该字符不为文件结束字符时进入循环
            putchar(ch); //输出该字符
            ch = fgetc(fp); //继续从文件中获取字符
        }
        fclose(fp); //关闭文件
    }else{
        cout << "创建文件失败\n" << endl;
    }
    system("pause"); //暂停黑窗口
    return 0;
}