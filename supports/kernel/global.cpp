#include "kernellibrary.h"
#include "system.h"
#include "global.h"

//extern "C"
extern "C"{
	int				disp_pos_ASM;
	__U8			gdt_ptr_ASM[6];
	__U8			idt_ptr_ASM[6];
	Process *		p_proc_ready_ASM;
	TSS				tss_ASM;
	__U32			k_reenter_ASM;
	__PF_IRQHandler	irq_table_ASM[GlobalConstant::NR_IRQ];
	void *		sys_call_table[GlobalConstant::NR_SYS_CALL] = {
		(void *)KernelLibrary::sys_get_ticks,
		(void *)KernelLibrary::sys_write,
	};
}

//struct GlobalVariables
int &				GlobalVariables::cursor_position = disp_pos_ASM;
__U8 *				GlobalVariables::gdt_ptr = gdt_ptr_ASM;
__U8 *				GlobalVariables::idt_ptr = idt_ptr_ASM;
Process *&			GlobalVariables::p_proc_ready = p_proc_ready_ASM;
TSS &				GlobalVariables::tss = tss_ASM;
__U32 &				GlobalVariables::k_reenter = k_reenter_ASM;
__PF_IRQHandler *	GlobalVariables::irq_table = irq_table_ASM;
int					GlobalVariables::ticks = 0;
//C++
Process		GlobalVariables::proc_table[GlobalConstant::NR_ALL_TASKS];
Task		GlobalVariables::sys_task_table[GlobalConstant::NR_MAX_SYS_TASKS];
Task		GlobalVariables::usr_task_table[GlobalConstant::NR_MAX_USR_TASKS];
char		GlobalVariables::task_stack[GlobalConstant::STACK_SIZE_TOTAL];
Descriptor	GlobalVariables::gdt[GlobalConstant::GDT_SIZE];
Gate		GlobalVariables::idt[GlobalConstant::IDT_SIZE];
TTY			GlobalVariables::tty_table[GlobalConstant::NR_MAX_CONSOLES];
int			GlobalVariables::current_console = 0;

void GlobalVariables::init_gdt()
{
	// 将 LOADER 中的 GDT 复制到新的 GDT 中
	KernelLibrary::memcpy(&gdt,					// New GDT
		(void*)(*((__U32*)(&gdt_ptr[2]))),	// Base  of Old GDT
		*((__U16*)(gdt_ptr)) + 1		// Limit of Old GDT
		);
	// gdt_ptr[6] 共 6 个字节：0~15:Limit  16~47:Base。用作 sgdt 以及 lgdt 的参数。
	__U16* p_gdt_limit = (__U16*)(&gdt_ptr[0]);
	__U32* p_gdt_base  = (__U32*)(&gdt_ptr[2]);
	*p_gdt_limit = GlobalConstant::GDT_SIZE * sizeof(Descriptor) - 1;
	*p_gdt_base  = (__U32)&gdt;
}
void GlobalVariables::init_idt()
{
	// idt_ptr[6] 共 6 个字节：0~15:Limit  16~47:Base。用作 sidt 以及 lidt 的参数。
	__U16* p_idt_limit = (__U16*)(&idt_ptr[0]);
	__U32* p_idt_base  = (__U32*)(&idt_ptr[2]);
	*p_idt_limit = GlobalConstant::IDT_SIZE * sizeof(Gate) - 1;
	*p_idt_base  = (__U32)&idt;
}
void GlobalVariables::init_proc_table()
{
	Process * p_proc = proc_table;
	char * p_task_stack = task_stack + GlobalConstant::STACK_SIZE_TOTAL;
	__U16 selector_ldt = Protection::SELECTOR_LDT_FIRST;
	int pid = 0;
	for(int i = 0;i < TaskManager::num_sys_tasks;++i){
		p_proc->init(
			sys_task_table[i],
			pid,
			selector_ldt,
			GlobalConstant::PRIVILEGE_TASK,
			GlobalConstant::RPL_TASK,
			p_task_stack,
			0x1202,
			100);
		p_task_stack -= sys_task_table[i].stacksize;
		selector_ldt += (1 << 3);
		++p_proc;
		++pid;
	}
	for(int i = 0;i < TaskManager::num_usr_tasks;++i){
		p_proc->init(
			usr_task_table[i],
			pid,
			selector_ldt,
			GlobalConstant::PRIVILEGE_USER,
			GlobalConstant::RPL_USER,
			p_task_stack,
			0x202,
			150);
		p_task_stack -= usr_task_table[i].stacksize;
		selector_ldt += (1 << 3);
		++p_proc;
		++pid;
	}
	p_proc_ready = proc_table;
}
//切换控制台
void GlobalVariables::select_console(int index)		//0 <= index <= GlobalConstant::NR_MAX_CONSOLES - 1
{
	if(index < 0 || index >= GlobalConstant::NR_MAX_CONSOLES)
		return;
	current_console = index;
	tty_table[index].console.flush();
}
