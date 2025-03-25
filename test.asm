INCLUDE krsv2.inc
data segment

data ends

code segment
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        ;=== Start Code ===
        Theme_SetRed
        Draw_Rectangle 0 0 20 10
        Console_WriteChar 'K'
        ;=== End Code ====
        App_Exit
code ends
end start