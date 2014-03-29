#include "global.h"
#include "kernellibrary.h"
#include "system.h"
#include "clock.h"

//struct Clock
//时钟中断处理程序
void Clock::int_handler(int irq)
{
	++GlobalVariables::ticks;
	if(GlobalVariables::k_reenter != 0)		//发生中断重入
		return;
	System::schedule();
}
//初始化时钟中断
void Clock::init()
{
	/* 初始化 8253 PIT */
	KernelLibrary::out_byte(GlobalConstant::TIMER_MODE, GlobalConstant::RATE_GENERATOR);
	KernelLibrary::out_byte(GlobalConstant::TIMER0, __U8(GlobalConstant::TIMER_FREQ / GlobalConstant::HZ) );
	KernelLibrary::out_byte(GlobalConstant::TIMER0, __U8((GlobalConstant::TIMER_FREQ / GlobalConstant::HZ) >> 8));
	/* 初始化 8253 PIT 完毕 */

	I8259::set_irq_handler(GlobalConstant::CLOCK_IRQ, int_handler);	//设定时钟中断处理程序
}
