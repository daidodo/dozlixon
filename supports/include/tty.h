#ifndef _DOZLIXON_TTY_H_
#define _DOZLIXON_TTY_H_

#include "const.h"
#include "console.h"
#include "BufferTemplate.h"

//终端
class TTY
{
public:
	static const int TTY_IN_BYTES = 256;	/* tty input queue size */
	static void init();
	static void task();		//task interface of tty
private:
	static void init_screen();	//初始化每个TTY及其console
private:
	void process_key_in(__U32 key);	//处理按键输入
	void readKeyboard();
	void flushKeyboard();
	struct TTYBuffer
	{
		__U32	in_buf[TTY_IN_BYTES];	/* TTY 输入缓冲区 */
		__U32 *	p_inbuf_head;			/* 指向缓冲区中下一个空闲位置 */
		__U32 *	p_inbuf_tail;			/* 指向键盘任务应处理的键值 */
		int		inbuf_count;			/* 缓冲区中已经填充了多少 */
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
	BufferTemplate<__U32,TTY_IN_BYTES> tty_in_buf;		/* TTY 输入缓冲区 */
public:
	Console console;
	void writeConsole(const char * buf, int len);
};

#endif
