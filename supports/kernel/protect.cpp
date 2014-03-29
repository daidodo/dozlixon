#include "kernellibrary.h"
#include "global.h"

void Protection::init_prot()
{
	I8259::init_8259A();
	// 全部初始化成中断门(没有陷阱门)
	init_idt_desc(INT_VECTOR_DIVIDE,		DA_386IGate,	divide_error,			GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_DEBUG,			DA_386IGate,	single_step_exception,	GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_NMI,			DA_386IGate,	nmi,					GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_BREAKPOINT,	DA_386IGate,	breakpoint_exception,	GlobalConstant::PRIVILEGE_USER);
	init_idt_desc(INT_VECTOR_OVERFLOW,		DA_386IGate,	overflow,				GlobalConstant::PRIVILEGE_USER);
	init_idt_desc(INT_VECTOR_BOUNDS,		DA_386IGate,	bounds_check,			GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_INVAL_OP,		DA_386IGate,	inval_opcode,			GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_COPROC_NOT,	DA_386IGate,	copr_not_available,		GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_DOUBLE_FAULT,	DA_386IGate,	double_fault,			GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_COPROC_SEG,	DA_386IGate,	copr_seg_overrun,		GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_INVAL_TSS,		DA_386IGate,	inval_tss,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_SEG_NOT,		DA_386IGate,	segment_not_present,	GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_STACK_FAULT,	DA_386IGate,	stack_exception,		GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_PROTECTION,	DA_386IGate,	general_protection,		GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_PAGE_FAULT,	DA_386IGate,	page_fault,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_COPROC_ERR,	DA_386IGate,	copr_error,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ0 + 0,		DA_386IGate,	hwint00,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ0 + 1,		DA_386IGate,	hwint01,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ0 + 2,		DA_386IGate,	hwint02,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ0 + 3,		DA_386IGate,	hwint03,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ0 + 4,		DA_386IGate,	hwint04,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ0 + 5,		DA_386IGate,	hwint05,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ0 + 6,		DA_386IGate,	hwint06,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ0 + 7,		DA_386IGate,	hwint07,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ8 + 0,		DA_386IGate,	hwint08,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ8 + 1,		DA_386IGate,	hwint09,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ8 + 2,		DA_386IGate,	hwint10,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ8 + 3,		DA_386IGate,	hwint11,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ8 + 4,		DA_386IGate,	hwint12,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ8 + 5,		DA_386IGate,	hwint13,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ8 + 6,		DA_386IGate,	hwint14,				GlobalConstant::PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_IRQ8 + 7,		DA_386IGate,	hwint15,				GlobalConstant::PRIVILEGE_KRNL);
	//系统调用中断门
	init_idt_desc(INT_VECTOR_SYS_CALL,		DA_386IGate,	SystemCall::sys_call,	GlobalConstant::PRIVILEGE_USER);
	/* 填充 GDT 中 TSS 这个描述符 */
	KernelLibrary::memset(&GlobalVariables::tss, 0, sizeof(GlobalVariables::tss));
	GlobalVariables::tss.ss0		= SELECTOR_KERNEL_DS;
	init_descriptor(&GlobalVariables::gdt[INDEX_TSS],
		vir2phys(seg2phys(SELECTOR_KERNEL_DS), &GlobalVariables::tss),
		sizeof(GlobalVariables::tss) - 1,
		DA_386TSS);
	GlobalVariables::tss.iobase	= sizeof(GlobalVariables::tss);	/* 没有I/O许可位图 */

	// 填充 GDT 中进程的 LDT 的描述符
	int i;
	Process * p_proc = GlobalVariables::proc_table;
	__U16 selector_ldt = INDEX_LDT_FIRST << 3;
	for(i = 0;i < GlobalConstant::NR_ALL_TASKS;++i){		/* 由于Protection::init_prot()在cstart_ASM_FN()中被调用，
															比GlobalVariables::init_proc_table()和TaskManager::registerSysTask()
															在DozLixon_main_ASM_FN()中被调用早，所以用GlobalConstant::NR_ALL_TASKS
															而不是TaskManager::taskCount()
															*/
		init_descriptor(&GlobalVariables::gdt[selector_ldt >> 3],
			vir2phys(
				seg2phys(SELECTOR_KERNEL_DS),
				GlobalVariables::proc_table[i].ldts),
			GlobalConstant::LDT_SIZE * sizeof(Descriptor) - 1,
			DA_LDT);
		++p_proc;
		selector_ldt += 1 << 3;
	}
}

/* 线性地址 → 物理地址 */
inline __U32 Protection::vir2phys(__U32 seg_base, void * vir){
	return (seg_base + (__U32)vir);
}

//由段名求绝对地址
inline __U32 Protection::seg2phys(__U16 seg)
{
	Descriptor* p_dest = &GlobalVariables::gdt[seg >> 3];
	return (p_dest->base_high << 24) | (p_dest->base_mid << 16) | (p_dest->base_low);
}

//初始化 386 中断门
 void Protection::init_idt_desc(__U8 vector, __U8 desc_type, __PF_IntHandler handler, __U8 privilege)
{
	Gate * p_gate = &GlobalVariables::idt[vector];
	__U32 base = (__U32)handler;
	p_gate->offset_low = base & 0xFFFF;
	p_gate->selector = SELECTOR_KERNEL_CS;
	p_gate->dcount = 0;
	p_gate->attr = desc_type | (privilege << 5);
	p_gate->offset_high = (base >> 16) & 0xFFFF;
}

//初始化段描述符
void Protection::init_descriptor(Descriptor * p_desc, __U32 base, __U32 limit, __U16 attribute)
{
	p_desc->limit_low			= limit & 0x0FFFF;			// 段界限 1		(2 字节)
	p_desc->base_low			= base & 0x0FFFF;			// 段基址 1		(2 字节)
	p_desc->base_mid			= (base >> 16) & 0x0FF;		// 段基址 2		(1 字节)
	p_desc->attr1				= attribute & 0xFF;			// 属性 1
	p_desc->limit_high_attr2	= ((limit >> 16) & 0x0F) | (attribute >> 8) & 0xF0;// 段界限 2 + 属性 2
	p_desc->base_high			= (base >> 24) & 0x0FF;		// 段基址 3		(1 字节)
}

