INCLUDE krsv2.inc
data segment
    ; ========== Program Cycle Variables =============
    inputBuffer db 200 dup(0)
    lastPressChar db 0
    successFlagWord dw 0
    successFlagByte db 0
    
    
    ; ======== File Variables ===========
    filename db 20 dup(0)
    fileHandler dw 0
    fileTextLength dw 0
    
    ;========= MSGS =============
    msg_1 db 'Ingresa el nombre del archivo en donde guardar el contenido: ', 0
    msg_2 db 'Escribe el texto a introducir a continuacion:', 0
    msg_error  db 'Hubo un error con el archivo', 0
data ends

code segment
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        ;=== Start Code ===
        Console_WriteTextWith0 msg_1
        Console_ReadTextWith0 filename 15
        Console_WriteBlankLine
        Console_WriteTextWith0 msg_2
        Console_WriteBlankLine
        File_Create filename,fileHandler
        jc @ERROR_APP
        File_Close fileHandler,successFlagByte ;Checkif success
        
 @WHILE_APP:
        Console_ReadTextWith0 inputBuffer 195
        
        File_Open filename,fileHandler, 1 ;Only write
        String_Len inputBuffer,fileTextLength
        ;File_MovePointerInside 02h,fileHandler,0,0
        File_Write fileHandler,fileTextLength,inputBuffer,successFlagWord
        File_Close fileHandler,successFlagByte
        
        Console_ReadChar lastPressChar
        cmp lastPressChar, 13
        je @END_APP
        jmp @WHILE_APP
 @ERROR_APP:
        Console_WriteTextWith0 msg_error
 @END_APP:
        ;=== End Code ====
        App_Exit
code ends
end start