#include <cstdio>
#include <vector>
#include <string>
#include <cassert>

class FileIO
{
public:
	typedef std::vector<int> __BufType;
	__BufType buf;
	FileIO(const char * name):file(name){Read();}
	~FileIO(){}
	void Read(){
		buf.clear();
		FILE * fp = fopen(file.c_str(),"rb");
		if(fp == 0)
			throw "can't open file";
		while(!feof(fp)){
			buf.push_back(fgetc(fp));
		}
		fclose(fp);
		buf.pop_back();
	}
	void Write() const{
		FILE * fp = fopen(file.c_str(),"wb");
		if(fp == 0)
			throw "can't open file";
		for(__BufType::const_iterator i = buf.begin();i != buf.end();++i)
			fputc(*i,fp);
		fclose(fp);
	}
private:
	std::string file;
};
