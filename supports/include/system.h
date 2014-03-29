#ifndef _DOZLIXON_SYSTEM_H_
#define _DOZLIXON_SYSTEM_H_

#include "type.h"

struct System
{
	//终端任务
	static __PF_Task task_tty;
	//进程调度
	static void schedule();
};

#endif
