#include "kernellibrary.h"
#include "usrlibrary.h"
#include "global.h"
#include "system.h"
#include "keyboard.h"
#include "clock.h"

//extern "C"
void cstart_ASM_FN()
{
	KernelLibrary::disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\ninitializing GDT and IDT                ");
	GlobalVariables::init_gdt();
	GlobalVariables::init_idt();
	Protection::init_prot();
	KernelLibrary::disp_str_DONE();
}

//compromise
void __gxx_personality_v0(){}

//异常处理
void exception_handler_ASM_FN(int vec_no, int err_code, int eip, int cs, int eflags)
{
	const int text_color = 0x74;
	const char * const err_description[64] = {
		"#DE Divide Error",
		"#DB RESERVED",
		"—  NMI Interrupt",
		"#BP Breakpoint",
		"#OF Overflow",
		"#BR BOUND Range Exceeded",
		"#UD Invalid Opcode (Undefined Opcode)",
		"#NM Device Not Available (No Math Coprocessor)",
		"#DF Double Fault",
		"    Coprocessor Segment Overrun (reserved)",
		"#TS Invalid TSS",
		"#NP Segment Not Present",
		"#SS Stack-Segment Fault",
		"#GP General Protection",
		"#PF Page Fault",
		"—  (Intel reserved. Do not use.)",
		"#MF x87 FPU Floating-Point Error (Math Fault)",
		"#AC Alignment Check",
		"#MC Machine Check",
		"#XF SIMD Floating-Point Exception"
	};
	// 通过打印空格的方式清空屏幕的前五行，并把 GlobalVariables::cursor_position 清零
	GlobalVariables::cursor_position = 0;
	for(int i = 0;i < 20 * 5;++i){
		KernelLibrary::disp_str("    ");
	}
	GlobalVariables::cursor_position = 0;

	KernelLibrary::disp_color_str("Exception! --> ", text_color);
	KernelLibrary::disp_color_str(err_description[vec_no], text_color);
	KernelLibrary::disp_color_str("\n\n", text_color);
	KernelLibrary::disp_color_str("EFLAGS:", text_color);
	KernelLibrary::disp_int(eflags);
	KernelLibrary::disp_color_str("CS:", text_color);
	KernelLibrary::disp_int(cs);
	KernelLibrary::disp_color_str("EIP:", text_color);
	KernelLibrary::disp_int(eip);

	if(err_code != 0xFFFFFFFF){
		KernelLibrary::disp_color_str("Error code:", text_color);
		KernelLibrary::disp_int(err_code);
	}
}

void spurious_irq_ASM_FN(int irq)
{
	KernelLibrary::disp_str("spurious_irq_ASM_FN: ");
	KernelLibrary::disp_int(irq);
	KernelLibrary::disp_str("\n");
}

void DozLixon_main_ASM_FN()
{
	//register SYSTEM TASKs，运行于ring1特权级下
	KernelLibrary::disp_str("registering system task \"task_tty\"      "); 
	TaskManager::registerSysTask(Task(System::task_tty, "task_tty"));
	KernelLibrary::disp_str_DONE();
	//register USER TASKs，运行于ring3特权级下
	KernelLibrary::disp_str("registering user tasks                  "); 
//	TaskManager::registerUsrTask(Task(TestA, "TestA"));
	TaskManager::registerUsrTask(Task(TestB, "TestB"));
//	TaskManager::registerUsrTask(Task(TestC, "TestC"));
	TaskManager::registerUsrTask(Task(TestD, "TestD"));
	KernelLibrary::disp_str_DONE();

	KernelLibrary::disp_str("initializing process table              "); 
	GlobalVariables::init_proc_table();
	GlobalVariables::k_reenter = 0;

	//设定进程的输出终端
	GlobalVariables::proc_table[1].nr_tty = 1;
	GlobalVariables::proc_table[2].nr_tty = 2 ;
	GlobalVariables::proc_table[3].nr_tty = 2;
	GlobalVariables::proc_table[4].nr_tty = 0;
	KernelLibrary::disp_str_DONE();

	KernelLibrary::disp_str("initializing clock interruption         "); 
	Clock::init();		//时钟中断
	KernelLibrary::disp_str_DONE();
	KernelLibrary::disp_str("initializing keyboard interruption      "); 
	Keyboard::init();	//键盘中断
	KernelLibrary::disp_str_DONE();
	KernelLibrary::disp_str("initializing tty environment            "); 
	TTY::init();		//从此我们可以用UserLibrary::printf了
	KernelLibrary::disp_str_DONE();
	
	//显示欢迎信息
	const int WELCOME_COLOR = GlobalConstant::MakeColor(GlobalConstant::BLUE,GlobalConstant::WHITE)
		| GlobalConstant::FLASH;
	KernelLibrary::disp_str("\n------------------------------");
	KernelLibrary::disp_color_str("Welcome to DozLixon",WELCOME_COLOR);
	KernelLibrary::disp_str("------------------------------\n");
	/* Now go to the assembly code to start running the current process. */
	restart_ASM_FN();
	while(1){}
}
