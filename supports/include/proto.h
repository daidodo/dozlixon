#ifndef	_DOLIXON_PROTO_H_
#define	_DOLIXON_PROTO_H_

#include "type.h"

extern "C"{
	//start.cpp
	void cstart_ASM_FN();
	void __gxx_personality_v0();	//compromise to gcc compiler ?????????????????????????????
	void exception_handler_ASM_FN(int vec_no, int err_code, int eip, int cs, int eflags);
	void spurious_irq_ASM_FN(int irq);
	void DozLixon_main_ASM_FN();

	//kernel.asm
	void restart_ASM_FN();

	/* 中断处理函数 */
	void divide_error();
	void single_step_exception();
	void nmi();
	void breakpoint_exception();
	void overflow();
	void bounds_check();
	void inval_opcode();
	void copr_not_available();
	void double_fault();
	void copr_seg_overrun();
	void inval_tss();
	void segment_not_present();
	void stack_exception();
	void general_protection();
	void page_fault();
	void copr_error();
	void hwint00();
	void hwint01();
	void hwint02();
	void hwint03();
	void hwint04();
	void hwint05();
	void hwint06();
	void hwint07();
	void hwint08();
	void hwint09();
	void hwint10();
	void hwint11();
	void hwint12();
	void hwint13();
	void hwint14();
	void hwint15();
}

//i8259.cpp
struct I8259
{
	static void (* spurious_irq)(int irq);
	static void init_8259A();
	static void set_irq_handler(int irq, __PF_IRQHandler handler);
};

//main.cpp
void TestA();
void TestB();
void TestC();
void TestD();

//proc.cpp
int sys_get_ticks();

#endif
