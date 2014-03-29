#include <iostream>
#include "FileIO.h"

using namespace std;

int main(int argc,char ** argv){
	if(argc < 3){
		cout<<"need source or dest files\n";
		return 1;
	}
	FileIO in(argv[1]),out(argv[2]);
	long inSize = in.buf.size();
	long outSize = out.buf.size();
	/*
	cout<<inSize<<" "<<outSize<<"\n";
	for(int i = 0;i < inSize;i++)
		cout<<hex<<outBuf[i]<<' ';
	cout<<"\n\n";
	for(int i = outSize - 100;i < outSize;i++)
		cout<<hex<<outBuf[i]<<' ';

	/*/
	if(inSize >= outSize)
		out.buf = in.buf;
	else
		for(long i = 0;i < inSize;++i)
			out.buf[i] = in.buf[i];
	out.Write();
	cout<<"write file successful\n";
	//*/
	return 0;
}
