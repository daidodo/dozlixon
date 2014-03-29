#ifndef _DOZLIXON_CONSOLE_H_
#define _DOZLIXON_CONSOLE_H_

#include "type.h"

//CONSOLE
struct Console
{
	static const __U8 DEFAULT_CHAR_COLOR	= 0x07;	/* 0000 0111 黑底白字 */
	static const int SCROLL_SCREEN_UP		= 0;	/* scroll forward */
	static const int SCROLL_SCREEN_DOWN		= 1;	/* scroll backward*/
	static const int SCREEN_WIDTH			= 80;
	static const int SCREEN_SIZE			= SCREEN_WIDTH * 25;
	//functions
	static void set_cursor(__U32 position);
	static void set_video_start_addr(__U32 addr);
	__U32 current_start_addr;	/* 当前显示到了什么位置   */
	__U32 original_addr;		/* 当前控制台对应显存位置 */
	__U32 v_mem_limit;			/* 当前控制台占的显存大小 */
	__U32 cursor;				/* 当前光标位置 */
	int id;
	void out_char(char ch);
	void set_cursor() const;
	void flush(__Bool unconditional = false) const;		//如果unconditional为true，则无条件更新屏幕显示为当前Console
	void scroll_screen(int direction);
private:
	__Bool isCurrentConsole() const;

};

#endif
