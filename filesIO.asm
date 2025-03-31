INCLUDE krsv2.inc
data segment
    archivo db 'archivo.txt', 0
    handler dw 0
    success db 0
    
    text db 'Hola mundo texto', 0
    
    isCorrect db 'Todo bien', 0
    isIncorrect db 'Todo mal', 0
data ends

code segment
    .386
    start: 
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        ;=== Start Code ===
        
        Theme_SetRed
        File_Create archivo,handler
        File_Close handler,success
        cmp success, 0
        jne @APP_END
        Console_WriteTextWith0 isCorrect
        
        @APP_END:
        Console_WriteTextWith0 isIncorrect
        ;File_Write handler,17,text
        

        ;=== End Code ====
        App_Exit
code ends
end start