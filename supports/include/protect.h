#ifndef	_DOLIXON_PROTECT_H_
#define	_DOLIXON_PROTECT_H_

#include "type.h"

/* �洢��������/ϵͳ�������� */
struct Descriptor		/* �� 8 ���ֽ� */
{
	__U16	limit_low;			/* Limit	�ν���	0-15	*/
	__U16	base_low;			/* Base		�λ�ַ	0-15	*/
	__U8	base_mid;			/* Base		�λ�ַ	16-24	*/
	__U8	attr1;				/* P(1) DPL(2) DT(1) TYPE(4) */
	__U8	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) �ν���(16-20) */
	__U8	base_high;			/* Base		�λ�ַ	24-31	*/
};

/* �������� */
struct Gate
{
	__U16	offset_low;	/* Offset Low */
	__U16	selector;	/* Selector */
	__U8	dcount;		/* ���ֶ�ֻ�ڵ���������������Ч��
						��������õ����ŵ����ӳ���ʱ������Ȩ����ת���Ͷ�ջ�ĸı䣬��Ҫ������ջ�еĲ������Ƶ��ڲ��ջ��
						��˫�ּ����ֶξ�������˵�������������ʱ��Ҫ���Ƶ�˫�ֲ����������� */
	__U8	attr;		/* P(1) DPL(2) DT(1) TYPE(4) */
	__U16	offset_high;	/* Offset High */
};

//Task-State Stack
struct TSS{
	__U32	backlink;	//��һ��������
	__U32	esp0;		/* stack pointer to use during interrupt */
	__U32	ss0;		/*   "   segment  "  "    "        "     */
	__U32	esp1;
	__U32	ss1;
	__U32	esp2;
	__U32	ss2;
	__U32	cr3;		//PDBR(Page-Directory Base Register)
	__U32	eip;
	__U32	flags;
	__U32	eax;
	__U32	ecx;
	__U32	edx;
	__U32	ebx;
	__U32	esp;
	__U32	ebp;
	__U32	esi;
	__U32	edi;
	__U32	es;
	__U32	cs;
	__U32	ss;
	__U32	ds;
	__U32	fs;
	__U32	gs;
	__U32	ldt;		//LDTѡ����
	__U16	trap;		//���������־
	__U16	iobase;	/* I/Oλͼ��ַ���ڻ����TSS�ν��ޣ��ͱ�ʾû��I/O���λͼ */
	//__U8	iomap[2];	//I/Oλͼ������־ 0xFF
};

class Protection
{
public:
	/* GDT */
	/* ���������� */
	static const int INDEX_DUMMY		= 0;	// ��
	static const int INDEX_FLAT_C		= 1;	// �� LOADER �����Ѿ�ȷ���˵�.
	static const int INDEX_FLAT_RW		= 2;	// ��
	static const int INDEX_VIDEO		= 3;	// ��
	static const int INDEX_TSS			= 4;
	static const int INDEX_LDT_FIRST	= 5;
	/* ѡ���� */
	static const int SELECTOR_DUMMY		= 0;		// ��
	static const int SELECTOR_FLAT_C	= 0x08;		// �� LOADER �����Ѿ�ȷ���˵�.
	static const int SELECTOR_FLAT_RW	= 0x10;		// ��
	static const int SELECTOR_VIDEO		= 0x18 + 3;	// ��<-- RPL=3
	static const int SELECTOR_TSS		= 0x20;		// TSS. ����������ڴ�ʱ SS �� ESP ��ֵ��������.
	static const int SELECTOR_LDT_FIRST	= 0x28;

