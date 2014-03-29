#ifndef _DOZLIXON_TTY_H_
#define _DOZLIXON_TTY_H_

#include "const.h"
#include "console.h"
#include "BufferTemplate.h"

//�ն�
class TTY
{
public:
	static const int TTY_IN_BYTES = 256;	/* tty input queue size */
	static void init();
	static void task();		//task interface of tty
private:
	static void init_screen();	//��ʼ��ÿ��TTY����console
private:
	void process_key_in(__U32 key);	//����������
	void readKeyboard();
	void flushKeyboard();
	struct TTYBuffer
	{
		__U32	in_buf[TTY_IN_BYTES];	/* TTY ���뻺���� */
		__U32 *	p_inbuf_head;			/* ָ�򻺳�������һ������λ�� */
		__U32 *	p_inbuf_tail;			/* ָ���������Ӧ����ļ�ֵ */
		int		inbuf_count;			/* ���������Ѿ�����˶��� */
		TTYBuffer()
			:p_inbuf_head(in_buf)
			,p_inbuf_tail(in_buf)
			,inbuf_count(0)
		{}
		void PushKey(__U32 key){
			if(inbuf_count < TTY_IN_BYTES){
				*(p_inbuf_head++) = key;
				if(p_inbuf_head == in_buf + TTY_IN_BYTES)
					p_inbuf_head = in_buf;
				++inbuf_count;
			}
		}
		__U32 popItem(){
			if(!inbuf_count)
				return 0;
			__U32 key = *(p_inbuf_tail++);
			if(p_inbuf_tail == in_buf + TTY_IN_BYTES)
				p_inbuf_tail = in_buf;
			--inbuf_count;
			return key;
		}
	};
	BufferTemplate<__U32,TTY_IN_BYTES> tty_in_buf;		/* TTY ���뻺���� */
public:
	Console console;
	void writeConsole(const char * buf, int len);
};

#endif
