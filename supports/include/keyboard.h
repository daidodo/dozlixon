#ifndef _DOZLIXON_KEYBOARD_H_
#define _DOZLIXON_KEYBOARD_H_

#include "BufferTemplate.h"

struct Keyboard
{
/************************************************************************/
/*                         Constant Declaration                         */
/************************************************************************/
	static const int KB_IN_BYTES	= 32;			/* size of keyboard input buffer */
	static const int MAP_COLS		= 3;			/* Number of columns in keymap */
	static const int NR_SCAN_CODES	= 0x80;			/* Number of scan codes (rows in keymap) */

	static const int FLAG_BREAK		= 0x0080;		/* Break Code			*/
	static const int FLAG_EXT		= 0x0100;		/* Normal function keys	*/
	static const int FLAG_SHIFT_L	= 0x0200;		/* Shift key			*/
	static const int FLAG_SHIFT_R	= 0x0400;		/* Shift key			*/
	static const int FLAG_CTRL_L	= 0x0800;		/* Control key			*/
	static const int FLAG_CTRL_R	= 0x1000;		/* Control key			*/
	static const int FLAG_ALT_L		= 0x2000;		/* Alternate key		*/
	static const int FLAG_ALT_R		= 0x4000;		/* Alternate key		*/
	static const int FLAG_PAD		= 0x8000;		/* keys in num pad		*/

	static const int MASK_RAW		= 0x01FF;		/*	raw key value = code passed to tty & MASK_RAW
														the value can be found either in the keymap column 0
														or in the list below */
/* Special keys */
	static const int ESC		= (0x01 + FLAG_EXT);	/* Esc		*/
	static const int TAB		= (0x02 + FLAG_EXT);	/* Tab		*/
	static const int ENTER		= (0x03 + FLAG_EXT);	/* Enter	*/
	static const int BACKSPACE	= (0x04 + FLAG_EXT);	/* BackSpace*/