	static const int SELECTOR_KERNEL_CS	= SELECTOR_FLAT_C;
	static const int SELECTOR_KERNEL_DS	= SELECTOR_FLAT_RW;
	static const int SELECTOR_KERNEL_GS	= SELECTOR_VIDEO;
	/* ����������ֵ˵�� */
	static const int DA_32			= 0x4000;	/* 32 λ��				*/
	static const int DA_LIMIT_4K	= 0x8000;	/* �ν�������Ϊ 4K �ֽ�	*/
	static const int DA_DPL0		= 0x00;		/* DPL = 0				*/
	static const int DA_DPL1		= 0x20;		/* DPL = 1				*/
	static const int DA_DPL2		= 0x40;		/* DPL = 2				*/
	static const int DA_DPL3		= 0x60;		/* DPL = 3				*/
	/* �洢������������ֵ˵�� */
	static const __U8 DA_DR		= 0x90;	/* ���ڵ�ֻ�����ݶ�����ֵ			*/
	static const __U8 DA_DRW	= 0x92;	/* ���ڵĿɶ�д���ݶ�����ֵ			*/
	static const __U8 DA_DRWA	= 0x93;	/* ���ڵ��ѷ��ʿɶ�д���ݶ�����ֵ	*/
	static const __U8 DA_C		= 0x98;	/* ���ڵ�ִֻ�д��������ֵ			*/
	static const __U8 DA_CR		= 0x9A;	/* ���ڵĿ�ִ�пɶ����������ֵ		*/
	static const __U8 DA_CCO	= 0x9C;	/* ���ڵ�ִֻ��һ�´��������ֵ		*/
	static const __U8 DA_CCOR	= 0x9E;	/* ���ڵĿ�ִ�пɶ�һ�´��������ֵ	*/
	/* ϵͳ������������ֵ˵�� */
	static const __U16 DA_LDT		= 0x82;	/* �ֲ��������������ֵ			*/
	static const __U16 DA_TaskGate	= 0x85;	/* ����������ֵ				*/
	static const __U16 DA_386TSS	= 0x89;	/* ���� 386 ����״̬������ֵ		*/
	static const __U8 DA_386CGate	= 0x8C;	/* 386 ����������ֵ			*/
	static const __U8 DA_386IGate	= 0x8E;	/* 386 �ж�������ֵ			*/
	static const __U8 DA_386TGate	= 0x8F;	/* 386 ����������ֵ			*/
	/* ѡ��������ֵ˵�� */
	/* ����, SA_ : Selector Attribute */
	static const __U32 SA_RPL_MASK	= 0xFFFC;
	static const __U32 SA_RPL0		= 0;
	static const __U32 SA_RPL1		= 1;
	static const __U32 SA_RPL2		= 2;
	static const __U32 SA_RPL3		= 3;

	static const __U32 SA_TI_MASK	= 0xFFFB;
	static const __U32 SA_TIG		= 0;
	static const __U32 SA_TIL		= 4;
	/* �ж����� */
	static const __U8 INT_VECTOR_DIVIDE			= 0x0;
	static const __U8 INT_VECTOR_DEBUG			= 0x1;
	static const __U8 INT_VECTOR_NMI			= 0x2;
	static const __U8 INT_VECTOR_BREAKPOINT		= 0x3;
	static const __U8 INT_VECTOR_OVERFLOW		= 0x4;
	static const __U8 INT_VECTOR_BOUNDS			= 0x5;
	static const __U8 INT_VECTOR_INVAL_OP		= 0x6;
	static const __U8 INT_VECTOR_COPROC_NOT		= 0x7;
	static const __U8 INT_VECTOR_DOUBLE_FAULT	= 0x8;
	static const __U8 INT_VECTOR_COPROC_SEG		= 0x9;
	static const __U8 INT_VECTOR_INVAL_TSS		= 0xA;
	static const __U8 INT_VECTOR_SEG_NOT		= 0xB;
	static const __U8 INT_VECTOR_STACK_FAULT	= 0xC;
	static const __U8 INT_VECTOR_PROTECTION		= 0xD;
	static const __U8 INT_VECTOR_PAGE_FAULT		= 0xE;
	static const __U8 INT_VECTOR_COPROC_ERR		= 0x10;
	/* Ӳ���ж����� */
	static const __U8 INT_VECTOR_IRQ0			= 0x20;
	static const __U8 INT_VECTOR_IRQ8			= 0x28;
	/* ϵͳ���� */
	static const __U8 INT_VECTOR_SYS_CALL		= 0x80;

	//functions
	static void init_prot();
private:
	static __U32 vir2phys(__U32 seg_base, void * vir);	/* ���Ե�ַ �� �����ַ */
	static __U32 seg2phys(__U16 seg);					//�ɶ�������Ե�ַ
	static void init_idt_desc(__U8 vector, __U8 desc_type, __PF_IntHandler handler, __U8 privilege);	//��ʼ�� 386 �ж���
	static void init_descriptor(Descriptor * p_desc, __U32 base, __U32 limit, __U16 attribute);			//��ʼ����������
};

#endif
