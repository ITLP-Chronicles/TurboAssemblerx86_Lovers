INCLUDE krsv2.inc
data segment
    ; ========== Program Cycle Variables =============
    inputBuffer db 200 dup(0)
    lastPressChar db 0
    
    ; ======== File Variables ===========
    filename db 20 dup(0)
    fileHandler dw 0
    fileTextLength dw 0
    fileBytesActuallyWritten dw 0
    
    ;========= MSGS =============
    msg_1 db 'Ingresa el nombre del archivo en donde guardar el contenido: ', 0
    msg_2 db 'Escribe el texto a introducir a continuacion (Enter solo para salir):', 0
    msg_error db 'Hubo un error con el archivo', 0
    newline db 13, 10, 0  ; For appending newlines
data ends

code segment
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        ;=== Start Code ===
        Console_WriteTextWith0 msg_1
        Console_ReadTextWith0 filename, 15
        Console_WriteBlankLine
        Console_WriteTextWith0 msg_2
        Console_WriteBlankLine
        
        ; Create and keep file open
        File_Create filename, fileHandler
        jc @ERROR_APP
        
 @WHILE_APP:
        Console_ReadTextWith0 inputBuffer, 195
        String_Len inputBuffer, fileTextLength
        cmp fileTextLength, 0  ; Check if input is empty (just Enter)
        je @END_APP
        
        ; Append the input text
        File_Append fileHandler, fileTextLength, inputBuffer, fileBytesActuallyWritten
        jc @ERROR_APP
        
        ; Append a newline
        File_Append fileHandler, 2, newline, fileBytesActuallyWritten
        jc @ERROR_APP
        
        jmp @WHILE_APP
        
 @ERROR_APP:
        Console_WriteTextWith0 msg_error
        jmp @CLOSE_AND_EXIT
        
 @END_APP:
        File_Close fileHandler
        jc @ERROR_APP
        
 @CLOSE_AND_EXIT:
        App_Exit
code ends
end start