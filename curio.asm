INCLUDE krsv2.inc

data segment
    ;================= MENU =======================
    menu_line1 db "Men", 0A3h, " de Cadenas", 0
    menu_line2 db "1. ", 0A8h, "Es pal", 0A1h, "ndromo?", 0
    menu_line3 db "2. N", 0A3h, "mero de vocales", 0
    menu_line4 db "3. Obtener subcadena", 0
    menu_line5 db "4. ?", 0
    menu_line6 db "<ESC> Para Salir", 0
    input_prompt db "Enter your choice: ", 0
    not_available_msg db "Opci", 0A2h, "n no disponible", 0

    ; Define the input buffer (5 characters + 1 for null terminator)
    input_buffer db 6 dup(0)  ; Reserve 6 bytes, initialized to 0
    
    ;================== PALINDROME ===================
    palindrome_line1 db 0A8h, "Es pal", 0A1h, "ndromo?", 0
    palindrome_line2 db "Cadena: ", 0
    palindrome_yes   db "S", 0A1h, " es Pal", 0A1h, "ndromo", 0
    palindrome_not   db "No es Pal", 0A1h, "ndromo", 0
    palindrome_input db 20 dup(0)  ; Reserve 20 bytes for input string
    
    ;================== Vowel Counter ===================
    vowel_line1 db "N", 0A3h, "mero de vocales", 0
    vowel_line2 db "Cadena: ", 0
    vowel_line3 db " vocales", 0
    vowel_input db 16 dup(0)  ; Reserve 16 bytes for input string
    
    ;================== Substring Extraction ===================
    substring_line1 db "Obtener subcadena", 0
    substring_line2 db "Cadena: ", 0
    substring_line3 db "Inicio: ", 0
    substring_line4 db "T", 0A2h, "rmino: ", 0  ; T?rmino (? = 0A2h in code page 437)
    substring_line5 db "Subcadena = ", 0
    substring_input db 21 dup(0)  ; Reserve 21 bytes for input string
    start_input db 3 dup(0)       ; Reserve 3 bytes for start index
    end_input db 3 dup(0)         ; Reserve 3 bytes for end index
    substring_result db 16 dup(0) ; Reserve 16 bytes for the substring
    
data ends

code segment 
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        ;=== Start Code ===
@MAIN:
        Theme_SetRed
        Draw_Menu 0, 0  ; Draw the menu on the left side (column 0)
        Console_MoveCursor 10, 1  ; Position cursor for input prompt
        Console_WriteTextWith0 input_prompt
        Console_ReadTextWith0 input_buffer, 5  ; Read user input
        
        ; Check the input
        mov al, input_buffer[0]
        
        ; Check for ESC (ASCII 27)
        cmp al, 27
        je @EXIT
        
        ; Check for option '1' (Palindrome Check)
        cmp al, '1'
        jne @CHECK_OPTION_2
        Clear_Rectangle 0, 26, 54, 25  ; Clear the right side (columns 26 to 79)
        Draw_PalindromeCheck 0, 26     ; Draw on the right side
        jmp @MAIN
        
@CHECK_OPTION_2:
        ; Check for option '2' (Vowel Count)
        cmp al, '2'
        jne @CHECK_OPTION_3
        Clear_Rectangle 0, 26, 54, 25
        Draw_VowelCount 0, 26
        jmp @MAIN
        
@CHECK_OPTION_3:
        ; Check for option '3' (Substring Extraction)
        cmp al, '3'
        jne @CHECK_OPTION_4
        Clear_Rectangle 0, 26, 54, 25
        Draw_SubstringInput 0, 26
        jmp @MAIN
        
@CHECK_OPTION_4:
        ; Check for option '4' (Not available)
        cmp al, '4'
        jne @MAIN  ; If not '4', loop back to menu
        Console_MoveCursor 12, 1  ; Position for message
        Console_WriteTextWith0 not_available_msg
        jmp @MAIN
        
@EXIT:
        ;=== End Code ====
        App_Exit
code ends
end start

; Clears a rectangle area on the screen by filling it with spaces
Clear_Rectangle macro rowIndex, columnIndex, width, height
    push ax
    push bx
    push cx
    push dx
    
    mov ch, rowIndex
    mov cl, columnIndex
    mov bl, height
@CLEAR_ROW:
    Cursor_MoveTo ch, cl
    mov dl, width
    mov al, ' '  ; Space character to clear
@CLEAR_COL:
    Console_FillChar al
    dec dl
    jnz @CLEAR_COL
    inc ch
    dec bl
    jnz @CLEAR_ROW
    
    pop dx
    pop cx
    pop bx
    pop ax
