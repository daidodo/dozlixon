#ifndef _DOZLIXON_PROC_H_
#define _DOZLIXON_PROC_H_

#include "const.h"

struct StackFrame{
	__U32
		gs,
		fs,
		es,
		ds,
		edi,
		esi,
		ebp,
		kernel_esp,
		ebx,
		edx,
		ecx,
		eax,
		retaddr,
		eip,
		cs,
		eflags,
		esp,
		ss;
};

struct Task{
	__PF_Task	initial_eip;
	int	stacksize;
	char name[32];
	Task();
	Task(__PF_Task funcPtr, const char * tskname = "no name", int stcksz = GlobalConstant::DEFAULT_STACK_SIZE);
};

struct TaskManager
{
	static int num_sys_tasks;		//当前运行的系统任务数
	static int num_usr_tasks;		//当前运行的用户任务数
	static int taskCount();			//当前运行的任务总数
//	Register a SYSTEM TASK, shuould be called before GlobalVariables::init_proc_table() is invoked
	static void registerSysTask(const Task & task);
//	Register a USER TASK, shuould be called before GlobalVariables::init_proc_table() is invoked
	static void registerUsrTask(const Task & task);
};

struct Process{
	StackFrame regs;
	__U16 ldt_sel;
	Descriptor ldts[GlobalConstant::LDT_SIZE];
	__U32 pid;
	char p_name[16];
	int ticks;
	int priority;
	int nr_tty;		//tty number,from 0 to (GlobalConstant::NR_MAX_CONSOLES - 1)
	void init(const Task & task,__U32 id,__U16 selector_ldt,__U8 privilege,__U8 rpl,const char * const p_task_stack,__U32 eflags,int prio);
};


#endif
