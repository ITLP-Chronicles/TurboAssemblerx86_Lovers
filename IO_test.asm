INCLUDE krsv2.inc

data segment
    filename    db 'test.txt', 0        ; Null-terminated filename
    handler     dw 0                    ; File handle storage
    success     db 0                    ; Success flag for file operations
    text        db 'Hello, world!', 0   ; Text to write
    writeResult dw 0                    ; Result of File_Write (bytes written or error)
    numStr      db 6 dup(0)             ; Buffer for number-to-string conversion
    
    msgOpen     db 'File opened', 0
    msgWrite    db 'Writing to file', 0
    msgSuccess  db 'Write successful: bytes written = ', 0
    msgClose    db 'File closed', 0
    msgError    db 'Error occurred: AX = ', 0
data ends

code segment
    .386
    start: 
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax

        ; Create the file
        File_Create filename, handler
        jc @CREATE_ERROR
        Console_WriteTextWith0 msgOpen
        Console_WriteBlankLine

        ; Write to the file
        Console_WriteTextWith0 msgWrite
        Console_WriteBlankLine
        
        push ax
        mov ax, writeResult
        String_Len text writeResult
        File_Write handler, writeResult, text, writeResult  ; 13 = length of "Hello, world!"
        pop ax
        
        jc @WRITE_ERROR
        Console_WriteTextWith0 msgSuccess
        Int_ToString writeResult, numStr           ; Convert writeResult to string
        Console_WriteTextWith0 numStr              ; Print the number of bytes written
        Console_WriteBlankLine

        ; Close the file
        File_Close handler, success
        jc @CLOSE_ERROR
        Console_WriteTextWith0 msgClose
        Console_WriteBlankLine
        jmp @FINAL_END

    @CREATE_ERROR:
        Console_WriteTextWith0 msgError
        Int_ToString ax, numStr                    ; Convert error code to string
        Console_WriteTextWith0 numStr              ; Print error code
        Console_WriteBlankLine
        jmp @FINAL_END

    @WRITE_ERROR:
        Console_WriteTextWith0 msgError
        Int_ToString writeResult, numStr           ; Convert partial bytes or error to string
        Console_WriteTextWith0 numStr
        Console_WriteBlankLine
        jmp @FINAL_END

    @CLOSE_ERROR:
        Console_WriteTextWith0 msgError
        Int_ToString ax, numStr                    ; Convert error code to string
        Console_WriteTextWith0 numStr
        Console_WriteBlankLine

    @FINAL_END:
        App_Exit
        
code ends
end start