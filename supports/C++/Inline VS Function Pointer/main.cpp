#include <ctime>
#include <iostream>
#include "Inline.h"

using namespace std;

int main()
{
	const int times = 1000000;

	for(int j = 0;j < 20;++j){
		double start,end;
		start = clock();
		for(int i = 0;i < times;++i){
			CInline::inl(i,i + 1);
		}
		end = clock();
		cout<<"inline Time: "<<(end - start)<<endl;

		start = clock();
		for(int i = 0;i < times;++i){
			CInline::fnptr(i,i + 1);
		}
		end = clock();
		cout<<"function pointer Time: "<<(end - start)<<endl;
	}

}