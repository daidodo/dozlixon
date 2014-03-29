#include "kernellibrary.h"
#include "usrlibrary.h"
#include "global.h"

//struct Process
void Process::init(const Task & task,__U32 id,__U16 selector_ldt,__U8 privilege,__U8 rpl,const char * const p_task_stack,__U32 eflags,int prio)
{
	UserLibrary::strcpy(p_name,task.name);	/* name of the process */
	pid = id;							/* pid */
	ldt_sel	= selector_ldt;
	KernelLibrary::memcpy(&ldts[0], &GlobalVariables::gdt[Protection::SELECTOR_KERNEL_CS >> 3], sizeof(Descriptor));
	ldts[0].attr1 = Protection::DA_C | privilege << 5;		// change the DPL
	KernelLibrary::memcpy(&ldts[1], &GlobalVariables::gdt[Protection::SELECTOR_KERNEL_DS >> 3], sizeof(Descriptor));
	ldts[1].attr1 = Protection::DA_DRW | privilege << 5;	// change the DPL
	regs.cs		= ((8 * 0) & Protection::SA_RPL_MASK & Protection::SA_TI_MASK) | Protection::SA_TIL | rpl;
	regs.ds		= ((8 * 1) & Protection::SA_RPL_MASK & Protection::SA_TI_MASK) | Protection::SA_TIL | rpl;
	regs.es		= ((8 * 1) & Protection::SA_RPL_MASK & Protection::SA_TI_MASK) | Protection::SA_TIL | rpl;
	regs.fs		= ((8 * 1) & Protection::SA_RPL_MASK & Protection::SA_TI_MASK) | Protection::SA_TIL | rpl;
	regs.ss		= ((8 * 1) & Protection::SA_RPL_MASK & Protection::SA_TI_MASK) | Protection::SA_TIL | rpl;
	regs.gs		= (Protection::SELECTOR_KERNEL_GS & Protection::SA_RPL_MASK) | rpl;
	regs.eip	= (__U32)task.initial_eip;
	regs.esp	= (__U32)p_task_stack;
	regs.eflags	= eflags;
	ticks = priority = prio;
}

//struct Task
Task::Task(){}
Task::Task(__PF_Task funcPtr, const char * tskname, int stcksz)
	:initial_eip(funcPtr)
	,stacksize(stcksz)
{
	UserLibrary::strcpy(name, tskname);
}

//struct TaskManager
int TaskManager::num_sys_tasks = 0;
int TaskManager::num_usr_tasks = 0;
int TaskManager::taskCount()
{
	return num_sys_tasks + num_usr_tasks;
}
//	Register a SYSTEM TASK, shuould be called before GlobalVariables::init_proc_table() is invoked
void TaskManager::registerSysTask(const Task & task)
{
	if(num_sys_tasks < GlobalConstant::NR_MAX_SYS_TASKS){
		GlobalVariables::sys_task_table[num_sys_tasks] = task;
		++num_sys_tasks;
	}
}
//	Register a USER TASK, shuould be called before GlobalVariables::init_proc_table() is invoked
void TaskManager::registerUsrTask(const Task & task)
{
	if(num_usr_tasks < GlobalConstant::NR_MAX_USR_TASKS){
		GlobalVariables::usr_task_table[num_usr_tasks] = task;
		++num_usr_tasks;
	}
}
