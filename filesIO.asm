INCLUDE krsv2.inc
data segment
    filename    db 'test.txt', 0        ; Null-terminated filename
    handler     dw 0                    ; File handle storage
    successFlag db 0                    ; Success flag ONLY for File_Close (as example, could be removed if File_Close uses CF)
    textToWrite db 'Hello, world!', 0   ; Text to write
    bytesToWrite dw 0                   ; To store length of textToWrite
    bytesWritten dw 0                   ; Result of File_Write (bytes written or error code on failure)
    bytesToRead  dw 50                  ; Max bytes we want to try reading
    bytesRead    dw 0                   ; To store actual bytes read from file
    readBuffer   db 100 dup(?)          ; Buffer to store read text (ensure size >= bytesToRead)

    numStr      db 6 dup(0)             ; Buffer for number-to-string conversion

    ; Messages
    msgCreate     db 'Creating file...', 0
    msgOpenWrite  db 'File opened for writing.', 0
    msgWrite      db 'Writing to file...', 0
    msgWriteOk    db 'Write successful. Bytes written: ', 0
    msgCloseWrite db 'Closing file after writing...', 0
    msgCloseOk    db 'File closed.', 0
    msgOpenRead   db 'Opening file for reading...', 0
    msgOpenReadOk db 'File opened for reading.', 0
    msgRead       db 'Reading from file...', 0
    msgReadOk     db 'Read successful. Bytes read: ', 0
    msgReadData   db 'Read data: [', 0
    msgReadDataEnd db ']', 0
    msgCloseRead  db 'Closing file after reading...', 0
    msgError      db 'Error occurred. Code: ', 0
    newLine       db '', 13, 10, 0       ; Newline sequence for printing

data ends

