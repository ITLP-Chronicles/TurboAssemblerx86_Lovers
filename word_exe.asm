; --- Include the library ---
INCLUDE krsv2.inc ; Assuming krsv2.txt is in the include path

data segment
    ;filename    db 'test.txt', 0        ; Null-terminated filename
    handler     dw 0                    ; File handle storage
    closeSuccess db 0                   ; Success flag for File_Close
    ;textToWrite db 'Hello, world!', 0   ; Text to write, null-terminated
    bytesToWrite dw 0                   ; To store length of textToWrite
    bytesWritten dw 0                   ; Result of File_Write (bytes written or error code)
    bytesToRead  dw 50                  ; Max bytes we want to try reading
    bytesRead    dw 0                   ; To store actual bytes read from file
    readBuffer   db 100 dup(0)          ; Buffer to store read text, initialized to 0
                                        ; Ensure size >= bytesToRead + 1 for null terminator

    numStr      db 7 dup(0)             ; Buffer for Int_ToString conversion (6 digits + null)

    ; --- NEW VARIABLES ---
    userFilename db 50 dup(0)          ; Buffer for user-provided filename (adjust size as needed)
    userTextToWrite db 100 dup(0)       ; Buffer for user-provided text (adjust size as needed)

    ; Messages (all null-terminated for Console_WriteTextWith0)
    msgCreate     db 'Enter filename: ', 0
    msgOpenWrite  db 'File created/truncated.', 0
    msgWrite      db 'Enter text to write: ', 0
    msgWriteOk    db 'Write successful. Bytes written: ', 0
    msgCloseWrite db 'Closing file after writing...', 0
    msgCloseOk    db 'File closed successfully.', 0
    msgOpenRead   db 'Opening file for reading...', 0
    msgOpenReadOk db 'File opened for reading.', 0
    msgRead       db 'Reading from file...', 0
    ; --- DEBUG MESSAGE ADDED ---
    msgAxAfterRead db 'DEBUG: AX after File_Read = ', 0
    ; --- END DEBUG MESSAGE ---
    msgReadOk     db 'Read successful. Stored bytes read: ', 0 ; Clarified message
    msgReadData   db 'Read data: [', 0
    msgReadDataEnd db ']', 0
    msgCloseRead  db 'Closing file after reading...', 0
    msgError      db 'Error occurred. Code: ', 0
    newLine       db 13, 10, 0          ; CRLF sequence for Console_WriteTextWith0

data ends

code segment
    .386 ; Assuming 386+ for some library features if needed, safe otherwise
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax

        ; === STAGE 1: Create and Write to File ===
        ; (Same as before, but with user input)
        Console_WriteTextWith0 msgCreate
        Console_WriteTextWith0 newLine
        Console_ReadTextWith0 userFilename, 49 ; Read filename from user (max 49 chars + null)
        File_Create userFilename, handler ; Use File_Create with user-provided filename
        
        jc @CREATE_ERROR            ; Check Carry Flag for error
        Console_WriteTextWith0 msgOpenWrite
        Console_WriteTextWith0 newLine
        
        Console_WriteTextWith0 msgWrite
        Console_WriteTextWith0 newLine
        Console_ReadTextWith0 userTextToWrite, 99 ; Read text from user (max 99 chars + null)
        String_Len userTextToWrite, bytesToWrite ; Get length of user-provided text
        File_Write handler, bytesToWrite, userTextToWrite, bytesWritten ; Use File_Write
        jc @WRITE_ERROR ; Check Carry Flag for write error
        Console_WriteTextWith0 msgWriteOk
        Int_ToString bytesWritten, numStr ; Use Int_ToString
        Console_WriteTextWith0 numStr
        Console_WriteTextWith0 newLine
        Console_WriteTextWith0 msgCloseWrite
        Console_WriteTextWith0 newLine
        File_Close handler, closeSuccess ; Pass handle and success flag variable
        cmp closeSuccess, 1
        jne @CLOSE_ERROR_WRITE
        Console_WriteTextWith0 msgCloseOk
        Console_WriteTextWith0 newLine
        Console_WriteTextWith0 newLine ; Extra blank line

        ; === STAGE 2: Open and Read from File ===

        Console_WriteTextWith0 msgOpenRead
        Console_WriteTextWith0 newLine
        File_Open userFilename, handler, 0  ; Pass user-provided filename, handler var, mode 0
        jc @OPEN_READ_ERROR         ; Check Carry Flag for error

        Console_WriteTextWith0 msgOpenReadOk
        Console_WriteTextWith0 newLine

        Console_WriteTextWith0 msgRead
        Console_WriteTextWith0 newLine

        ; Use the File_Read macro from the library
        File_Read handler, bytesToRead, readBuffer ; Pass handle, max bytes, buffer
        jc @READ_ERROR ; Check Carry Flag directly!

        ; --- Success Path for Read (Continues) ---
        mov bytesRead, ax ; Store the number of bytes actually read (from AX)

        Console_WriteTextWith0 msgReadOk
        Int_ToString bytesRead, numStr ; Use Int_ToString (displaying the stored value)
        Console_WriteTextWith0 numStr
        Console_WriteTextWith0 newLine

        ; Null-terminate the buffer AFTER the read data for safe printing
        mov bx, bytesRead   ; Use BX as index (value from AX)
        mov readBuffer[bx], 0 ; Add null terminator

        ; Print the read data using Console_WriteTextWith0
        Console_WriteTextWith0 msgReadData
        Console_WriteTextWith0 readBuffer ; Print the null-terminated buffer
        Console_WriteTextWith0 msgReadDataEnd
        Console_WriteTextWith0 newLine

        ; Close the file after reading using File_Close
        Console_WriteTextWith0 msgCloseRead
        Console_WriteTextWith0 newLine
        File_Close handler, closeSuccess ; Reuse success flag variable
        cmp closeSuccess, 1
        jne @CLOSE_ERROR_READ
        Console_WriteTextWith0 msgCloseOk
        Console_WriteTextWith0 newLine 

        jmp @FINAL_END ; All operations successful

        ; === Error Handling Sections ===
        ; (Same as before)
    @CREATE_ERROR:
        Console_WriteTextWith0 msgError
        Int_ToString ax, numStr
        jmp @PRINT_ERROR_AND_EXIT
    @WRITE_ERROR:
        Console_WriteTextWith0 msgError
        Int_ToString ax, numStr
        jmp @PRINT_ERROR_AND_EXIT
    @CLOSE_ERROR_WRITE:
        Console_WriteTextWith0 msgError
        mov ax, 6 ; Assume error 6 for display purposes
        Int_ToString ax, numStr
        jmp @PRINT_ERROR_AND_EXIT
    @OPEN_READ_ERROR:
        Console_WriteTextWith0 msgError
        Int_ToString ax, numStr
        jmp @PRINT_ERROR_AND_EXIT
    @READ_ERROR:
        Console_WriteTextWith0 msgError
        Int_ToString ax, numStr
        jmp @PRINT_ERROR_AND_EXIT
    @CLOSE_ERROR_READ:
        Console_WriteTextWith0 msgError
        mov ax, 6 ; Assume error 6 for display purposes
        Int_ToString ax, numStr
        jmp @PRINT_ERROR_AND_EXIT

    @PRINT_ERROR_AND_EXIT:
        Console_WriteTextWith0 numStr ; Print the error code already in numStr
        Console_WriteTextWith0 newLine

    @FINAL_END:
        App_Exit ; Use App_Exit macro

code ends
end start