
[SECTION .text]

; ��������
global	memcpy_ASM_FN
global	memset_ASM_FN
global	strcpy_ASM_FN

; ------------------------------------------------------------------------
; void* memcpy_ASM_FN(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------
memcpy_ASM_FN:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	; Destination
	mov	esi, [ebp + 12]	; Source
	mov	ecx, [ebp + 16]	; Counter
.1:
	cmp	ecx, 0		; �жϼ�����
	jz	.2		; ������Ϊ��ʱ����

	mov	al, [ds:esi]		; ��
	inc	esi					; ��
							; �� ���ֽ��ƶ�
	mov	byte [es:edi], al	; ��
	inc	edi					; ��

	dec	ecx		; ��������һ
	jmp	.1		; ѭ��
.2:
	mov	eax, [ebp + 8]	; ����ֵ

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret			; ��������������
; memcpy_ASM_FN ����-------------------------------------------------------------

; ------------------------------------------------------------------------
; void memset_ASM_FN(void* p_dst, char ch, int size);
; ------------------------------------------------------------------------
memset_ASM_FN:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	; Destination
	mov	edx, [ebp + 12]	; Char to be putted
	mov	ecx, [ebp + 16]	; Counter
.1:
	cmp	ecx, 0		; �жϼ�����
	jz	.2		; ������Ϊ��ʱ����

	mov	byte [edi], dl		; ��
	inc	edi					; ��

	dec	ecx		; ��������һ
	jmp	.1		; ѭ��
.2:

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret			; ��������������
; ------------------------------------------------------------------------

; ------------------------------------------------------------------------
; char* strcpy_ASM_FN(char* p_dst, char* p_src);
; ------------------------------------------------------------------------
strcpy_ASM_FN:
	push	ebp
	mov	ebp, esp

	mov	esi, [ebp + 12]	; Source
	mov	edi, [ebp + 8]	; Destination

.1:
	mov	al, [esi]			; ��
	inc	esi					; ��
							; �� ���ֽ��ƶ�
	mov	byte [edi], al		; ��
	inc	edi					; ��

	cmp	al, 0		; �Ƿ����� '\0'
	jnz	.1		; û�����ͼ���ѭ���������ͽ���

	mov	eax, [ebp + 8]	; ����ֵ

	pop	ebp
	ret			; ��������������
; strcpy_ASM_FN ����-------------------------------------------------------------
