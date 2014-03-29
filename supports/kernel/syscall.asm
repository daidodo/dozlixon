%include "sconst.inc"

_NR_GET_TICKS		equ	0		; 要跟 global.c 中 sys_call_table 的定义相对应！
_NR_WRITE			equ	1
INT_VECTOR_SYS_CALL	equ	0x80	; Linux用的是0x80


; 导出符号
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
	mov	ebx, [esp + 4]		;2个参数
	mov	ecx, [esp + 8]
	int	INT_VECTOR_SYS_CALL
	ret

