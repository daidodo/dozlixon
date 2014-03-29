char * int2addr(char * str, int num)
{
	char * p = str;
	char ch;
	int i;
	bool flag = false;
	*p++ = '0';
	*p++ = 'x';
	if(num == 0){
		*p++ = '0';
	}else{
	//	*p++ = 'D';
	//	*p++ = 'o';
	//	*p++ = 'z';
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
