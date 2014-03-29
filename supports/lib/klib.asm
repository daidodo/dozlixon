%include "sconst.inc"

[SECTION .data]

; 导入全局变量
extern	disp_pos_ASM

[SECTION .text]

; 导出函数
global	disp_str_ASM_FN
global	disp_color_str_ASM_FN
global	out_byte_ASM_FN
global	in_byte_ASM_FN
global	enable_irq_ASM_FN
global	disable_irq_ASM_FN
global	enable_int_ASM_FN
global	disable_int_ASM_FN

;===========================================
;显示字符串
;void disp_str_ASM_FN(char * info);
;===========================================
disp_str_ASM_FN:
	push	ebp
	mov	ebp, esp

	mov	esi, [ebp + 8]	; pszInfo
	mov	edi, [disp_pos_ASM]
	mov	ah, 0Fh
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	; 是回车吗?
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1

.2:
	mov	[disp_pos_ASM], edi

	pop	ebp
	ret

;====================================================
;写端口
;void	out_byte_ASM_FN(__Port port,__U8 value);
;====================================================
out_byte_ASM_FN:
	mov edx,[esp + 4]
	mov al,[esp + 4 + 4]
	out dx,al
	
	nop
	nop
	ret
	
;====================================================
;读端口
;__U8	in_byte_ASM_FN(__Port);
;====================================================
in_byte_ASM_FN:
	mov edx,[esp + 4]
	xor eax,eax
	in al,dx
	
	nop
	nop
	ret

;====================================================
;显示彩色字符串
;void disp_color_str_ASM_FN(char * info, int color);
;====================================================
disp_color_str_ASM_FN:
	push ebp
	mov ebp,esp
	mov esi,[ebp + 8]		;sizeof(char *) + sizeof(int) = 8
	mov edi,[disp_pos_ASM]
	mov ah,[ebp + 12]
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	; 是回车吗?
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1

.2:
	mov	[disp_pos_ASM], edi

	pop	ebp
	ret

; ========================================================================
;                  void disable_irq_ASM_FN(int irq);
; ========================================================================
; Disable an interrupt request line by setting an 8259 bit.
; Equivalent code for irq < 8:
;       out_byte_ASM_FN(INT_CTLMASK, in_byte_ASM_FN(INT_CTLMASK) | (1 << irq));
; Returns true iff the interrupt was not already disabled.
;
disable_irq_ASM_FN:
	mov	ecx, [esp + 4]		; irq
	pushf
	cli
	mov	ah, 1
	rol	ah, cl			; ah = (1 << (irq % 8))
	cmp	cl, 8
	jae	disable_8		; disable irq >= 8 at the slave 8259
disable_0:
	in	al, INT_M_CTLMASK
	test	al, ah
	jnz	dis_already		; already disabled?
	or	al, ah
	out	INT_M_CTLMASK, al	; set bit at master 8259
	popf
	mov	eax, 1			; disabled by this function
	ret
disable_8:
	in	al, INT_S_CTLMASK
	test	al, ah
	jnz	dis_already		; already disabled?
	or	al, ah
	out	INT_S_CTLMASK, al	; set bit at slave 8259
	popf
	mov	eax, 1			; disabled by this function
	ret
dis_already:
	popf
	xor	eax, eax		; already disabled
	ret

; ========================================================================
;                  void enable_irq_ASM_FN(int irq);
; ========================================================================
; Enable an interrupt request line by clearing an 8259 bit.
; Equivalent code:
;	if(irq < 8){
;		out_byte_ASM_FN(INT_M_CTLMASK, in_byte_ASM_FN(INT_M_CTLMASK) & ~(1 << irq));
;	}
;	else{
;		out_byte_ASM_FN(INT_S_CTLMASK, in_byte_ASM_FN(INT_S_CTLMASK) & ~(1 << irq));
;	}
;
enable_irq_ASM_FN:
        mov	ecx, [esp + 4]		; irq
        pushf
        cli
        mov	ah, ~1
        rol	ah, cl			; ah = ~(1 << (irq % 8))
        cmp	cl, 8
        jae	enable_8		; enable irq >= 8 at the slave 8259
enable_0:
        in	al, INT_M_CTLMASK
        and	al, ah
        out	INT_M_CTLMASK, al	; clear bit at master 8259
        popf
        ret
enable_8:
        in	al, INT_S_CTLMASK
        and	al, ah
        out	INT_S_CTLMASK, al	; clear bit at slave 8259
        popf
        ret

; ========================================================================
;                  void disable_int_ASM_FN();
; ========================================================================
disable_int_ASM_FN:
	cli
	ret

; ========================================================================
;                  void enable_int_ASM_FN();
; ========================================================================
enable_int_ASM_FN:
	sti
	ret

