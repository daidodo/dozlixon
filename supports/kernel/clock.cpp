#include "global.h"
#include "kernellibrary.h"
#include "system.h"
#include "clock.h"

//struct Clock
//ʱ���жϴ������
void Clock::int_handler(int irq)
{
	++GlobalVariables::ticks;
	if(GlobalVariables::k_reenter != 0)		//�����ж�����
		return;
	System::schedule();
}
//��ʼ��ʱ���ж�
void Clock::init()
{
	/* ��ʼ�� 8253 PIT */
	KernelLibrary::out_byte(GlobalConstant::TIMER_MODE, GlobalConstant::RATE_GENERATOR);
	KernelLibrary::out_byte(GlobalConstant::TIMER0, __U8(GlobalConstant::TIMER_FREQ / GlobalConstant::HZ) );
	KernelLibrary::out_byte(GlobalConstant::TIMER0, __U8((GlobalConstant::TIMER_FREQ / GlobalConstant::HZ) >> 8));
	/* ��ʼ�� 8253 PIT ��� */

	I8259::set_irq_handler(GlobalConstant::CLOCK_IRQ, int_handler);	//�趨ʱ���жϴ������
}
