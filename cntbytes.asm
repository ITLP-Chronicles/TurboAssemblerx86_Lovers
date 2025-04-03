INCLUDE krsv2.inc
data segment
    start_msg db 'Escribe el nombre del archivo a abrir: ', 10, 13, 0
    msg db 'Son ',0
    msg_end db ' bytes dentro del archivo: ', 0
    msg_end_end db '.', 0
    
    doesntExist_msg db 'El archivo: ', 0
    doesntExist_msg_end db ' no existe...', 0
    
    numberStr db 30 dup(0)
    
    fileNameInputBuffer db 30 dup(0)
    fileHandler dw 0
    fileSizeHigh dw 0
    fileSizeLow dw 15
data ends

code segment
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        ;=== Start Code ===
        Console_WriteTextWith0 start_msg
        Console_ReadTextWith0 fileNameInputBuffer, 25
        
        File_Open fileNameInputBuffer,fileHandler,2
        jc @NOTFOUND_ERROR
        jmp @CORRECT
        
    @NOTFOUND_ERROR:
        Console_WriteTextWith0 doesntExist_msg
        Console_WriteTextWith0 fileNameInputBuffer
        Console_WriteTextWith0 doesntExist_msg_end
        
        jmp @APP_END
    @CORRECT:
        File_GetSize fileHandler,fileSizeHigh,fileSizeLow
        Int_ToString fileSizeLow,numberStr
        Console_WriteTextWith0 msg
        Console_WriteTextWith0 numberStr
        Console_WriteTextWith0 msg_end
        
        jmp @APP_END
    @APP_END:
        ;=== End Code ====
        App_Exit
code ends
end start