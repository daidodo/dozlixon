#include "kernellibrary.h"
#include "global.h"

//struct I8259
void (* I8259::spurious_irq)(int irq) = spurious_irq_ASM_FN;

void I8259::init_8259A()
{
	KernelLibrary::out_byte(GlobalConstant::INT_M_CTL,	0x11);			// Master 8259, ICW1.
	KernelLibrary::out_byte(GlobalConstant::INT_S_CTL,	0x11);			// Slave  8259, ICW1.
	KernelLibrary::out_byte(GlobalConstant::INT_M_CTLMASK,	Protection::INT_VECTOR_IRQ0);	// Master 8259, ICW2. ���� '��8259' ���ж���ڵ�ַΪ 0x20.
	KernelLibrary::out_byte(GlobalConstant::INT_S_CTLMASK,	Protection::INT_VECTOR_IRQ8);	// Slave  8259, ICW2. ���� '��8259' ���ж���ڵ�ַΪ 0x28
	KernelLibrary::out_byte(GlobalConstant::INT_M_CTLMASK,	0x4);			// Master 8259, ICW3. IR2 ��Ӧ '��8259'.
	KernelLibrary::out_byte(GlobalConstant::INT_S_CTLMASK,	0x2);			// Slave  8259, ICW3. ��Ӧ '��8259' �� IR2.
	KernelLibrary::out_byte(GlobalConstant::INT_M_CTLMASK,	0x1);			// Master 8259, ICW4.
	KernelLibrary::out_byte(GlobalConstant::INT_S_CTLMASK,	0x1);			// Slave  8259, ICW4.

	KernelLibrary::out_byte(GlobalConstant::INT_M_CTLMASK,	0xFF);	// Master 8259, OCW1. 
	KernelLibrary::out_byte(GlobalConstant::INT_S_CTLMASK,	0xFF);	// Slave  8259, OCW1. 

	//��ʼ��GlobalVariables::irq_table
	for(int i = 0;i < GlobalConstant::NR_IRQ;++i)
		GlobalVariables::irq_table[i] = spurious_irq;
}
void I8259::set_irq_handler(int irq, __PF_IRQHandler handler)
{
	KernelLibrary::disable_irq(irq);
	GlobalVariables::irq_table[irq] = handler;
	KernelLibrary::enable_irq(irq);			//��8259A���Խ����ж�
}


