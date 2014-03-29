#ifndef _DOZLIXON_GLOBAL_H_
#define _DOZLIXON_GLOBAL_H_

#include "proc.h"
#include "tty.h"

struct GlobalVariables
{
	//extern "C"
	static int &				cursor_position;
	static __U8	*				gdt_ptr;
	static __U8	*				idt_ptr;
	static Process *&			p_proc_ready;
	static TSS &				tss;
	static __U32 &				k_reenter;
	static __PF_IRQHandler *	irq_table;
	static int					ticks;
	//C++
	static Process		proc_table[GlobalConstant::NR_ALL_TASKS];
	static Task			sys_task_table[GlobalConstant::NR_MAX_SYS_TASKS];	//系统任务
	static Task			usr_task_table[GlobalConstant::NR_MAX_USR_TASKS];	//用户进程
	static char			task_stack[GlobalConstant::STACK_SIZE_TOTAL];
	static Descriptor	gdt[GlobalConstant::GDT_SIZE];
	static Gate			idt[GlobalConstant::IDT_SIZE];
	static TTY			tty_table[GlobalConstant::NR_MAX_CONSOLES];
	static int			current_console;
	//functions
	static void init_gdt();
	static void init_idt();
	static void init_proc_table();
	static void select_console(int index);
};

#endif