	static const int GUI_L		= (0x05 + FLAG_EXT);	/* L GUI	*/
	static const int GUI_R		= (0x06 + FLAG_EXT);	/* R GUI	*/
	static const int APPS		= (0x07 + FLAG_EXT);	/* APPS		*/

/* Shift, Ctrl, Alt */
	static const int SHIFT_L	= (0x08 + FLAG_EXT);	/* L Shift	*/
	static const int SHIFT_R	= (0x09 + FLAG_EXT);	/* R Shift	*/
	static const int CTRL_L		= (0x0A + FLAG_EXT);	/* L Ctrl	*/
	static const int CTRL_R		= (0x0B + FLAG_EXT);	/* R Ctrl	*/
	static const int ALT_L		= (0x0C + FLAG_EXT);	/* L Alt	*/
	static const int ALT_R		= (0x0D + FLAG_EXT);	/* R Alt	*/

/* Lock keys */
	static const int CAPS_LOCK		= (0x0E + FLAG_EXT);	/* Caps Lock	*/
	static const int NUM_LOCK		= (0x0F + FLAG_EXT);	/* Number Lock	*/
	static const int SCROLL_LOCK	= (0x10 + FLAG_EXT);	/* Scroll Lock	*/

/* Function keys */
	static const int F1		= (0x11 + FLAG_EXT);	/* F1		*/
	static const int F2		= (0x12 + FLAG_EXT);	/* F2		*/
	static const int F3		= (0x13 + FLAG_EXT);	/* F3		*/
	static const int F4		= (0x14 + FLAG_EXT);	/* F4		*/
	static const int F5		= (0x15 + FLAG_EXT);	/* F5		*/
	static const int F6		= (0x16 + FLAG_EXT);	/* F6		*/
	static const int F7		= (0x17 + FLAG_EXT);	/* F7		*/
	static const int F8		= (0x18 + FLAG_EXT);	/* F8		*/
	static const int F9		= (0x19 + FLAG_EXT);	/* F9		*/
	static const int F10	= (0x1A + FLAG_EXT);	/* F10		*/
	static const int F11	= (0x1B + FLAG_EXT);	/* F11		*/
	static const int F12	= (0x1C + FLAG_EXT);	/* F12		*/

/* Control Pad */
	static const int PRINTSCREEN	= (0x1D + FLAG_EXT);	/* Print Screen	*/
	static const int PAUSEBREAK		= (0x1E + FLAG_EXT);	/* Pause/Break	*/
	static const int INSERT			= (0x1F + FLAG_EXT);	/* Insert	*/
	static const int DELETE			= (0x20 + FLAG_EXT);	/* Delete	*/
	static const int HOME			= (0x21 + FLAG_EXT);	/* Home		*/
	static const int END			= (0x22 + FLAG_EXT);	/* End		*/
	static const int PAGEUP			= (0x23 + FLAG_EXT);	/* Page Up	*/
	static const int PAGEDOWN		= (0x24 + FLAG_EXT);	/* Page Down*/
	static const int UP				= (0x25 + FLAG_EXT);	/* Up		*/
	static const int DOWN			= (0x26 + FLAG_EXT);	/* Down		*/
	static const int LEFT			= (0x27 + FLAG_EXT);	/* Left		*/
	static const int RIGHT			= (0x28 + FLAG_EXT);	/* Right	*/

/* ACPI keys */
	static const int POWER		= (0x29 + FLAG_EXT);	/* Power	*/
	static const int SLEEP		= (0x2A + FLAG_EXT);	/* Sleep	*/
	static const int WAKE		= (0x2B + FLAG_EXT);	/* Wake Up	*/

/* Num Pad */
	static const int PAD_SLASH	= (0x2C + FLAG_EXT);	/* /		*/
	static const int PAD_STAR	= (0x2D + FLAG_EXT);	/* *		*/
	static const int PAD_MINUS	= (0x2E + FLAG_EXT);	/* -		*/
	static const int PAD_PLUS	= (0x2F + FLAG_EXT);	/* +		*/
	static const int PAD_ENTER	= (0x30 + FLAG_EXT);	/* Enter	*/
	static const int PAD_DOT	= (0x31 + FLAG_EXT);	/* .		*/
	static const int PAD_0		= (0x32 + FLAG_EXT);	/* 0		*/
	static const int PAD_1		= (0x33 + FLAG_EXT);	/* 1		*/
	static const int PAD_2		= (0x34 + FLAG_EXT);	/* 2		*/
	static const int PAD_3		= (0x35 + FLAG_EXT);	/* 3		*/
	static const int PAD_4		= (0x36 + FLAG_EXT);	/* 4		*/
	static const int PAD_5		= (0x37 + FLAG_EXT);	/* 5		*/
	static const int PAD_6		= (0x38 + FLAG_EXT);	/* 6		*/
	static const int PAD_7		= (0x39 + FLAG_EXT);	/* 7		*/
	static const int PAD_8		= (0x3A + FLAG_EXT);	/* 8		*/
	static const int PAD_9		= (0x3B + FLAG_EXT);	/* 9		*/
	static const int PAD_UP			= PAD_8;			/* Up		*/
	static const int PAD_DOWN		= PAD_2;			/* Down		*/
	static const int PAD_LEFT		= PAD_4;			/* Left		*/
	static const int PAD_RIGHT		= PAD_6;			/* Right	*/
	static const int PAD_HOME		= PAD_7;			/* Home		*/
	static const int PAD_END		= PAD_1;			/* End		*/
	static const int PAD_PAGEUP		= PAD_9;			/* Page Up	*/
	static const int PAD_PAGEDOWN	= PAD_3;			/* Page Down	*/
	static const int PAD_INS		= PAD_0;			/* Ins			*/
	static const int PAD_MID		= PAD_5;			/* Middle key	*/
	static const int PAD_DEL		= PAD_DOT;			/* Del			*/
	
	//functions and fields
	static void int_handler(int irq);
	static void init();
	static __U32 read();
private:
	static __U8 read_char_from_buf();
	static BufferTemplate<char,KB_IN_BYTES> kb_in;	/* Keyboard buffer, 1 per console. */
	//LEDsµÄ×´Ì¬
	static __U8 Caps_Lock;		//0:Ï¨Ãð£»1:µãÁÁ
	static __U8 Number_Lock;
	static __U8 Scroll_Lock;
	static void kb_wait();		/* µÈ´ý 8042 µÄÊäÈë»º³åÇø¿Õ */
	static void kb_ack();
	static void setLEDs();
	/* Keymap for US MF-2 keyboard. */
	struct KeyMap;
};

#endif