endm

; Draws the menu on the left side of the screen
Draw_Menu macro rowIndex, columnIndex
    push ax
    push bx
    push cx
    push dx
    
    ; Draw the rectangle (width=26, height=10)
    Draw_Rectangle rowIndex, columnIndex, 26, 10
    
    ; Position text inside the rectangle
    mov ch, rowIndex
    mov cl, columnIndex
    inc ch  ; Top padding
    add cl, 1  ; Left padding
    
    ; Line 1: "Men? de Cadenas"
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 menu_line1
    
    ; Line 2: "1. ?Es pal?ndromo?"
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 menu_line2
    
    ; Line 3: "2. N?mero de vocales"
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 menu_line3
    
    ; Line 4: "3. Obtener subcadena"
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 menu_line4
    
    ; Line 5: "4. ?"
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 menu_line5
    
    ; Line 6: "<ESC> Para Salir"
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 menu_line6
    
    pop dx
    pop cx
    pop bx
    pop ax
endm

; Macro to check if a string is a palindrome
Is_Palindrome macro stringWith0
    local @CHECK_LOOP, @IS_PALINDROME, @NOT_PALINDROME, @END
    
    push si
    push di
    push ax
    push bx
    push cx
    
    ; Find the length of the string
    mov si, offset stringWith0
    mov cx, 0  ; Length counter
@FIND_LENGTH:
    mov al, [si]
    cmp al, 0
    je @LENGTH_DONE
    inc si
    inc cx
    jmp @FIND_LENGTH
@LENGTH_DONE:
    dec si  ; Point to last character before null
    mov di, offset stringWith0  ; Point to first character
    
    ; Check if string is empty or has length 1
    cmp cx, 1
    jle @IS_PALINDROME
    
    ; Compare characters from start and end
    shr cx, 1  ; Divide length by 2
@CHECK_LOOP:
    mov al, [di]
    mov bl, [si]
    cmp al, bl
    jne @NOT_PALINDROME
    inc di
    dec si
    loop @CHECK_LOOP
    
@IS_PALINDROME:
    mov al, 1  ; Return 1 if palindrome
    jmp @END
    
@NOT_PALINDROME:
    mov al, 0  ; Return 0 if not palindrome
    
@END:
    pop cx
    pop bx
    pop ax
    pop di
    pop si
endm

; Macro to draw the palindrome check interface
Draw_PalindromeCheck macro rowIndex, columnIndex
    local @END, @SHOW_YES, @SHOW_NOT
    push ax
    push bx
    push cx
    push dx
    
    ; Draw the rectangle (width=30, height=8)
    Draw_Rectangle rowIndex, columnIndex, 30, 8
    
    mov ch, rowIndex
    mov cl, columnIndex
    inc ch  ; Top padding
    add cl, 4  ; Left padding
    
    ; Line 1: "?Es pal?ndromo?"
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 palindrome_line1
    
    ; Line 2: "Cadena: " followed by user input
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 palindrome_line2
    add cl, 8  ; Move past "Cadena: "
    Cursor_MoveTo ch, cl
    Console_ReadTextWith0 palindrome_input, 15
    
    ; Line 3: Blank row
    inc ch
    
    ; Line 4: Display result
    inc ch
    mov cl, columnIndex
    add cl, 4
    Cursor_MoveTo ch, cl
    
    ; Check if the input is a palindrome
    Is_Palindrome palindrome_input
    cmp al, 1
    je @SHOW_YES
    
@SHOW_NOT:
    Console_WriteTextWith0 palindrome_not
    jmp @END
    
@SHOW_YES:
    Console_WriteTextWith0 palindrome_yes
    
@END:
    pop dx
    pop cx
    pop bx
    pop ax
endm

; Macro to count vowels in a string
Count_Vowels macro stringWith0
    local @CHECK_LOOP, @NOT_VOWEL, @IS_VOWEL, @END_COUNT
    push si
    push cx
    
    mov si, 0  ; Index into the string
    mov cx, 0  ; Vowel counter
    
@CHECK_LOOP:
    mov al, stringWith0[si]
    cmp al, 0  ; Check for null terminator
    je @END_COUNT
    
    ; Check for vowels (lowercase and accented)
    cmp al, 'a'
    je @IS_VOWEL
    cmp al, 0A0h  ; ?
    je @IS_VOWEL
    cmp al, 'e'
    je @IS_VOWEL
    cmp al, 082h  ; ?
    je @IS_VOWEL
    cmp al, 'i'
    je @IS_VOWEL
    cmp al, 0A1h  ; ?
    je @IS_VOWEL
    cmp al, 'o'
    je @IS_VOWEL
    cmp al, 0A2h  ; ?
    je @IS_VOWEL
    cmp al, 'u'
    je @IS_VOWEL
    cmp al, 0A3h  ; ?
    je @IS_VOWEL
    
    jmp @NOT_VOWEL

