#include <iostream>
#include "int2addr.h"

using namespace std;

int main(){
	char str[16];
	int num = 16;
	int2addr(str,num);
	cout<<str<<endl;
	cout<<strlen("123456789")<<endl;
}
