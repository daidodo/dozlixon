#ifndef	_DOLIXON_CONST_H_
#define	_DOLIXON_CONST_H_

#include "protect.h"

//全局常量
struct GlobalConstant{
	//Color
	/*
	* e.g.	MAKE_COLOR(BLUE, RED)
	*	MAKE_COLOR(BLACK, RED) | BRIGHT
	*	MAKE_COLOR(BLACK, RED) | BRIGHT | FLASH
	*/
	static const __U8 BLACK		= 0x0;	/* 0000 */
	static const __U8 WHITE		= 0x7;	/* 0111 */
	static const __U8 RED		= 0x4;	/* 0100 */
	static const __U8 GREEN		= 0x2;	/* 0010 */
	static const __U8 BLUE		= 0x1;	/* 0001 */
	static const __U8 FLASH		= 0x80;	/* 1000 0000 */
	static const __U8 BRIGHT	= 0x08;	/* 0000 1000 */
	static __U8 MakeColor(__U8 frground,__U8 bkground){return (bkground << 4 | frground);}	/* MAKE_COLOR(Background,Foreground) */

	//GDT 和 IDT 中描述符的个数
	static const int GDT_SIZE = 128;
	static const int IDT_SIZE = 256;
	static const int LDT_SIZE = 2;	// 每个任务有一个单独的 LDT, 每个 LDT 中的描述符个数

	// 权限
	static const __U8 PRIVILEGE_KRNL = 0;
	static const __U8 PRIVILEGE_TASK = 1;
	static const __U8 PRIVILEGE_USER = 3;
	//RPL
	static const __U32 RPL_KRNL = Protection::SA_RPL0;
	static const __U32 RPL_TASK	= Protection::SA_RPL1;
	static const __U32 RPL_USER	= Protection::SA_RPL3;

	//TTY
	static const int NR_MAX_CONSOLES	= 3;	/* max number of tty and consoles */

	//8259 INTERRUPT CONTROLLER PORTS
	static const __U32 INT_M_CTL		= 0x20;	//I/O port for interrupt controller	<Master>
	static const __U32 INT_M_CTLMASK	= 0x21;	//setting bits in this port disables ints	<Master>
	static const __U32 INT_S_CTL		= 0xA0;	//I/O prot for second interrupt controller	<Slave>
	static const __U32 INT_S_CTLMASK	= 0xA1;	//setting bits in this port disables ints	<Slave>

	/* 8253/8254 PIT (Programmable Interval Timer) */
	static const __U32 TIMER0			= 0x40;		/* I/O port for timer channel 0 */
	static const __U32 TIMER_MODE		= 0x43;		/* I/O port for timer mode control */
	static const __U8  RATE_GENERATOR	= 0x34;		/* 00-11-010-0 : Counter0 - LSB then MSB - rate generator - binary */
	static const __U32 TIMER_FREQ		= 1193182;	/* clock frequency for timer in PC and AT */
	static const __U32 HZ				= 1000;		/* clock freq (software settable on IBM-PC) */

	/* AT keyboard */
	/* 8042 ports */
	static const __Port KB_DATA	= 0x60;	/*	I/O port for keyboard data
											Read : Read Output Buffer 
											Write: Write Input Buffer(8042 Data&8048 Command) */
	static const __Port KB_CMD	= 0x64;	/*	I/O port for keyboard command
											Read : Read Status Register
											Write: Write Input Buffer(8042 Command) */
	static const __U8 LED_CODE	= 0xED;
	static const __U8 KB_ACK	= 0xED;
	
	/* VGA */
	static const __Port CRTC_ADDR_REG				= 0x3D4;	/* CRT Controller Registers - Address Register */
	static const __Port CRTC_DATA_REG				= 0x3D5;	/* CRT Controller Registers - Data Registers */
	static const __U8 CRTC_DATA_IDX_START_ADDR_H	= 0xC;		/* register index of video mem start address (MSB) */
	static const __U8 CRTC_DATA_IDX_START_ADDR_L	= 0xD;		/* register index of video mem start address (LSB) */
	static const __U8 CRTC_DATA_IDX_CURSOR_H		= 0xE;		/* register index of cursor position (MSB) */
	static const __U8 CRTC_DATA_IDX_CURSOR_L		= 0xF;		/* register index of cursor position (LSB) */
	static const __U32 V_MEM_BASE					= 0xB8000;	/* base of color video memory */
	static const __U32 V_MEM_SIZE					= 0x8000;	/* 32K: B8000H -> BFFFFH */

	// Hardware interrupts
	static const int NR_IRQ			= 16;	/* Number of IRQs */
	static const int CLOCK_IRQ		= 0;
	static const int KEYBOARD_IRQ	= 1;
	static const int CASCADE_IRQ	= 2;	/* cascade enable for 2nd AT controller */
	static const int ETHER_IRQ		= 3;	/* default ethernet interrupt vector */
	static const int SECONDARY_IRQ	= 3;	/* RS232 interrupt vector for port 2 */
	static const int RS232_IRQ		= 4;	/* RS232 interrupt vector for port 1 */
	static const int XT_WINI_IRQ	= 5;	/* xt winchester */
	static const int FLOPPY_IRQ		= 6;	/* floppy disk */
	static const int PRINTER_IRQ	= 7;
	static const int AT_WINI_IRQ	= 14;	/* at winchester */

	//进程
	static const int NR_MAX_SYS_TASKS	= 5;				//最大系统任务数目
	static const int NR_MAX_USR_TASKS	= 5;				//最大用户进程数目
	static const int NR_ALL_TASKS		= NR_MAX_SYS_TASKS + NR_MAX_USR_TASKS;	//任务总数
	static const int DEFAULT_STACK_SIZE = 0x8000;	//默认堆栈长度
	static const int STACK_SIZE_TOTAL	= NR_ALL_TASKS * DEFAULT_STACK_SIZE;

	//System Call
	static const int NR_SYS_CALL	= 2;	//系统调用总数
	static const int _NR_GET_TICKS	= 0;
	static const int _NR_WRITE		= 1;
};

#endif
