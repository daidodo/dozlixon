%include "sconst.inc"

; 导入函数
extern	cstart_ASM_FN
extern	DozLixon_main_ASM_FN
extern	exception_handler_ASM_FN
extern	spurious_irq_ASM_FN

; 导入全局变量
extern	gdt_ptr_ASM
extern	idt_ptr_ASM
extern	p_proc_ready_ASM
extern	tss_ASM
extern	disp_pos_ASM
extern	k_reenter_ASM
extern	irq_table_ASM
extern	sys_call_table

bits 32

[SECTION .bss]
StackSpace		resb	2 * 1024
StackTop:		; 栈顶

[section .text]	; 代码在此

; 导出函数
global _start	; 导出 _start
global restart_ASM_FN
global sys_call_ASM_FN

; 中断处理函数
global	divide_error
global	single_step_exception
global	nmi
global	breakpoint_exception
global	overflow
global	bounds_check
global	inval_opcode
global	copr_not_available
global	double_fault
global	copr_seg_overrun
global	inval_tss
global	segment_not_present
global	stack_exception
global	general_protection
global	page_fault
global	copr_error
global	hwint00
global	hwint01
global	hwint02
global	hwint03
global	hwint04
global	hwint05
global	hwint06
global	hwint07
global	hwint08
global	hwint09
global	hwint10
global	hwint11
global	hwint12
global	hwint13
global	hwint14
global	hwint15


_start:
	; 此时内存看上去是这样的（更详细的内存情况在 LOADER.ASM 中有说明）：
	;              ┃                                    ┃
	;              ┃                 ...                ┃
	;              ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃■■■■■■Page  Tables■■■■■■┃
	;              ┃■■■■■(大小由LOADER决定)■■■■┃ PageTblBase
	;    00101000h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃■■■■Page Directory Table■■■■┃ PageDirBase = 1M
	;    00100000h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃□□□□ Hardware  Reserved □□□□┃ B8000h ← gs
	;       9FC00h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃■■■■■■■LOADER.BIN■■■■■■┃ somewhere in LOADER ← esp
	;       90000h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃■■■■■■■KERNEL.BIN■■■■■■┃
	;       80000h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃■■■■■■■■KERNEL■■■■■■■┃ 30400h ← KERNEL 入口 (KernelEntryPointPhyAddr)
	;       30000h ┣━━━━━━━━━━━━━━━━━━┫
	;              ┋                 ...                ┋
	;              ┋                                    ┋
	;           0h ┗━━━━━━━━━━━━━━━━━━┛ ← cs, ds, es, fs, ss
	;
	;
	; GDT 以及相应的描述符是这样的：
	;
	;		              Descriptors               Selectors
	;              ┏━━━━━━━━━━━━━━━━━━┓
	;              ┃         Dummy Descriptor           ┃
	;              ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃         DESC_FLAT_C    (0～4G)     ┃   8h = cs
	;              ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃         DESC_FLAT_RW   (0～4G)     ┃  10h = ds, es, fs, ss
	;              ┣━━━━━━━━━━━━━━━━━━┫
	;              ┃         DESC_VIDEO                 ┃  1Bh = gs
	;              ┗━━━━━━━━━━━━━━━━━━┛
	;
	; 注意! 在使用 C 代码的时候一定要保证 ds, es, ss 这几个段寄存器的值是一样的
	; 因为编译器有可能编译出使用它们的代码, 而编译器默认它们是一样的. 比如串拷贝操作会用到 ds 和 es.
	;
	;

	; 把 esp 从 LOADER 挪到 KERNEL
	mov	esp, StackTop	; 堆栈在 bss 段中

	;mov	dword [disp_pos_ASM], 0		;光标至屏幕左上角

	sgdt	[gdt_ptr_ASM]	; cstart_ASM_FN() 中将会用到 gdt_ptr_ASM
	call	cstart_ASM_FN	; 在此函数中改变了gdt_ptr_ASM，让它指向新的GDT(start.cpp)
	lgdt	[gdt_ptr_ASM]	; 使用新的GDT

	lidt	[idt_ptr_ASM]

	jmp	SELECTOR_KERNEL_CS:csinit
csinit:		; “这个跳转指令强制使用刚刚初始化的结构”——<<OS:D&I 2nd>> P90.

	;异常处理试验
	;jmp 0x40:0
	;ud2

	;sti
	
	;加载tr
	xor eax,eax
	mov ax,SELECTOR_TSS
	ltr ax
	
	jmp DozLixon_main_ASM_FN	;跳入一个进程中(start.cpp)


; 中断和异常 -- 硬件中断
; ---------------------------------
%macro	hwint_master	1
	call save
	
	;屏蔽当前中断
	in al,INT_M_CTLMASK
	or al,(1 << %1)
	out INT_M_CTLMASK,al
	
	;reenable master 8259
	mov al,EOI
	out INT_M_CTL,al
	
	sti		; CPU在响应中断的过程中会自动关中断，这句之后就允许响应新的中断
	
	;调用中断处理程序
	push %1
	call [irq_table_ASM + 4 * %1]
	pop ecx
	
	cli
	
	;恢复接受当前中断
	in al,INT_M_CTLMASK
	and al,~(1 << %1)
	out INT_M_CTLMASK,al
	
	ret		;重入时跳到restart_reenter，通常情况下到restart_ASM_FN
%endmacro


