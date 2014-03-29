#include "const.h"
#include "kernellibrary.h"
#include "keyboard.h"
#include "keymap.h"
#include "tty.h"

//struct Keyboard
BufferTemplate<char,Keyboard::KB_IN_BYTES> Keyboard::kb_in;
//LEDs�ĳ�ʼ״̬
__U8 Keyboard::Caps_Lock	= 0;
__U8 Keyboard::Number_Lock	= 0;
__U8 Keyboard::Scroll_Lock	= 0;

//�����жϴ������
void Keyboard::int_handler(int irq)		//������ring0��Ȩ����
{
	__U8 scan_code = KernelLibrary::in_byte(GlobalConstant::KB_DATA);
	if(kb_in.count() < kb_in.SIZE)
		kb_in.pushItem(scan_code);
}
//��ʼ������
void Keyboard::init()
{
	setLEDs();
	//��ʼ�������ж�
	I8259::set_irq_handler(GlobalConstant::KEYBOARD_IRQ, int_handler);	/* �趨�����жϴ������ ring0 */
//	KernelLibrary::enable_irq(GlobalConstant::KEYBOARD_IRQ);			/* �������ж� */
}

__U32 Keyboard::read()
{
	static __Bool code_with_E0	= false;
	static __Bool shift_l		= false;	/* l shift state	*/
	static __Bool shift_r		= false;	/* r shift state	*/
	static __Bool alt_l			= false;	/* l alt state		*/
	static __Bool alt_r			= false;	/* r left state		*/
	static __Bool ctrl_l		= false;	/* l ctrl state		*/
	static __Bool ctrl_r		= false;	/* l ctrl state		*/
	static int    column		= 0;		/* keyrow[column] ���� keymap ��ĳһ��ֵ */
	
	__U32 key = 0;	/* ��һ����������ʾһ������ */
	if(kb_in.count() > 0){
		code_with_E0 = false;
		__U8 scan_code = read_char_from_buf();
						/* ���磬��� Home �����£��� key ֵ��Ϊ������ keyboard.h �е� 'HOME'��*/
		__Bool	make;	/* TRUE : make  */
						/* FALSE: break */
		//����ɨ����
		if(scan_code == 0xE1){
			const int SIZE = 6;
			const __U8 pausebreak_code[SIZE] = {0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5};
			key = PAUSEBREAK;
			for(int i = 1;i < SIZE;++i){
				if(read_char_from_buf() != pausebreak_code[i]){
					key = 0;
					break;
				}
			}
		}else if(scan_code == 0xE0){
			code_with_E0 = true;
			scan_code = read_char_from_buf();
			//PrintScreen ������
			if(scan_code == 0x2A){
				code_with_E0 = false;
				if((scan_code = read_char_from_buf()) == 0xE0){
					code_with_E0 = true;
					if((scan_code = read_char_from_buf()) == 0x37){
						key = PRINTSCREEN;
						make = true;
					}
				}
			}
			//PrintScreen ���ͷ�
			if(scan_code == 0xB7){
				code_with_E0 = false;
				if((scan_code = read_char_from_buf()) == 0xE0){
					code_with_E0 = true;
					if((scan_code = read_char_from_buf()) == 0xAA){
						key = PRINTSCREEN;
						make = false;
					}
				}
			}
		}
		//���� PrintScreen����ʱ scan_code Ϊ 0xE0 �������Ǹ�ֵ��
		if(key != PAUSEBREAK && key != PRINTSCREEN){	//�ɴ�ӡ�ַ�
			//�ж��� Make Code ���� Break Code
			make = !(scan_code & FLAG_BREAK);
			//��λ�� keymap �е���
			const __U32 * keyrow = KeyMap::MAPPED_KEY + (scan_code & 0x7F) * MAP_COLS;
			//��λ�� keymap �е���
			column = 0;
			if(shift_l || shift_r)
				column = 1;
			if (code_with_E0)
				column = 2;
			key = keyrow[column];

			switch(key) {
				case SHIFT_L:
					shift_l	= make;
					break;
				case SHIFT_R:
					shift_r	= make;
					break;
				case CTRL_L:
					ctrl_l	= make;
					break;
				case CTRL_R:
					ctrl_r	= make;
					break;
				case ALT_L:
					alt_l	= make;
					break;
				case ALT_R:
					alt_l	= make;
					break;
				case CAPS_LOCK:
					if(make){
						Caps_Lock = !Caps_Lock;
						setLEDs();
					}
					break;
				case NUM_LOCK:
					if(make){
						Number_Lock = !Number_Lock;
						setLEDs();
					}
					break;
				case SCROLL_LOCK:
					if(make){
						Scroll_Lock = !Scroll_Lock;
						setLEDs();
					}
					break;
				default:
					break;
			}
		}
		if(make){	//���� Break Code
			__Bool pad = false;
			/* ���ȴ���С���� */
			if(key >= PAD_SLASH && key <= PAD_9){
				pad = true;
				switch(key){
					case PAD_SLASH:
						key = '/';
						break;
					case PAD_STAR:
						key = '*';
						break;
					case PAD_MINUS:
						key = '-';
						break;
					case PAD_PLUS:
						key = '+';
						break;
					case PAD_ENTER:
						key = ENTER;
						break;
					default:{	/* keys whose value depends on the NumLock */
						if(Number_Lock){
							if(key == PAD_DOT)
								key = '.';
							else if(key >= PAD_0 && key <= PAD_9)
								key += '0' - PAD_0;
						}else{
							switch(key) {
								case PAD_HOME:
									key = HOME;
									break;
								case PAD_END:
									key = END;
									break;
								case PAD_PAGEUP:
									key = PAGEUP;
									break;
								case PAD_PAGEDOWN:
									key = PAGEDOWN;
									break;
								case PAD_INS:
									key = INSERT;
									break;
								case PAD_UP:
									key = UP;
									break;
								case PAD_DOWN:
									key = DOWN;
									break;
								case PAD_LEFT:
									key = LEFT;
									break;
								case PAD_RIGHT:
									key = RIGHT;
									break;
								case PAD_DOT:
									key = DELETE;
									break;
								default:;
							}
						}
					}
				}
			}



			key |= (shift_l	? FLAG_SHIFT_L	: 0);
			key |= (shift_r	? FLAG_SHIFT_R	: 0);
			key |= (ctrl_l	? FLAG_CTRL_L	: 0);
			key |= (ctrl_r	? FLAG_CTRL_R	: 0);
			key |= (alt_l	? FLAG_ALT_L	: 0);
			key |= (alt_r	? FLAG_ALT_R	: 0);
			key |= (pad		? FLAG_PAD		: 0);
		}else{		//���� Break Code
			key = 0;
		}
	}
	return key;
}

