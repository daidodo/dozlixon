#include "global.h"
#include "kernellibrary.h"
#include "console.h"

//struct Console
void Console::set_cursor(__U32 position)
{
	//设置光标位置
	KernelLibrary::disable_int();
	KernelLibrary::out_byte(GlobalConstant::CRTC_ADDR_REG,
		GlobalConstant::CRTC_DATA_IDX_CURSOR_H);
	KernelLibrary::out_byte(GlobalConstant::CRTC_DATA_REG,
		(position >> 8) & 0xFF);
	KernelLibrary::out_byte(GlobalConstant::CRTC_ADDR_REG,
		GlobalConstant::CRTC_DATA_IDX_CURSOR_L);
	KernelLibrary::out_byte(GlobalConstant::CRTC_DATA_REG,
		position & 0xFF);
	KernelLibrary::enable_int();
}
void Console::set_video_start_addr(__U32 addr)
{
	KernelLibrary::disable_int();
	KernelLibrary::out_byte(GlobalConstant::CRTC_ADDR_REG,
		GlobalConstant::CRTC_DATA_IDX_START_ADDR_H);
	KernelLibrary::out_byte(GlobalConstant::CRTC_DATA_REG,
		(addr >> 8) & 0xFF);
	KernelLibrary::out_byte(GlobalConstant::CRTC_ADDR_REG,
		GlobalConstant::CRTC_DATA_IDX_START_ADDR_L);
	KernelLibrary::out_byte(GlobalConstant::CRTC_DATA_REG,
		addr & 0xFF);
	KernelLibrary::enable_int();
}
void Console::out_char(char ch)
{
	if(!ch)
		return;
	__U8 * p_vmem = (__U8 *)(GlobalConstant::V_MEM_BASE + cursor * 2);
	switch(ch){
		case '\n':{
			if (cursor < original_addr + v_mem_limit - SCREEN_WIDTH)
				cursor += SCREEN_WIDTH - (cursor - original_addr) % SCREEN_WIDTH;
			break;}
		case '\b':{
			if (cursor > original_addr){
				--cursor;
				*(p_vmem - 2) = ' ';
				*(p_vmem - 1) = DEFAULT_CHAR_COLOR;
			}
			break;}
		default:{
			if (cursor < original_addr + v_mem_limit - 1){
				*p_vmem++ = ch;
				*p_vmem++ = DEFAULT_CHAR_COLOR;
				++cursor;
			}
		}
	}
	while(cursor >= current_start_addr + SCREEN_SIZE)
		scroll_screen(SCROLL_SCREEN_DOWN);
	flush();
}
inline void Console::set_cursor() const
{
	set_cursor(cursor);
}
void Console::flush(__Bool unconditional) const
{
	if(unconditional || isCurrentConsole()){
		set_cursor();
		set_video_start_addr(current_start_addr);
	}
}
void Console::scroll_screen(int direction)
{
	if (direction == SCROLL_SCREEN_UP){				//向上滚屏
		if (current_start_addr > original_addr)
			current_start_addr -= SCREEN_WIDTH;
	}else if (direction == SCROLL_SCREEN_DOWN){		//向下滚屏
		if (current_start_addr + SCREEN_SIZE < original_addr + v_mem_limit)
			current_start_addr += SCREEN_WIDTH;
	}else{											//其他暂不处理
	}
	flush();
}
__Bool Console::isCurrentConsole() const
{
	return this == &GlobalVariables::tty_table[GlobalVariables::current_console].console;
}