ALIGN	16
hwint00:		; Interrupt routine for irq 0 (the clock).
	hwint_master	0

ALIGN	16
hwint01:		; Interrupt routine for irq 1 (keyboard)
	hwint_master	1

ALIGN	16
hwint02:		; Interrupt routine for irq 2 (cascade!)
	hwint_master	2

ALIGN	16
hwint03:		; Interrupt routine for irq 3 (second serial)
	hwint_master	3

ALIGN	16
hwint04:		; Interrupt routine for irq 4 (first serial)
	hwint_master	4

ALIGN	16
hwint05:		; Interrupt routine for irq 5 (XT winchester)
	hwint_master	5

ALIGN	16
hwint06:		; Interrupt routine for irq 6 (floppy)
	hwint_master	6

ALIGN	16
hwint07:		; Interrupt routine for irq 7 (printer)
	hwint_master	7

; ---------------------------------
%macro	hwint_slave	1
	push	%1
	call	spurious_irq_ASM_FN
	add	esp, 4
	hlt
%endmacro
; ---------------------------------

ALIGN	16
hwint08:		; Interrupt routine for irq 8 (realtime clock).
	hwint_slave	8

ALIGN	16
hwint09:		; Interrupt routine for irq 9 (irq 2 redirected)
	hwint_slave	9

ALIGN	16
hwint10:		; Interrupt routine for irq 10
	hwint_slave	10

ALIGN	16
hwint11:		; Interrupt routine for irq 11
	hwint_slave	11

ALIGN	16
hwint12:		; Interrupt routine for irq 12
	hwint_slave	12

ALIGN	16
hwint13:		; Interrupt routine for irq 13 (FPU exception)
	hwint_slave	13

ALIGN	16
hwint14:		; Interrupt routine for irq 14 (AT winchester)
	hwint_slave	14

ALIGN	16
hwint15:		; Interrupt routine for irq 15
	hwint_slave	15



; 中断和异常 -- 异常
divide_error:
	push	0xFFFFFFFF	; no err code
	push	0		; vector_no	= 0
	jmp	exception
single_step_exception:
	push	0xFFFFFFFF	; no err code
	push	1		; vector_no	= 1
	jmp	exception
nmi:
	push	0xFFFFFFFF	; no err code
	push	2		; vector_no	= 2
	jmp	exception
breakpoint_exception:
	push	0xFFFFFFFF	; no err code
	push	3		; vector_no	= 3
	jmp	exception
overflow:
	push	0xFFFFFFFF	; no err code
	push	4		; vector_no	= 4
	jmp	exception
bounds_check:
	push	0xFFFFFFFF	; no err code
	push	5		; vector_no	= 5
	jmp	exception
inval_opcode:
	push	0xFFFFFFFF	; no err code
	push	6		; vector_no	= 6
	jmp	exception
copr_not_available:
	push	0xFFFFFFFF	; no err code
	push	7		; vector_no	= 7
	jmp	exception
double_fault:
	push	8		; vector_no	= 8
	jmp	exception
copr_seg_overrun:
	push	0xFFFFFFFF	; no err code
	push	9		; vector_no	= 9
	jmp	exception
inval_tss:
	push	10		; vector_no	= A
	jmp	exception
segment_not_present:
	push	11		; vector_no	= B
	jmp	exception
stack_exception:
	push	12		; vector_no	= C
	jmp	exception
general_protection:
	push	13		; vector_no	= D
	jmp	exception
page_fault:
	push	14		; vector_no	= E
	jmp	exception
copr_error:
	push	0xFFFFFFFF	; no err code
	push	16		; vector_no	= 10h
	jmp	exception

exception:
	call	exception_handler_ASM_FN
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	hlt

;========================================================
;				void restart_ASM_FN();
;========================================================	
restart_ASM_FN:
	mov esp,[p_proc_ready_ASM]	;离开内核栈
	
	lldt [esp + P_LDT_SEL]	;重新加载ldtr
	
	lea eax,[esp + P_STACKTOP]
	mov dword [tss_ASM + TSS3_S_SP0],eax
	
;========================================================
;				restart_reenter
;========================================================	
restart_reenter:
	dec dword [k_reenter_ASM]
	
	pop gs
	pop fs
	pop es
	pop ds
	popad
	add esp,4
	
	iretd

;========================================================
;				save
;========================================================	
save:
	;保存原寄存器的值
	pushad
	push ds
	push es
	push fs
	push gs
	
	mov dx,ss
	mov ds,dx
	mov es,dx
	
	mov esi,esp		;eax = 进程表起始地址
	
	inc dword [k_reenter_ASM]
	cmp dword [k_reenter_ASM],0
	jne .1	;发生中断重入，通常情况下不会发生
	
	mov esp,StackTop	;切入内核栈
	
	push restart_ASM_FN
	jmp [esi + RETADDR - P_STACKBASE]
.1:	;中断重入
	push restart_reenter
	jmp [esi + RETADDR - P_STACKBASE]

; ====================================================================================
;                                 sys_call_ASM_FN
; ====================================================================================
sys_call_ASM_FN:
	call	save
	push	dword [p_proc_ready_ASM]	;当前进程
	sti
	push	ecx							;参数入栈
	push	ebx
	call	[sys_call_table + eax * 4]
	add		esp, 4 * 3					;参数出栈
	mov		[esi + EAXREG - P_STACKBASE], eax
	cli
	ret

