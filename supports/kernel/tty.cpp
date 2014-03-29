#include "kernellibrary.h"
#include "global.h"
#include "keyboard.h"
#include "tty.h"

//struct TTY
void TTY::init()
{
//	Keyboard::init();	//�����ж�
	init_screen();
}
void TTY::task()	//������ring1��Ȩ����
{
//	init_screen();
	//����console 0�Ĺ��λ�ú���ʾλ��
	Console & r_console = GlobalVariables::tty_table[0].console;
	r_console.cursor = GlobalVariables::cursor_position / 2;
	r_console.current_start_addr = KernelLibrary::video_current_start_addr;
	GlobalVariables::cursor_position  = 0;	//������2������û������
	KernelLibrary::video_current_start_addr = 0;
	//�л���console 0,����ʼ����
	GlobalVariables::select_console(0);	//set current console index, from 0 to GlobalConstant::NR_MAX_CONSOLES - 1
	while(1){	//only current tty can read keyboard and write console
		TTY & r_tty = GlobalVariables::tty_table[GlobalVariables::current_console];
		r_tty.readKeyboard();
		r_tty.flushKeyboard();
	}
}
void TTY::init_screen()
{
	__U32 v_mem_size = GlobalConstant::V_MEM_SIZE >> 1;	/* �Դ��ܴ�С (WORD) */
	__U32 con_v_mem_size = v_mem_size / GlobalConstant::NR_MAX_CONSOLES;	/* ÿ������̨ռ���Դ��С (WORD) */
	for(int i = 0;i < GlobalConstant::NR_MAX_CONSOLES;++i){
		Console & r_console = GlobalVariables::tty_table[i].console;
		r_console.original_addr			= i * con_v_mem_size;		/* �˿���̨ռ���Դ濪ʼ��ַ	(WORD)		*/
		r_console.v_mem_limit			= con_v_mem_size;			/* �˿���̨ռ���Դ��С (WORD)			*/
		r_console.current_start_addr	= r_console.original_addr;	/* �˿���̨��ʾ�����Դ��ʲôλ�� (WORD)*/
		r_console.cursor				= r_console.original_addr;	/* Ĭ�Ϲ��λ�����ʼ��				*/
		if(i){
			//��ʾ�ն˺�
			r_console.out_char(i + '0');
			r_console.out_char('#');
		}
	}
}
void TTY::process_key_in(__U32 key)
{
	if(!key)
		return;
	if(!(key & Keyboard::FLAG_EXT)){	//�ɴ�ӡ�ַ�
		tty_in_buf.pushItem(key);
	}else{
		int raw_code = key & Keyboard::MASK_RAW;
		switch(raw_code){
			case Keyboard::ENTER:{
				tty_in_buf.pushItem('\n');
				break;}
			case Keyboard::BACKSPACE:{
				tty_in_buf.pushItem('\b');
				break;}
			case Keyboard::UP:{
				if((key & Keyboard::FLAG_SHIFT_L) || (key & Keyboard::FLAG_SHIFT_R)){
					//Shift + Up
					console.scroll_screen(Console::SCROLL_SCREEN_UP);
				}
				break;}
			case Keyboard::DOWN:{
				if((key & Keyboard::FLAG_SHIFT_L) || (key & Keyboard::FLAG_SHIFT_R)){
					//Shift + Down
					console.scroll_screen(Console::SCROLL_SCREEN_DOWN);
				}
				break;}
			case Keyboard::F1:
			case Keyboard::F2:
			case Keyboard::F3:
			case Keyboard::F4:
			case Keyboard::F5:
			case Keyboard::F6:
			case Keyboard::F7:
			case Keyboard::F8:
			case Keyboard::F9:
			case Keyboard::F10:
			case Keyboard::F11:
			case Keyboard::F12:{			/* Alt + F1~F12 */
				if (key & Keyboard::FLAG_ALT_L || key & Keyboard::FLAG_ALT_R){
					GlobalVariables::select_console(raw_code - Keyboard::F1);
				}
				break;}
			default:;
		}
	}
}
void TTY::readKeyboard()
{
	__U32 key = Keyboard::read();
	process_key_in(key);
}
void TTY::flushKeyboard()
{
	__U32 key = tty_in_buf.popItem();
	console.out_char(key & 0xFF);
}
void TTY::writeConsole(const char * buf, int len)
{
	for(int i = 0;i < len;++i)
		console.out_char(buf[i]);
}
