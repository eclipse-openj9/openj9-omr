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

	.486p
	assume cs:flat,ds:flat,ss:flat

; hardcoded CPUID instruction
cpuid macro
	db	00Fh
	db	0A2h
endm

; hardcoded STMXCSR instruction (32-bit result stored in [esp])
stmxcsr macro
	db	00Fh
	db	0AEh
	db	01Ch
	db	024h
endm

eq_false                      equ	0
eq_true				            equ	1
eq_vendorId0			         equ	0	; byte offsets into buf[]
eq_vendorId4			         equ	4	; byte offsets into buf[]
eq_vendorId8			         equ	8	; byte offsets into buf[]
eq_processorSignature		   equ	12
eq_brandIdEtc			         equ	16
eq_featureFlags			      equ	20
eq_featureFlags2			      equ	24
eq_cacheDescription_L1instr	equ	28
eq_cacheDescription_L1data	   equ	32
eq_cacheDescription_L2		   equ	36
eq_cacheDescription_L3		   equ	40
eq_featureFlags8              equ   44

	_TEXT SEGMENT PARA USE32 PUBLIC 'CODE'
	public jitGetCPUID
	public jitGetMXCSR

; Prototype: bool jitGetCPUID(void *buf);
; - buf must point to a buffer of sufficient size (40 bytes)
; - returns true if successful; false otherwise, in which case buf[] can contain anything
	align 16
jitGetCPUID	proc near
	push	ebp		      ; preserve last frame pointer
	mov	ebp, esp	      ; create new frame at current stack top
	push	ebx		      ; preserve registers
	push	esi
	mov	esi, 16[esp]	; get value of buf
	pushfd			      ; get EFLAGS content (assuming EFLAGS exists!)
	pop	eax
	mov	ecx, eax	      ; make a backup of the original EFLAGS content
	xor	eax, 200000h	; flip ID flag
	push	eax		      ; replace current EFLAGS content
	popfd
	pushfd			      ; check EFLAGS again
	pop	eax
	xor	eax, ecx	      ; compare updated value with backup
	je	L1                ; processor not supporting cpuid

	mov	dword ptr eq_cacheDescription_L1instr[esi],	0 ; initialize the cache info
	mov	dword ptr eq_cacheDescription_L1data[esi],	0
	mov	dword ptr eq_cacheDescription_L2[esi],		0
	mov	dword ptr eq_cacheDescription_L3[esi],		0

	mov	eax, 0		; obtain initial set of CPUID information
	cpuid
	mov	dword ptr eq_vendorId0[esi], ebx		; store vendorId to buf[0]
	mov	dword ptr eq_vendorId4[esi], edx
	mov	dword ptr eq_vendorId8[esi], ecx

LcheckIntel:
	cmp	ebx,756e6547h	; "Genu"
	jne	LcheckAMD	; check if we're on AMD if we aren't on Intel
	cmp	edx,49656e69h	; "ineI"
	jne	LcheckAMD
	cmp	ecx,6c65746eh	; "ntel"
	jne	LcheckAMD

	cmp	eax, 2
	jl	LcheckSignature

	mov	eax, 2
	cpuid
	mov	dword ptr eq_cacheDescription_L1instr[esi],	eax ; load intel cache info
	mov	dword ptr eq_cacheDescription_L1data[esi],	ebx
	mov	dword ptr eq_cacheDescription_L2[esi],		ecx
	mov	dword ptr eq_cacheDescription_L3[esi],		edx

	jmp	Lsignature

LcheckAMD:
	cmp	ebx,68747541h	; "Auth"
	jne	LcheckSignature	   ; bail out if we aren't on AuthenticAMD
	cmp	edx,69746e65h	; "enti"
	jne	LcheckSignature
	cmp	ecx,444d4163h	; "cAMD"
	jne	LcheckSignature

	mov	eax, 80000000h ; check the largest extended function number supported
	cpuid
	cmp	eax, 80000006h
	jl	LcheckAMD3

	mov	eax, 80000006h
	cpuid
	mov	dword ptr eq_cacheDescription_L2[esi],		ecx

	mov	eax, 80000005h

LcheckAMD2:
	cpuid
	mov	dword ptr eq_cacheDescription_L1instr[esi],	edx ; load amd cache info
	mov	dword ptr eq_cacheDescription_L1data[esi],	ecx
	jmp	Lsignature


LcheckAMD3:
   cmp	eax, 80000005h
   je LcheckAMD2

LcheckSignature:
	cmp	eax, 1		; check if we can obtain processor signature and features
	jl	L1

Lsignature:
	mov	eax, 1		; obtain processor signature and features
	cpuid
	mov	dword ptr eq_processorSignature[esi], eax	; store data into buf[]
	mov	dword ptr eq_brandIdEtc[esi], ebx
	mov	dword ptr eq_featureFlags[esi], edx
	mov	dword ptr eq_featureFlags2[esi], ecx
   mov   ecx, 0;
   mov   eax, 7; obtain transactional memory support information
   cpuid
   mov   dword ptr eq_featureFlags8[esi], ebx
	mov	eax, eq_true
L2:
	pop	esi		; restore registers
	pop	ebx
	pop	ebp
	ret

L1:				; unsupported architecture
	mov	eax, eq_false
	jmp	short L2

jitGetCPUID	endp

; Prototype: U_32 jitGetMXCSR(void);
; - throws an Illegal Instruction exception if the OS does not support SSE instructions
; - otherwise, returns the content of the MXCSR.
	align	16
jitGetMXCSR	proc near
	sub	esp, 4
	stmxcsr
	pop	eax
	ret
jitGetMXCSR	endp

	_TEXT	ENDS

