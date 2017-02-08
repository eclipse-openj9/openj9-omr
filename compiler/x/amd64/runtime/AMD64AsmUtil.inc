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

_TEXT   segment para 'CODE'

public compareAndExchange1
public compareAndExchange2
public compareAndExchange4
public compareAndExchange8
public _patchingFence16
public _patchingFence24
public _patchingFenceCPUID
public atomicIncrement4

Align16 macro
; TODO:AMD64: Find a way not to get 16 bytes of padding when we are already aligned!
        align 16
endm

; Atomically Compare And Exchange the value contained data pointed to by ptr
; with oldValue, and if equal, replace by newValue
; Return zero on failure. Nonzero otherwise.
;
; prototype: int32_t compareAndExchange1(uint8_t *ptr, uint8_t oldValue, uint8_t newValue)
;

prepareCompareAndExchangeArguments macro
        ;; TODO: this generates Rex prefixes that are only needed for the 8-byte case
IFDEF WINDOWS
        ;; rcx=address, rdx=old, r8=new
        mov     rax, rdx
        mov     rdx, r8
ELSE
        ;; rdi=address, rsi=old, rdx=new
        mov     rcx, rdi
        mov     rax, rsi
ENDIF
        ;; rcx=address, rax=old, rdx=new
endm

        Align16
compareAndExchange1 PROC
        prepareCompareAndExchangeArguments
        lock cmpxchg byte ptr [rcx], dl
        jnz     failedCMPXCHG
passedCMPXCHG: ;; Placed here to enhance branch prediction
        mov     eax, 1
        ret
compareAndExchange1 ENDP

        Align16
compareAndExchange2 PROC
        prepareCompareAndExchangeArguments
        lock cmpxchg word ptr [rcx], dx
        jz      passedCMPXCHG
failedCMPXCHG: ;; Placed here to enhance branch prediction
        xor     eax, eax
        ret
compareAndExchange2 ENDP

        Align16
compareAndExchange4 PROC
        prepareCompareAndExchangeArguments
        lock cmpxchg dword ptr [rcx], edx
        jz      passedCMPXCHG
        jmp     failedCMPXCHG
compareAndExchange4 ENDP

        Align16
compareAndExchange8 PROC
        prepareCompareAndExchangeArguments
        lock cmpxchg qword ptr [rcx], rdx
        jz      passedCMPXCHG
        jmp     failedCMPXCHG
compareAndExchange8 ENDP

        Align16
_patchingFence16 PROC
        mfence
IFDEF WINDOWS
        clflush [rcx]
        clflush [rcx+8]
ELSE
        clflush [rdi]
        clflush [rdi+8]
ENDIF
        mfence
        ret
_patchingFence16 ENDP

        Align16
_patchingFence24 PROC
        mfence
IFDEF WINDOWS
        clflush [rcx]
        clflush [rcx+8]
        clflush [rcx+16]
ELSE
        clflush [rdi]
        clflush [rdi+8]
        clflush [rdi+16]
ENDIF
        mfence
        ret
_patchingFence24 ENDP


; void _patchingFenceCPUID();

_patchingFenceCPUID PROC
         pushfq
         push       rbx
         cpuid
         pop        rbx
         popfq
         ret
_patchingFenceCPUID ENDP

; prototype: int32_t atomicIncrement4(int32_t *value)
atomicIncrement4 PROC
   mov   eax, [rdi]        ; load original value
LtryNextValue:
   mov   ecx, eax
   add   ecx, 1
   lock cmpxchg dword ptr [rdi], ecx
   jnz   LtryNextValue
   mov   eax, ecx
   ret
atomicIncrement4 ENDP

_TEXT   ends
