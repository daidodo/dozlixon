#include "kernellibrary.h"
#include "usrlibrary.h"

extern "C"{
	//string.asm
	char * strcpy_ASM_FN(char* p_dst, const char* p_src);
}
//struct UserLibrary
char * (* UserLibrary::strcpy)(char* p_dst, const char* p_src) = strcpy_ASM_FN;
int UserLibrary::strlen(const char * str)
{
	int i = 0;
	while(*str++)
		++i;
	return i;
}
char * UserLibrary::itoa(char * str, int num)
{
	char * p = str;
	char ch;
	int i;
	__Bool flag = false;
	*p++ = '0';
	*p++ = 'x';
	if(num == 0){
		*p++ = '0';
	}else{
		for(i = 28;i >= 0;i -= 4){
			ch = (num >> i) & 0xF;
			if(flag || ch > 0){
				flag = true;
				ch += '0';
				if(ch > '9')
					ch += 7;
				*p++ = ch;
			}
		}
	}
	*p = 0;
	return str;
}
int UserLibrary::printf(const char * fmt,...)
{
	int i;
	char buf[256];

	va_list arg = (va_list)((char*)(&fmt) + 4);        /* 4 是参数 fmt 所占堆栈中的大小 */
	i = vsprintf(buf, fmt, arg);
	SystemCall::write(buf, i);
	return i;
}
int UserLibrary::vsprintf(char * buf, const char * fmt, va_list args)
{
	char tmp[256];
	char * p = buf;
	for(va_list	p_next_arg = args;*fmt;++fmt) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		}
		++fmt;
		switch(*fmt) {
			case 'x':
				itoa(tmp, *((int*)p_next_arg));
				strcpy(p, tmp);
				p_next_arg += 4;
				p += strlen(tmp);
				break;
			case 's':
				break;
			default:
				break;
		}
	}
	return (p - buf);
}
