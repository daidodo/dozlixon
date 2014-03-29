#include "kernellibrary.h"
#include "usrlibrary.h"

void TestA()	//测试进程A
{
	for(int i = 0;;++i){
		UserLibrary::printf("A%x\n",i);
		KernelLibrary::delay(1000);
	}
}

void TestB()	//测试进程B
{
	for(int i = 0;;++i){
		UserLibrary::printf("B%x\n",i);
		KernelLibrary::delay(1000);
	}
}

void TestC()	//测试进程C
{
	for(int i = 0;;++i){
		UserLibrary::printf("B%x\n",i);
		KernelLibrary::delay(1000);
	}
}

void TestD()	//测试进程D
{
	while(1){
		UserLibrary::printf("<Ticks:%x>\n",SystemCall::get_ticks());
		KernelLibrary::delay(1000);
	}
}
