INCLUDE krsv2.inc
data segment
    ;byte
    tom db 10 dup(1)
    tom2 db 0 ;byte
    
    ;doubleByte
    tomlargo dw 15 dup(0) ; new doubleByte[15] {0,0,0,0,0,0,0,0,0,0,0,0,0}
    tomlargo2 dw 0 ; (2byte)
data ends  

code segment
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        
        ;=== Start Code ===
        ; 2byte <= 1byte
        mov ah, 0
        mov al, tom 
        
        
        
        
        
        ;=== End Code ====
        App_Exit
code ends
end start