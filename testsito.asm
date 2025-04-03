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
        
        ;=== End Code ====
        App_Exit
code ends
end start