@IS_VOWEL:
    inc cx

@NOT_VOWEL:
    inc si
    jmp @CHECK_LOOP

@END_COUNT:
    mov al, cl  ; Return vowel count in al
    pop cx
    pop si
endm

; Macro to draw the vowel count interface
Draw_VowelCount macro rowIndex, columnIndex
    local @END
    push ax
    push bx
    push cx
    push dx
    
    ; Draw the rectangle (width=30, height=8)
    Draw_Rectangle rowIndex, columnIndex, 30, 8
    
    mov ch, rowIndex
    mov cl, columnIndex
    inc ch  ; Top padding
    add cl, 4  ; Left padding
    
    ; Line 1: "N?mero de vocales"
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 vowel_line1
    
    ; Line 2: "Cadena: " followed by user input
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 vowel_line2
    add cl, 8
    Cursor_MoveTo ch, cl
    Console_ReadTextWith0 vowel_input, 15
    
    ; Line 3: Blank row
    inc ch
    
    ; Line 4: Display vowel count
    inc ch
    mov cl, columnIndex
    add cl, 4
    Cursor_MoveTo ch, cl
    
    ; Count the vowels
    Count_Vowels vowel_input
    mov bl, al
    add bl, '0'  ; Convert count to ASCII
    Console_WriteChar bl
    Console_WriteTextWith0 vowel_line3
    
@END:
    pop dx
    pop cx
    pop bx
    pop ax
endm

; Macro to extract a substring from a given string
Extract_Substring macro source, startIndex, endIndex, destination
    local @COPY_LOOP, @END_COPY, @TERMINATE, @SKIP_LOOP
    push ax
    push bx
    push cx
    push si
    push di
    
    ; Initialize pointers
    mov si, offset source    ; Source string pointer
    mov di, offset destination ; Destination buffer pointer
    
    ; Check if startIndex is 0
    mov bl, startIndex
    cmp bl, 0
    je @COPY_LOOP
    
    ; Skip characters until startIndex
    xor cx, cx
@SKIP_LOOP:
    mov al, [si]
    cmp al, 0
    je @TERMINATE
    
    inc si
    inc cx
    
    cmp cl, bl
    jb @SKIP_LOOP
    
@COPY_LOOP:
    mov al, [si]
    cmp al, 0
    je @TERMINATE
    
    cmp cl, endIndex
    ja @TERMINATE
    
    mov al, [si]
    mov [di], al
    
    inc si
    inc di
    inc cx
    
    jmp @COPY_LOOP
    
@TERMINATE:
    mov byte ptr [di], 0
    
@END_COPY:
    pop di
    pop si
    pop cx
    pop bx
    pop ax
endm

; Macro to draw the substring input interface
Draw_SubstringInput macro rowIndex, columnIndex
    local @END
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    
    ; Draw the rectangle (width=30, height=8)
    Draw_Rectangle rowIndex, columnIndex, 30, 8
    
    mov ch, rowIndex
    mov cl, columnIndex
    inc ch
    add cl, 4
    
    ; Line 1: "Obtener subcadena"
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_line1
    
    ; Line 2: "Cadena: " followed by user input
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_line2
    add cl, 8
    Cursor_MoveTo ch, cl
    Console_ReadTextWith0 substring_input, 20
    
    ; Line 3: "Inicio: " followed by user input
    inc ch
    mov cl, columnIndex
    add cl, 4
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_line3
    add cl, 8
    Cursor_MoveTo ch, cl
    Console_ReadTextWith0 start_input, 2
    
    ; Line 4: "T?rmino: " followed by user input
    inc ch
    mov cl, columnIndex
    add cl, 4
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_line4
    add cl, 9
    Cursor_MoveTo ch, cl
    Console_ReadTextWith0 end_input, 2
    
    ; Line 5: Blank row
    inc ch
    
    ; Line 6: Display the substring
    inc ch
    mov cl, columnIndex
    add cl, 4
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_line5
    
    ; Convert start and end indices from ASCII to numbers and adjust to 0-based
    mov al, start_input[0]
    sub al, '0'
    dec al
    mov bl, al
    
    mov al, end_input[0]
    sub al, '0'
    dec al
    mov bh, al
    
    ; Extract the substring
    Extract_Substring substring_input, bl, bh, substring_result
    
    ; Display the result
    add cl, 12
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_result
    
@END:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
endm