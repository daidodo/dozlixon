#ifndef _DOZLIXON_SYSTEM_H_
#define _DOZLIXON_SYSTEM_H_

#include "type.h"

struct System
{
	//�ն�����
	static __PF_Task task_tty;
	//���̵���
	static void schedule();
};

#endif
