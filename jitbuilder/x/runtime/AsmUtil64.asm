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

; VM should define this at some point
OFFSETOF_CPU_CYCLE_COUNT equ 0

ifdef TR_HOST_64BIT

   include x/amd64/runtime/AMD64AsmUtil.inc

public initCycleCounter
public incrementCycleCounter
public getCycles

GetCycles MACRO
        rdtsc
        shl    rdx,32
        or     rax,rdx
ENDM

; void initCycleCounter()
initCycleCounter PROC NEAR
	GetCycles
        mov    qword ptr [rbp + OFFSETOF_CPU_CYCLE_COUNT], rax
        ret
initCycleCounter ENDP

; void incrementCycleCounter(intptr_t cycleCounterPointer)
incrementCycleCounter PROC NEAR
	GetCycles
        sub    rax, qword ptr [rbp + OFFSETOF_CPU_CYCLE_COUNT]
        add    qword ptr [rdi], rax
        ret
incrementCycleCounter ENDP

getCycles PROC NEAR
	GetCycles
	ret
getCycles ENDP

else
   .686p
   assume cs:flat,ds:flat,ss:flat
_DATA           segment para use32 public 'DATA'
AMD64AsmUtil:
   db      00h
_DATA           ends

endif
                end
