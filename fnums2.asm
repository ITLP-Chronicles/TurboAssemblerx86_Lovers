INCLUDE krsv2.inc
.model SMALL
.386
.STACK 100h

.data
    pi      dd 3.0
  
.code
start:
        mov ax, @data
        mov ds, ax
        
        ;=== Start Code ===
        fld dword ptr [pi]      ; Load pi (3.14) to FPU stack
        fmul dword ptr [number2] ; Add number2 (6.0) 
        ; Result (9.14) is now in ST(0)
        fistp word ptr [result] ; Convert float to integer and store (rounds to 9)
        
        ;==== Printing Zone ======
        mov ax, [result]        ; Move 16-bit result to AX
        add al, '0'
        Console_WriteChar al  ; Prints ASCII character corresponding to value in AL
        ;=== End Code ===
        App_Exit
end start