__U8 Keyboard::read_char_from_buf()
{
	while(kb_in.count() <= 0){}	/* �ȴ���һ���ֽڵ��� */
	KernelLibrary::disable_int();
	__U8 scan_code = kb_in.popItem();
	KernelLibrary::enable_int();
	return scan_code;
}

void Keyboard::kb_wait()
{
	__U8 kb_stat;
	do{
		kb_stat = KernelLibrary::in_byte(GlobalConstant::KB_CMD);
	}while(0x02 & kb_stat);
//	while(0x02 & KernelLibrary::in_byte(GlobalConstant::KB_CMD)){}
}
void Keyboard::kb_ack()
{	/* ����ѭ��
	__U8 kb_read;
	do{
		kb_read = KernelLibrary::in_byte(GlobalConstant::KB_DATA);
	}while(GlobalConstant::KB_ACK != kb_read);	//*/
//	while(GlobalConstant::KB_ACK != KernelLibrary::in_byte(GlobalConstant::KB_DATA)){}
}
void Keyboard::setLEDs()
{
	kb_wait();
	KernelLibrary::out_byte(GlobalConstant::KB_DATA, GlobalConstant::LED_CODE);
	kb_ack();

	__U8 leds = (Caps_Lock << 2) | (Number_Lock << 1) | Scroll_Lock;
	kb_wait();
	KernelLibrary::out_byte(GlobalConstant::KB_DATA, leds);
	kb_ack();
}
