INCLUDE krsv2.inc
data segment
    pow_result dw 6 dup(0)
    pow_resultText db 6 dup(0)
data ends

code segment
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        ;=== Start Code ===
        mov ax, 2
        mov cx, 3
        Math_Pow 2, 3, pow_result
        Int_ToString pow_result pow_resultText
        Console_WriteTextWith0 pow_resultText
        ;=== End Code ====
        App_Exit
code ends
end start