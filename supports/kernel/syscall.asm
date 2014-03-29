%include "sconst.inc"

_NR_GET_TICKS		equ	0		; Ҫ�� global.c �� sys_call_table �Ķ������Ӧ��
_NR_WRITE			equ	1
INT_VECTOR_SYS_CALL	equ	0x80	; Linux�õ���0x80


; ��������
global	get_ticks_ASM_FN
global	write_ASM_FN

bits 32
[section .text]

; ====================================================================================
;                                    get_ticks_ASM_FN
; ====================================================================================
get_ticks_ASM_FN:
	mov	eax, _NR_GET_TICKS
	int	INT_VECTOR_SYS_CALL
	ret

; ====================================================================================
;                          void write_ASM_FN(char* buf, int len);
; ====================================================================================
write_ASM_FN:
	mov	eax, _NR_WRITE
	mov	ebx, [esp + 4]		;2������
	mov	ecx, [esp + 8]
	int	INT_VECTOR_SYS_CALL
	ret

