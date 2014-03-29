#include "global.h"
#include "keyboard.h"
#include "tty.h"
#include "system.h"

//struct System

//�ն�����
__PF_Task System::task_tty = TTY::task;
//���̵����㷨
void System::schedule()
{
	//*///��ת����
	if(++GlobalVariables::p_proc_ready >= GlobalVariables::proc_table + TaskManager::taskCount()){
		GlobalVariables::p_proc_ready = GlobalVariables::proc_table;
	}
	/*///���ȼ�����
	--GlobalVariables::p_proc_ready->ticks;
	if(GlobalVariables::p_proc_ready->ticks > 0)
		return;
	int greatest_ticks = 0;
	do{
		for (Process * p = GlobalVariables::proc_table; p < GlobalVariables::proc_table + TaskManager::taskCount();++p) {
			if (p->ticks > greatest_ticks) {
				greatest_ticks = p->ticks;
				GlobalVariables::p_proc_ready = p;
			}
		}

		if (!greatest_ticks) {
			for (Process * p = GlobalVariables::proc_table;p < GlobalVariables::proc_table + TaskManager::taskCount();++p) {
				p->ticks = p->priority;
			}
		}
	}while(!greatest_ticks);
	//*/
}