code segment
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax

        ; === STAGE 1: Create and Write to File ===

        Console_WriteTextWith0 msgCreate
        Console_WriteTextWith0 newLine
        File_Create filename, handler
        jc @CREATE_ERROR

        Console_WriteTextWith0 msgOpenWrite
        Console_WriteTextWith0 newLine

        ; Calculate length of text to write
        String_Len textToWrite, bytesToWrite

        Console_WriteTextWith0 msgWrite
        Console_WriteTextWith0 newLine
        File_Write handler, bytesToWrite, textToWrite, bytesWritten ; Pass length, get actual written
        jc @WRITE_ERROR ; Check Carry Flag for write error

        ; Check if bytes written matches bytes we intended to write (optional but good)
        mov ax, bytesToWrite
        cmp ax, bytesWritten
        jne @WRITE_PARTIAL_WARN ; Or treat as error if partial write is unacceptable

        Console_WriteTextWith0 msgWriteOk
        Int_ToString bytesWritten, numStr
        Console_WriteTextWith0 numStr
        Console_WriteTextWith0 newLine

        ; Close the file after writing
        Console_WriteTextWith0 msgCloseWrite
        Console_WriteTextWith0 newLine
        File_Close handler, successFlag ; Assuming File_Close might use a flag or CF
        ; jc @CLOSE_ERROR_WRITE ; Uncomment if File_Close sets CF on error

        ; Check successFlag if File_Close uses it (adapt based on krsv2.inc)
        cmp successFlag, 0
        jne @CLOSE_ERROR_WRITE
        Console_WriteTextWith0 msgCloseOk
        Console_WriteTextWith0 newLine
        Console_WriteTextWith0 newLine ; Extra blank line

        ; === STAGE 2: Open and Read from File ===

        Console_WriteTextWith0 msgOpenRead
        Console_WriteTextWith0 newLine
        ; Use File_Open (mode 0 = read)
        File_Open filename, 0, handler
        jc @OPEN_READ_ERROR

        Console_WriteTextWith0 msgOpenReadOk
        Console_WriteTextWith0 newLine

        Console_WriteTextWith0 msgRead
        Console_WriteTextWith0 newLine

        ; Use the improved File_Read_Improved macro
        File_Read_Improved handler, bytesToRead, readBuffer
        jc @READ_ERROR ; Check Carry Flag directly!

        ; --- Success Path for Read ---
        mov bytesRead, ax ; Store the number of bytes actually read

        Console_WriteTextWith0 msgReadOk
        Int_ToString bytesRead, numStr
        Console_WriteTextWith0 numStr
        Console_WriteTextWith0 newLine

        ; Null-terminate the buffer AFTER the read data for safe printing
        mov bx, bytesRead   ; BX = number of bytes read
        mov readBuffer[bx], 0 ; Add null terminator

        ; Print the read data
        Console_WriteTextWith0 msgReadData
        Console_WriteTextWith0 readBuffer
        Console_WriteTextWith0 msgReadDataEnd
        Console_WriteTextWith0 newLine

        ; Close the file after reading
        Console_WriteTextWith0 msgCloseRead
        Console_WriteTextWith0 newLine
        File_Close handler, successFlag ; Reuse flag or check CF
        ; jc @CLOSE_ERROR_READ ; Uncomment if File_Close sets CF on error
        cmp successFlag, 0
        jne @CLOSE_ERROR_READ
        Console_WriteTextWith0 msgCloseOk
        Console_WriteTextWith0 newLine

        jmp @FINAL_END ; All operations successful

        ; === Error Handling Sections ===

    @CREATE_ERROR:
        Console_WriteTextWith0 msgError
        Int_ToString ax, numStr ; AX contains error code from File_Create
        jmp @PRINT_ERROR_AND_EXIT

    @WRITE_ERROR:
        ; Here, CF was set by File_Write. AX likely contains the DOS error code.
        ; Note: The original code put the return value in 'writeResult',
        ; but standard DOS calls put the error code in AX when CF=1. Assuming File_Write follows this.
        Console_WriteTextWith0 msgError
        Int_ToString ax, numStr ; Display DOS error code from AX
        jmp @PRINT_ERROR_AND_EXIT

    @WRITE_PARTIAL_WARN:
        ; Handle case where fewer bytes were written than requested (disk full?)
        Console_WriteTextWith0 'Warning: Partial write. Expected '
        Int_ToString bytesToWrite, numStr
        Console_WriteTextWith0 numStr
        Console_WriteTextWith0 ', wrote '
        Int_ToString bytesWritten, numStr
        Console_WriteTextWith0 numStr
        Console_WriteTextWith0 newLine
        ; Decide whether to continue or exit
        jmp @CLOSE_ERROR_WRITE ; Treat as error for now, close file if handle valid

    @CLOSE_ERROR_WRITE:
        Console_WriteTextWith0 msgError
        ; If File_Close sets CF, AX has error. If uses flag, check flag value? Assume AX here.
        Int_ToString ax, numStr
        jmp @PRINT_ERROR_AND_EXIT

    @OPEN_READ_ERROR:
        Console_WriteTextWith0 msgError
        Int_ToString ax, numStr ; AX contains error code from File_Open
        jmp @PRINT_ERROR_AND_EXIT

    @READ_ERROR:
        ; Our improved macro sets CF=1 and puts DOS error code in AX
        Console_WriteTextWith0 msgError
        Int_ToString ax, numStr ; AX contains error code from File_Read_Improved
        jmp @PRINT_ERROR_AND_EXIT

    @CLOSE_ERROR_READ:
        Console_WriteTextWith0 msgError
        ; Similar to CLOSE_ERROR_WRITE
        Int_ToString ax, numStr
        jmp @PRINT_ERROR_AND_EXIT


    @PRINT_ERROR_AND_EXIT:
        Console_WriteTextWith0 numStr ; Print the error code already in numStr
        Console_WriteTextWith0 newLine
        ; Attempt to close file if handler might be valid? Risky. Best to exit.

    @FINAL_END:
        App_Exit

code ends
end start