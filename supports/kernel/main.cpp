#include "kernellibrary.h"
#include "usrlibrary.h"

void TestA()	//���Խ���A
{
	for(int i = 0;;++i){
		UserLibrary::printf("A%x\n",i);
		KernelLibrary::delay(1000);
	}
}

void TestB()	//���Խ���B
{
	for(int i = 0;;++i){
		UserLibrary::printf("B%x\n",i);
		KernelLibrary::delay(1000);
	}
}

void TestC()	//���Խ���C
{
	for(int i = 0;;++i){
		UserLibrary::printf("B%x\n",i);
		KernelLibrary::delay(1000);
	}
}

void TestD()	//���Խ���D
{
	while(1){
		UserLibrary::printf("<Ticks:%x>\n",SystemCall::get_ticks());
		KernelLibrary::delay(1000);
	}
}
