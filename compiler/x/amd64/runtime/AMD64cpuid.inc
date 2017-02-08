;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; (c) Copyright IBM Corp. 2000, 2016
;;
;;  This program and the accompanying materials are made available
;;  under the terms of the Eclipse Public License v1.0 and
;;  Apache License v2.0 which accompanies this distribution.
;;
;;      The Eclipse Public License is available at
;;      http://www.eclipse.org/legal/epl-v10.html
;;
;;      The Apache License v2.0 is available at
;;      http://www.opensource.org/licenses/apache2.0.php
;;
;; Contributors:
;;    Multiple authors (IBM Corp.) - initial implementation and documentation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OPTION NOSCOPED

ifndef WINDOWS
; hardcoded CPUID instruction
cpuid macro
	db	00Fh
	db	0A2h
endm

; hardcoded STMXCSR instruction (64-bit result stored in [esp])
stmxcsr macro mustbeesp
	db	00Fh
	db	0AEh
	db	01Ch
	db	024h
endm
endif

eq_false		equ	0
eq_true			equ	1
eq_vendorId0		equ	0	; byte offsets into buf[]
eq_vendorId4		equ	4	; byte offsets into buf[]
eq_vendorId8		equ	8	; byte offsets into buf[]
eq_processorSignature	equ	12
eq_brandIdEtc		equ	16
eq_featureFlags		equ	20
eq_featureFlags2  equ 24
eq_featureFlags8  equ 44

	_TEXT SEGMENT PARA 'CODE'
	public jitGetCPUID
	public jitGetMXCSR

; Prototype: bool jitGetCPUID(void *buf);
; - buf must point to a buffer of sufficient size (24 bytes)
; - returns true if successful; false otherwise, in which case buf[] can contain anything
	align 16
jitGetCPUID	proc
	push	rbp		; preserve last frame pointer
	mov	rbp, rsp	; create new frame at current stack top
	push	rbx		; preserve registers

ifdef WINDOWS
        push    rdi
        mov     rdi, rcx        ; preserve 1st arg
endif

        ; Check that the CPUID instruction is supported.
        ;
	pushfq			; get RFLAGS content (assuming RFLAGS exists!)
	pop	rax
	mov	rbx, rax	; make a backup of the original RFLAGS content
	xor	rax, 200000h	; flip ID flag
	push	rax		; replace current RFLAGS content
	popfq
	pushfq			; check RFLAGS again
	pop	rax
	xor	rax, rbx	; compare updated value with backup
	je	L1

	xor	rax, rax        ; obtain initial set of CPUID information
	cpuid
	mov	dword ptr eq_vendorId0[rdi], ebx   ; store vendorId to buf[0]
	mov	dword ptr eq_vendorId4[rdi], edx
	mov	dword ptr eq_vendorId8[rdi], ecx

	cmp	rax, 1		; check if we can obtain processor signature and features
	jl	L1

	mov	rax, 1		; obtain processor signature and features
	cpuid
	mov	dword ptr eq_processorSignature[rdi], eax	; store data into buf[]
	mov	dword ptr eq_brandIdEtc[rdi], ebx
	mov	dword ptr eq_featureFlags[rdi], edx
	mov	dword ptr eq_featureFlags2[rdi], ecx

   mov   rcx, 0        ; have to clear the value of rcx, otherwise the next cpuid instruction would not give correct value
   mov   rax, 7        ; obtain transactional memory support information
   cpuid
   mov   dword ptr eq_featureFlags8[rdi], ebx
   mov   rax, eq_true
L2:

ifdef WINDOWS
        pop     rdi
endif
	pop     rbx
	pop     rbp
	ret

L1:				; unsupported architecture
	mov	rax, eq_false
	jmp	short L2

jitGetCPUID	endp

; Prototype: U_32 jitGetMXCSR(void);
; - throws an Illegal Instruction exception if the OS does not support SSE instructions
; - otherwise, returns the content of the MXCSR.
	align	16
jitGetMXCSR	proc
	sub	rsp, 8
	stmxcsr [rsp]
	pop	rax
	ret
jitGetMXCSR	endp

	_TEXT	ENDS

