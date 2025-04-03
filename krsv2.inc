;================  Kristan Library  ========================

; Writes only a char/number to console output
; @NotDestructive
; @param car | 1byte | Char/Int number in ascii to display
; @returns void
Console_WriteChar macro car
    pusha
    mov ah, 02h
    mov dl, car
    int 21h
    popa
endm

; Writes a new line (\r\n) and moves cursor to there to console output
; @NotDestructive
; @returns void
Console_WriteBlankLine macro
    Console_WriteChar 13
    Console_WriteChar 10
endm

; Writes a full string text variable to console output
; @NotDestructive
; @param variableReference | (# bytes) | The reference to string (array of bytes)
Console_Write macro variableReference
    pusha
    mov ah, 09h
    lea dx, variableReference
    int 21h
    popa
endm

; Writes a full string text variable to console output then print a new line
; @NotDestructive
; @param variableReference |(# bytes) | The reference to string (array of bytes)
; @returns void
Console_WriteLine macro variableReference
    Console_Write variableReference
    Console_WriteBlankLine
endm

; Exits the program successfully
; @NotDestructive
; @returns void
App_Exit macro
    pusha
    mov ax, 4c00h
    int 21h
    popa
endm

; Move the actual cursor on screen to given row and column
; @NotDestructive
; @param row | (1 byte) | The int number index row to go
; @param column | (1 byte) | The int number index column to go
; @returns void
Cursor_MoveTo macro row, column
   pusha
   mov ah,02h
   mov bh,0  
   mov dh,row
   mov dl,column
   int 10h
   popa
endm

; Paints the screen with the desired color code
; @Destructive
; @param color | (1 byte) | The color code to paints the screen
; @returns void
Screen_Clear macro color
   mov ax,0600h    
   mov bh,color 
   mov cx,0000h
   mov dx,184fh
   int 10h
endm

; Set the screen with a predefined blue color
; @Destructive
; @returns void
Theme_SetBlue macro
    Screen_Clear 1Fh
endm

; Set the screen with a soft red background and white text
; @Destructive
; @returns void
Theme_SetRed macro
    Screen_Clear 4Fh
endm

; Read only a char from the user in console output and then print it as well
; @NotDestructive
; @param car | (1 byte) | Reference to the byte to store the user input char
; @returns (1 byte) letter char in ascii code
Console_ReadChar macro car
    pusha
    mov ah, 01h
    int 21h
    mov car, al
    popa
endm

; Read only a char from the user in console output and then print it as well
; @Destructive
; @returns (1 byte) letter char in ascii code in AL registry
Console_ReadCharNative macro
    mov ah, 01h
    int 21h
endm

; Reads a full text in the bufferText
; @NotDestructive
; @param bufferText | (bytes[]) | has to be length of expected string + 1 (for 0) ;Improving this
; @param maxchars | (1 byte) | Uint number from the max chars admitted from the user (maxlength to get from buffer text. You can limit that with this variable
; @returns (bytes[]) the text inserted by the user in bufferText variable
Console_ReadText macro bufferText, maxChars
    local cycle, end_tag, max_chars, delete
    pusha
    mov si, 0
cycle:
    Console_ReadCharNative
    cmp al, 13 ; Is it enter?
    je end_tag
    
    cmp al, 08h ; Is it backspace?
    je delete 
    
    mov bufferText[si], al
    inc si
    cmp si, maxChars ; Has already reached max chars?
    jae max_chars

    jmp cycle
delete:
    cmp si, 0
    je cycle
    
    dec si
    Console_WriteChar ' ' ;blankdigit!
    Console_WriteChar 08
    jmp cycle
max_chars:
    Console_WriteBlankLine
    jmp end_tag
end_tag:
    mov bufferText[si], '$' ;
    popa
endm

; Converts an int (UINT) to it's string representation in ascii
; Can be only printable with Console_WriteTextWith0 ....
; @Destructive
; @param intValue | (2 byte) | The int value to represent as string
; @param stringtoStore | (byte[]) | The variable where to store the string representation
; @returns (byte[]) The ascii representation in stringToStore variable
Int_ToStringRaw macro intValue, stringToStore, includeMinus
      local cycle, anothercycle, exit, withMinus, withNoMinus
      mov ax, intValue ;Clean the right part ;bits number!
      push '$' ;Finish flag
    cycle:
        mov dx, 0 ;Clean left part           ;32
        mov bx, 10 ;Divide by ten
        div bx
        push dx ; Push rightest number
        cmp ax, 0 ; Check if can't be divided by 10 anymore
        jne cycle
        
        mov cl, includeMinus
        cmp cl, 1
        je withMinus
        jne withNoMinus
    withMinus:
        mov stringToStore[0], '-'
        mov si, 1
        jmp anothercycle
    withNoMinus:
        mov si, 0
anothercycle:
        pop cx ; Pop the last item from the stack
        cmp cl, '$' ;Check if its flag '$' (end of stack)
        je exit
        mov stringToStore[si], cl ; Store the item popped
        add stringToStore[si], '0' ; Convert it into a digit
        inc si ;Go to next item in str array (and stack)
        jmp anothercycle ; Repeat
    exit:
        mov stringToStore[si + 1], 0 ;Set the last item
endm

; Converts an int (negative or positive) into its string representation. (Admits 2 bytes numbers only)
; @param intValue (2 BYTES!) 
; @param stringToStore (byte[])
Int_ToString macro intValue, stringToStore
    local isPositive, isNegative, final
    mov ax, 8000h ;1000 0000 0000 0000 
    mov bx, intValue
    test bx, ax 
    jz isPositive
    jnz isNegative
isPositive:
    Int_ToStringRaw intValue, stringToStore, 0 
    jmp final
isNegative:
    neg intValue
    Int_ToStringRaw intValue, stringToStore, 1
    jmp final
final:
endm

; Check if a number is negative
; @NotDestructive
; @param intValue | (2 byte) | The value to check if positive or negative
; @param boolean | (1 byte) | where to store if positive or negative
; @returns (1 byte!!) boolean. Returns true if negative, otherwise false as (1 or 0)
Int_IsNegative macro intValue, boolean
    local isPositive2, isNegative2, finalize
    pusha
    mov ax, 8000h
    test intValue, ax
    jz isPositive2
    mov boolean, 1
    jmp finalize
isPositive2:
    mov boolean, 0
finalize:
    popa
endm

; Reads a full text in the bufferText with 0 implementation
; @NotDestructive
; @param bufferText | (bytes[]) | has to be length of expected string + 1 (for 0) ;Improving this
; @param maxchars | (2 bytes number) | Uint number from the max chars admitted from the user (maxlength to get from buffer text. You can limit that with this variable
; @returns (bytes[]) the text inserted by the user in bufferText variable
Console_ReadTextWith0 macro bufferText, maxChars
    local cycle, end_tag, max_chars, delete
    pusha
    mov si, 0
cycle:
    Console_ReadCharNative
    cmp al, 13 ; Is it enter?
    je end_tag
    
    cmp al, 08h ; Is it backspace?
    je delete 
    
    mov bufferText[si], al
    inc si
    cmp si, maxChars ; Has already reached max chars?
    jae max_chars

    jmp cycle
delete:
    cmp si, 0
    je cycle
    
    dec si
    Console_WriteChar ' ' ;blankdigit!
    Console_WriteChar 08
    jmp cycle
max_chars:
    Console_WriteBlankLine
    jmp end_tag
end_tag:
    mov bufferText[si], 0 ; Changed here!
    popa
endm

;Allows you to pow any number (positive) to any exponent (positive)
;@note You can change the size, but all three params have to be SAME byte SIZE
;@NotDestructive
;@param number | (2 bytes) | Int value to elevate
;@param exponent | (2 bytes) | Only positives allowed so far
;@param toStoreResultNum | (2 bytes) | (int num)/ref to store the result 
;@returns The result of pow in toStoreResultNum
Math_Pow macro number, exponent, toStoreResultNum
    local zero_case, one_case, normal_case, exponent_loop, end_macro
    push ax
    push bx
    push cx
    push si
    
    mov ax, number
    mov cx, exponent          ; Use exponent for comparison, not toStoreResultNum
    mov toStoreResultNum, ax  ; Initialize result to number
    cmp cx, 0
    je zero_case 
    cmp cx, 1
    je one_case
    jmp normal_case 
zero_case:
    mov toStoreResultNum, 1   ; number^0 = 1
    jmp end_macro
one_case:
    mov ax, number            ; number^1 = number (redundant but kept for clarity)
    mov toStoreResultNum, ax
    jmp end_macro
normal_case:
    mov si, 1                 ; Start loop counter at 1 (using si to avoid overwriting cx)
exponent_loop:
    mov ax, toStoreResultNum  ; Load current result
    mov bx, number            ; Load full 16-bit number (fixed from bl)
    mul bx                    ; Multiply: result in dx:ax, assume dx=0 (fits in 16 bits)
    mov toStoreResultNum, ax  ; Store low 16 bits
    inc si                    ; Increment counter
    cmp si, exponent          ; Compare with exponent
    jb exponent_loop          ; Loop if si < exponent (runs exponent-1 times)
end_macro:
    pop si
    pop cx
    pop bx
    pop ax
endm

; Writes a text from a string variable DIRECTLY (Not Pointer) that was generated with Console_ReadTextWith0!\
; @Destructive
; @param stringWith0 | (bytes[]) | The variable returned by Console_ReadTextWith0
Console_WriteTextWith0 macro stringWith0
    local cycle, end_code
    mov si, 0
cycle:
    cmp stringWith0[si], 0
    je end_code
    Console_WriteChar stringWith0[si]
    inc si
    jmp cycle
end_code:
    ;yay, end code
endm

; Writes a text from a POINTER to a string, terminated with 0 or 1
; @NotDestructive
; @param stringPtr | (word) | Pointer to the string (array of bytes ending with 0)
; @returns void
Console_WriteTextPointerWith0 macro stringPtr
    pusha
    mov si, stringPtr       ; Load the pointer into SI
Console_WriteNull_Loop:
    mov al, [si]            ; Load the current character
    cmp al, 0               ; Check for null terminator
    je Console_WriteNull_End ; Exit if 0 is found
    cmp al, 1
    je Console_WriteNull_End
    Console_WriteChar al    ; Print the character
    inc si                  ; Move to next character
    jmp Console_WriteNull_Loop
Console_WriteNull_End:
    popa
endm

; Parse a string into a value int
;@Destructive
;@param numberStrWith0 (byte[]) This must be given by Console_ReadTextWith0!or ends with ascii 0 (not char '0')
;@param num HAS TO BE (2 BYTES)
Int_Parse macro num, numberStrWith0
    local parse_loop, end_parse, isNegative, isNegative2, final
    mov si, 0  
    mov bx, 0  ;Accumulator
    
    ; Check if first is '-'
    cmp numberStrWith0[si], '-'
    je isNegative
    jmp parse_loop
isNegative:
    mov si, 1
parse_loop: ; else
    mov al, numberStrWith0[si]  
    cmp al, 0                   ; Check if end (0)
    je end_parse                ; If null, parsing is complete
    
    ; Convert character to digit
    mov ah, 0                   ; Clear high byte for 16-bit addition
    sub al, '0'                 ; Convert
    
    ; Update accumulator: num = num * 10 + digit
    push ax  ; Save the digit
    mov ax, bx  ; Move current accumulator to ax
    mov cx, 10  ; Multiplier
    mul cx; ax = bx * 10 (dx assumed 0, fits in 16 bits)
    mov bx, ax  ; Update accumulator with scaled value
    pop ax   ; Restore the digit
    add bx, ax  ; Add the new digit to accumulator
    
    inc si                      
    jmp parse_loop ; Continue parsing
end_parse:
    mov si, 0
    cmp numberStrWith0[si], '-'
    je isNegative2
    jmp final
isNegative2:
    neg bx
final:
    mov num, bx    
endm

; SPLIT STANDARD 0, array of letters separated by 0 (words) and 1 in the end
; Splits a string of words separated and terminated by 0
; @NotDestructive
; @example
;    'hola', 0, 'mundo', 1
;    will return  
;    [2bytespointer to h, 2bytespointer = m] (pointers to those letters)
; @howtouse:
; (in data segment)
;        alltexts db 'Suma', 0, 'Another', 0, 'XD', 1
;        allPointers dw 3 dup(0) 

; (in code segment)
;        String_Split alltexts, allPointers 
;        mov si, allPointers[2 * 2]  <= Need to shift 4 bytes (00h, 00h, <here>00h0)
;        Console_WriteChar [si+1] <= Prints 'n' from 'Another'
;
; @param stringWith0 (byte[]) | String with words separated by 0, ends with 1.
;     =>@example: alltexts db 'Suma', 0, 'Another', 0, 'XD', 1
; @param ptrArray ((2bytes/word)[]) | Array to store 2-byte pointers to the split words
String_Split MACRO stringWith0, ptrArray
    LOCAL SplitLoop, EndMacro, FoundZero, FoundEnd, StoreWord
    
    pusha
    mov si, 0           ; Index into stringWith0
    mov di, 0           ; Index into ptrArray
    mov bx, offset stringWith0 ; Base address of the string

    ; Store the first word's absolute address
    mov ptrArray[di], bx ; Store absolute address of stringWith0 start
    add di, 2           ; Move to next pointer slot

SplitLoop:
    mov al, [bx + si]   ; Load current character
    cmp al, 0           ; Is it a word separator?
    je FoundZero
    cmp al, 1           ; Is it the terminator?
    je FoundEnd
    inc si              ; Move to next character
    jmp SplitLoop

FoundZero:
    inc si              ; Skip the 0
    mov al, [bx + si]   ; Check next character
    cmp al, 0           ; Another 0 means end
    je FoundEnd
    cmp al, 1           ; Alternative terminator
    je FoundEnd
    ; Store absolute address of the next word
StoreWord:
    lea ax, [bx + si]   ; Compute absolute address (bx + si)
    mov ptrArray[di], ax ; Store it in ptrArray
    add di, 2           ; Move to next pointer slot
    jmp SplitLoop

FoundEnd:
    jmp EndMacro        ; End of string

EndMacro:
    popa
ENDM

; Calculates the length of an array of string pointers (strings separated by 0, terminated by 1)
; Example: 'Suma', 0, 'Another', 1 => 2 elements ["Suma", "Another"]
; @NotDestructive
; @param ptrArray | ((2bytes)[]) | Array of pointers to strings
; @param counter | (1 bytes) | Variable to store the length
; @returns AX | Number of valid pointers (length)
Get_PointerArray_Length macro ptrArray, counter
    LOCAL LoopStart, CheckString, CheckNext, FoundEnd, MacroEnd
    
    pusha
    mov si, 0           ; Index into ptrArray (in bytes)
    mov cx, 0           ; Counter for length

LoopStart:
    mov bx, ptrArray[si] ; Load pointer
    cmp bx, 0           ; End of array if pointer is 0
    je FoundEnd

    mov di, bx          ; DI = start of string
CheckString:
    mov al, [di]        ; Load character
    cmp al, 0           ; End of current string?
    je CheckNext
    cmp al, 1           ; Alternative end (terminator)?
    je FoundEnd
    inc di              ; Next character
    jmp CheckString

CheckNext:
    inc di              ; Move past the 0
    mov al, [di]        ; Check next character
    cmp al, 1           ; Terminator?
    je FoundEnd
    inc cx              ; Count this string
    add si, 2           ; Next pointer
    jmp LoopStart

FoundEnd:
    inc cx              ; Count the last string
    mov ax, cx          ; Return length in AX
    mov counter, al     ; Store in counter
    jmp MacroEnd

MacroEnd:
    popa
endm


;Palabra es un arreglo bytes y es la palabra a verificar que sea palindroma
;esPalindromo es 1 byte para verificar que lo sea o no
CheckPalindrome macro palabra, esPalindromo
    local contar, fin_contar, comparar, es_palindromo, no_es_palindromo, fin_macro
    pusha
    
    lea si, palabra
    
    mov cx, 0
    mov bx, si       
    
    contar:
        mov al, [bx] 
        cmp al, 0        
        je fin_contar
        inc cx           
        inc bx           
        jmp contar
    fin_contar:
        dec bx           
        mov di, bx       
    comparar:
        mov al, [si]     
        mov ah, [di]     
        cmp al, ah       
        jne no_es_palindromo
        inc si          
        dec di           
        loop comparar    
    es_palindromo:
        mov esPalindromo, 1 
        jmp fin_macro
    no_es_palindromo:
        mov esPalindromo, 0  
    fin_macro:
        popa   
ENDM

; @param x 1 byte
; @param y 1 byte
; @param width 1 byte
; @param heigth 1 byte
; @free_to_use ax!, bx!
Draw_Rectangle macro rowIndex, columnIndex, width, height
    local @TOP, @MIDDLE, @BOTTOM
    push ax
    push bx
    push cx
    push dx
    
    ; Set initial coordinates
    mov ch, rowIndex    ; Row (y)
    mov cl, columnIndex ; Column (x)
    
    ; Set size
    mov dh, width       ; Width
    mov dl, height      ; Height
    
    ; Draw top row
    Cursor_MoveTo ch, cl
    Console_WriteChar 0C9h  ; ? (top-left corner)
    
    ; Draw top horizontal line
    push dx
    mov dh, width
@TOP:
    Console_WriteChar 0CDh  ; ? (horizontal line)
    dec dh
    cmp dh, 0
    ja @TOP
    pop dx
    
    Console_WriteChar 0BBh  ; ? (top-right corner)
    
    ; Draw middle section (vertical lines)
    push dx
    mov dl, height
@MIDDLE:
    inc ch                  ; Move to next row
    Cursor_MoveTo ch, cl    ; Left side position
    Console_WriteChar 0BAh  ; ? (left vertical)
    
    mov bl, cl              ; Save original column
    add cl, width           ; Move to right side
    add cl ,1
    Cursor_MoveTo ch, cl    ; Right side position
    Console_WriteChar 0BAh  ; ? (right vertical)
    mov cl, bl              ; Restore original column
    
    dec dl                  ; Decrease height counter
    cmp dl, 0
    ja @MIDDLE
    pop dx
    
    ; Draw bottom row
    inc ch                  ; Move to bottom row
    Cursor_MoveTo ch, cl
    Console_WriteChar 0C8h  ; ? (bottom-left corner)
    
    ; Draw bottom horizontal line
    push dx
    mov dh, width
@BOTTOM:
    Console_WriteChar 0CDh  ; ? (horizontal line)
    dec dh
    cmp dh, 0
    ja @BOTTOM
    pop dx
    
    Console_WriteChar 0BCh  ; ? (bottom-right corner)
    Console_WriteBlankLine
    
    pop dx
    pop cx
    pop bx
    pop ax
endm



Draw_Menu macro rowIndex, columnIndex
    local @END
    push ax
    push bx
    push cx
    push dx
    
    ; Draw the rectangle (width=25, height=9)
    Draw_Rectangle rowIndex, columnIndex, 25, 9
    
    ; Print the text inside the rectangle
    ; Start at rowIndex + 1, columnIndex + 4 for padding
    mov ch, rowIndex
    mov cl, columnIndex
    inc ch          ; Move 1 row down for top padding
    add cl, 4       ; Add 4 spaces of padding from left border
    
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
    
    ; Move cursor inside the box for user input
    ; Last text line is at rowIndex + 6
    ; Add 1 for the blank row -> rowIndex + 7
    inc ch          ; Blank row
    inc ch          ; Input row
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 input_prompt
    
    ; Read user input into input_buffer (max 5 characters)
    ; We'll limit to 5 characters since the input is likely a menu option (e.g., "1", "2", etc.)
    Console_ReadTextWith0 input_buffer, 5
    
@END:
    pop dx
    pop cx
    pop bx
    pop ax
endm

; Checks if a null-terminated string is a palindrome
; @param stringWith0 | (bytes[]) | The null-terminated string to check
; @returns al | 1 if palindrome, 0 if not palindrome
; @Destructive | Modifies si, di, cx
Is_Palindrome macro stringWith0
    local find_length, compare_loop, is_palindrome, not_palindrome, end_check
    
    push bx
    push cx
    push si
    push di
    
    ; Step 1: Find the length of the string (excluding the null terminator)
    mov si, 0
    mov cx, 0  ; cx will hold the length
find_length:
    cmp stringWith0[si], 0  ; Check for null terminator
    je end_length
    inc si
    inc cx
    jmp find_length
end_length:
    ; cx now holds the length of the string
    ; If length is 0 or 1, it's a palindrome
    cmp cx, 1
    jbe is_palindrome  ; If length <= 1, it's a palindrome
    
    ; Step 2: Set up pointers for comparison
    mov si, 0  ; Start pointer (beginning of string)
    mov di, cx ; End pointer (last character before null terminator)
    dec di     ; Adjust di to point to the last character
    
    ; Step 3: Compare characters from start and end
compare_loop:
    cmp si, di  ; If pointers meet or cross, we're done
    jge is_palindrome  ; If si >= di, all characters matched
    
    mov al, stringWith0[si]  ; Get character from start
    mov bl, stringWith0[di]  ; Get character from end
    cmp al, bl               ; Compare characters
    jne not_palindrome       ; If they don't match, not a palindrome
    
    inc si  ; Move start pointer forward
    dec di  ; Move end pointer backward
    jmp compare_loop

is_palindrome:
    mov al, 1  ; Set return value to 1 (true)
    jmp end_check

not_palindrome:
    mov al, 0  ; Set return value to 0 (false)

end_check:
    pop di
    pop si
    pop cx
    pop bx
endm

Draw_PalindromeCheck macro rowIndex, columnIndex
    local @END, @IS_PALINDROME, @NOT_PALINDROME
    push ax
    push bx
    push cx
    push dx
    
    ; Draw the rectangle (width=23, height=6)
    Draw_Rectangle rowIndex, columnIndex, 23, 6
    
    ; Print the text inside the rectangle
    ; Start at rowIndex + 1, columnIndex + 4 for padding
    mov ch, rowIndex
    mov cl, columnIndex
    inc ch          ; Move 1 row down for top padding
    add cl, 4       ; Add 4 spaces of padding from left border
    
    ; Line 1: "?Es pal?ndromo?"
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 palindrome_line1
    
    ; Line 2: "Cadena: " followed by user input
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 palindrome_line2
    ; Move cursor to after "Cadena: " to take input
    ; "Cadena: " is 8 characters long, so move 8 positions to the right
    add cl, 8
    Cursor_MoveTo ch, cl
    ; Read user input into palindrome_input (max 15 characters)
    Console_ReadTextWith0 palindrome_input, 15
    
    ; Line 3: Blank row (just increment ch)
    inc ch
    
    ; Line 4: Check if the input is a palindrome and display result
    inc ch
    mov cl, columnIndex  ; Reset column to left padding
    add cl, 4
    Cursor_MoveTo ch, cl
    
    ; Check if the input is a palindrome
    Is_Palindrome palindrome_input
    cmp al, 1
    je @IS_PALINDROME
    
@NOT_PALINDROME:
    Console_WriteTextWith0 palindrome_not
    jmp @END

@IS_PALINDROME:
    Console_WriteTextWith0 palindrome_yes
    
@END:
    Console_ReadTextWith0 palindrome_input, 1
    pop dx
    pop cx
    pop bx
    pop ax
endm

; Counts the number of vowels in a null-terminated string
; @param stringWith0 | (bytes[]) | The null-terminated string to check
; @returns al | Number of vowels in the string
; @Destructive | Modifies si, cx
Count_Vowels macro stringWith0
    local check_loop, not_vowel, end_count
    
    push si
    push cx
    
    ; Initialize counters
    mov si, 0  ; Index into the string
    mov cx, 0  ; Vowel counter
    
check_loop:
    mov al, stringWith0[si]  ; Get the current character
    cmp al, 0  ; Check for null terminator
    je end_count  ; If null, we're done
    
    ; Check for vowels (both lowercase and accented)
    ; Code page 437 values:
    ; a = 0x61, ? = 0xA0
    ; e = 0x65, ? = 0x82
    ; i = 0x69, ? = 0xA1
    ; o = 0x6F, ? = 0xA2
    ; u = 0x75, ? = 0xA3
    cmp al, 'a'
    je is_vowel
    cmp al, 0A0h  ; ?
    je is_vowel
    cmp al, 'e'
    je is_vowel
    cmp al, 0x82  ; ?
    je is_vowel
    cmp al, 'i'
    je is_vowel
    cmp al, 0A1h  ; ?
    je is_vowel
    cmp al, 'o'
    je is_vowel
    cmp al, 0A2h  ; ?
    je is_vowel
    cmp al, 'u'
    je is_vowel
    cmp al, 0A3h  ; ?
    je is_vowel
    
    jmp not_vowel  ; Not a vowel, skip to next character

is_vowel:
    inc cx  ; Increment vowel counter

not_vowel:
    inc si  ; Move to next character
    jmp check_loop

end_count:
    mov al, cl  ; Move the count to al (return value)
    pop cx
    pop si
endm

Draw_VowelCount macro rowIndex, columnIndex
    local @END
    push ax
    push bx
    push cx
    push dx
    
    ; Draw the rectangle (width=24, height=6)
    Draw_Rectangle rowIndex, columnIndex, 24, 6
    
    ; Print the text inside the rectangle
    ; Start at rowIndex + 1, columnIndex + 4 for padding
    mov ch, rowIndex
    mov cl, columnIndex
    inc ch          ; Move 1 row down for top padding
    add cl, 4       ; Add 4 spaces of padding from left border
    
    ; Line 1: "N?mero de vocales"
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 vowel_line1
    
    ; Line 2: "Cadena: " followed by user input
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 vowel_line2
    ; Move cursor to after "Cadena: " to take input
    ; "Cadena: " is 8 characters long, so move 8 positions to the right
    add cl, 8
    Cursor_MoveTo ch, cl
    ; Read user input into vowel_input (max 15 characters)
    Console_ReadTextWith0 vowel_input, 15
    
    ; Line 3: Blank row (just increment ch)
    inc ch
    
    ; Line 4: Count vowels and display result
    inc ch
    mov cl, columnIndex  ; Reset column to left padding
    add cl, 4
    Cursor_MoveTo ch, cl
    
    ; Count the vowels in the input
    Count_Vowels vowel_input
    ; al now contains the number of vowels
    ; Convert the number to a string and display it
    mov bl, al  ; Save the count
    add bl, '0' ; Convert to ASCII digit (e.g., 4 -> '4')
    Console_WriteChar bl
    Console_WriteTextWith0 vowel_line3
    Console_ReadTextWith0 vowel_input, 1
    
@END:
    pop dx
    pop cx
    pop bx
    pop ax
endm

; Counts the number of vowels in a null-terminated string
; @param stringWith0 | (bytes[]) | The null-terminated string to check
; @returns al | Number of vowels in the string
; @Destructive | Modifies si, cx
Count_Vowels macro stringWith0
    local check_loop, not_vowel, end_count
    
    push si
    push cx
    
    ; Initialize counters
    mov si, 0  ; Index into the string
    mov cx, 0  ; Vowel counter
    
check_loop:
    mov al, stringWith0[si]  ; Get the current character
    cmp al, 0  ; Check for null terminator
    je end_count  ; If null, we're done
    
    ; Check for vowels (both lowercase and accented)
    ; Code page 437 values:
    ; a = 0x61, ? = 0xA0
    ; e = 0x65, ? = 0x82
    ; i = 0x69, ? = 0xA1
    ; o = 0x6F, ? = 0xA2
    ; u = 0x75, ? = 0xA3
    cmp al, 'a'
    je is_vowel
    cmp al, 0A0h  ; ?
    je is_vowel
    cmp al, 'e'
    je is_vowel
    cmp al, 082h  ; ?
    je is_vowel
    cmp al, 'i'
    je is_vowel
    cmp al, 0A1h  ; ?
    je is_vowel
    cmp al, 'o'
    je is_vowel
    cmp al, 0A2h  ; ?
    je is_vowel
    cmp al, 'u'
    je is_vowel
    cmp al, 0A3h  ; ?
    je is_vowel
    
    jmp not_vowel  ; Not a vowel, skip to next character

is_vowel:
    inc cx  ; Increment vowel counter

not_vowel:
    inc si  ; Move to next character
    jmp check_loop

end_count:
    mov al, cl  ; Move the count to al (return value)
    pop cx
    pop si
endm

; Macro to extract a substring from a given string
; Parameters:
;   source - The source string (null-terminated)
;   startIndex - The starting index (0-based)
;   endIndex - The ending index (0-based, exclusive)
;   destination - The buffer to store the substring
Extract_Substring macro source, startIndex, endIndex, destination
    local @COPY_LOOP, @END_COPY, @TERMINATE
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
    je @COPY_LOOP            ; If startIndex is 0, start copying
    
    ; Skip characters until startIndex
    xor cx, cx               ; Counter for current position
@SKIP_LOOP:
    mov al, [si]
    cmp al, 0                ; Check for null terminator
    je @TERMINATE            ; If end of string, terminate
    
    inc si                   ; Move to next character
    inc cx                   ; Increment position counter
    
    cmp cl, bl               ; Compare with startIndex
    jb @SKIP_LOOP            ; If not reached, continue skipping
    
@COPY_LOOP:
    ; Check if we've reached endIndex or end of string
    mov al, [si]
    cmp al, 0                ; Check for null terminator
    je @TERMINATE            ; If end of string, terminate
    
    cmp cl, endIndex         ; Compare with endIndex
    jae @TERMINATE           ; If reached or exceeded, terminate
    
    ; Copy the character
    mov al, [si]
    mov [di], al
    
    ; Increment pointers and counters
    inc si
    inc di
    inc cx
    
    jmp @COPY_LOOP
    
@TERMINATE:
    ; Null-terminate the destination string
    mov byte ptr [di], 0
    
@END_COPY:
    pop di
    pop si
    pop cx
    pop bx
    pop ax
endm

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
    
    ; Print the text inside the rectangle
    ; Start at rowIndex + 1, columnIndex + 4 for padding
    mov ch, rowIndex
    mov cl, columnIndex
    inc ch          ; Move 1 row down for top padding
    add cl, 4       ; Add 4 spaces of padding from left border
    
    ; Line 1: "Obtener subcadena"
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_line1
    
    ; Line 2: "Cadena: " followed by user input
    inc ch
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_line2
    ; Move cursor to after "Cadena: " to take input
    add cl, 8
    Cursor_MoveTo ch, cl
    ; Read user input for the string (max 20 characters)
    Console_ReadTextWith0 substring_input, 20
    
    ; Line 3: "Inicio: " followed by user input
    inc ch
    mov cl, columnIndex  ; Reset column
    add cl, 4
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_line3
    ; Move cursor to after "Inicio: " to take input
    add cl, 8
    Cursor_MoveTo ch, cl
    ; Read user input for start index (max 2 characters, e.g., "6")
    Console_ReadTextWith0 start_input, 2
    
    ; Line 4: "T?rmino: " followed by user input
    inc ch
    mov cl, columnIndex  ; Reset column
    add cl, 4
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_line4
    ; Move cursor to after "T?rmino: " to take input
    add cl, 9
    Cursor_MoveTo ch, cl
    ; Read user input for end index (max 2 characters, e.g., "8")
    Console_ReadTextWith0 end_input, 2
    
    ; Line 5: Blank row (just increment ch)
    inc ch
    
    ; Line 6: Display the substring (two lines below "T?rmino")
    inc ch
    mov cl, columnIndex  ; Reset column to left padding
    add cl, 4
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_line5
    
    ; Convert start and end indices from ASCII to numbers
    ; Start index
    mov al, start_input[0]  ; Get the first character
    sub al, '0'             ; Convert ASCII digit to number (e.g., '6' -> 6)
    mov bl, al              ; Save start index in bl
    
    ; End index
    mov al, end_input[0]    ; Get the first character
    sub al, '0'             ; Convert ASCII digit to number (e.g., '8' -> 8)
    mov bh, al              ; Save end index in bh
    
    ; Extract the substring using the new macro
    Extract_Substring substring_input, bl, bh, substring_result
    
    ; Display the result after "Subcadena = "
    add cl, 12              ; Move cursor past "Subcadena = "
    Cursor_MoveTo ch, cl
    Console_WriteTextWith0 substring_result
    
    Console_ReadTextWith0 end_input, 1
@END:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
endm

; Clears a rectangle area on the screen by filling it with spaces
Clear_Rectangle macro rowIndex, columnIndex, width, height
LOCAL @CLEAR_ROW, @CLEAR_COL
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
    Console_WriteChar al
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

; ======================= String Utils ==============================|
; String_Len macro - Calculates the length of a null-terminated string
; @param stringWith0 - (bytes[]) | The null-terminated string to measure
; @param length - (word) | Variable to store the length (output, 2 bytes)
; @returns in length:
;       - Number of bytes in the string (excluding the null terminator)
; @NotDestructive - Preserves all registers
String_Len macro stringWith0, length
    local @COUNT_LOOP, @END_COUNT
    
    pusha                    ; Preserve all registers
    
    mov si, 0                ; Index into the string
    mov cx, 0                ; Length counter
    
@COUNT_LOOP:
    mov al, stringWith0[si]  ; Load current character
    cmp al, 0                ; Check for null terminator
    je @END_COUNT            ; If null, exit loop
    inc cx                   ; Increment length counter
    inc si                   ; Move to next character
    jmp @COUNT_LOOP          ; Continue counting
    
@END_COUNT:
    mov length, cx           ; Store the length in the output variable
    
    popa                     ; Restore all registers
endm


;==================== UNIDAD 3 ========================

; File_Create macro - Creates a new file or truncates existing file
; @param fileName - Pointer to null-terminated filename string
; @param handlerOrZero - Word variable to store file handle (output) (define word or two bytes variable)
; @returns in handlerOrZero:
;          - File handle if successful
;          - 0 if error occurred
; @returns in AH if error:
;          - 02h = file not found
;          - 03h = path not found
;          - 04h = no handles available
;          - 05h = access denied
File_Create macro fileName, handlerOrZero
    local @ERROR, @FIN, @SUCCESS
    
    push bx          ; Preserve registers
    push cx
    push dx
    
    mov ah, 3Ch      ; DOS function: Create file
    mov cx, 0        ; File attributes (0 = normal)
    lea dx, fileName ; Load effective address of filename
    int 21h          ; DOS interrupt
    
    jc @ERROR        ; Jump if carry flag set (error occurred)
    
    @SUCCESS:
    mov handlerOrZero, ax  ; Store file handle
    clc             ; Clear carry flag to indicate success
    jmp @FIN
    
    @ERROR:
    mov handlerOrZero, 0   ; Set handle to 0 on error
    ; AH already contains error code from DOS
    
    @FIN:
    pop dx
    pop cx
    pop bx          ; Restore registers
endm


; File_Open macro - Opens an existing file
; @param fileName - Pointer to null-terminated filename string (byte array)
; @param handlerOrZero - Word variable to store file handle (2 bytes output)
; @param openMode - Access mode (1 byte):
;       0 = read only
;       1 = write only
;       2 = read & write
; @returns in handlerOrZero:
;       - File handle if successful
;       - 0 if error occurred
; @returns in AH if error:
;       - 02h = file not found
;       - 03h = path not found
;       - 04h = no handles available
;       - 05h = access denied
File_Open macro fileName, handlerOrZero, openMode
    local @ERROR, @SUCCESS, @FIN
    
    push bx          ; Preserve registers
    push cx
    push dx
    
    mov ah, 3Dh      ; DOS function: Open file
    mov al, openMode ; Set access mode (0, 1, or 2)
    lea dx, fileName ; Load effective address of filename
    int 21h          ; DOS interrupt
    
    jc @ERROR        ; Jump if carry flag set (error occurred)
    
    @SUCCESS:
    mov handlerOrZero, ax  ; Store file handle
    clc             ; Clear carry flag to indicate success
    jmp @FIN
    
    @ERROR:
    mov handlerOrZero, 0   ; Set handle to 0 on error
    ; AH already contains error code from DOS
    
    @FIN:
    pop dx
    pop cx
    pop bx          ; Restore registers
endm

; File_Close macro - Closes an open file handle
; @param handlerFileToClose - Word variable containing file handle (2 bytes)
; @returns carry flag:
;       - CF = 0 if file closed successfully
;       - CF = 1 if error occurred
; @returns in AH if error:
;       - 06h = invalid handle
; @preserves: All registers except those modified by int 21h (AX, BX)
File_Close macro handlerFileToClose
    push bx          ; Preserve BX (used for file handle)
    
    mov ah, 3Eh      ; DOS function: Close file
    mov bx, handlerFileToClose  ; Load file handle
    int 21h          ; DOS interrupt
    ; CF is set by int 21h: 0 = success, 1 = error (AH = error code)
    
    pop bx           ; Restore BX
endm

; File_Write macro - Writes data to an open file
; @param handler - Word (2 bytes) file handle from File_Open or File_Create
; @param bytesToWrite - Word (2 bytes) number of bytes to write
; @param dataPtr - Pointer to byte array containing data to write
; @param result - Word (2 bytes) variable to store bytes written or error code (output)
; @preserves: BX, CX, DX (original values restored after execution)
; @returns in result:
;   - Number of bytes written if successful (matches bytesToWrite)
;   - Error code or partial bytes written if failed
; @returns carry flag:
;   - Cleared on success
;   - Set on error (AH contains 05h = access denied, 06h = invalid handle)
File_Write macro handler, bytesToWrite, dataPtr, result
    local @ERROR, @SUCCESS, @END
    
    push bx
    push cx
    push dx
    
    mov ah, 40h                 ; DOS function: Write to file/device
    mov bx, handler             ; File handle
    mov cx, bytesToWrite        ; Bytes to write
    lea dx, dataPtr             ; Address of data (use lea for pointer)
    int 21h                     ; Execute write
    
    jc @ERROR                   ; Jump if error (carry flag set)
    cmp ax, cx                  ; Verify all bytes written
    jne @ERROR                  ; Partial write is an error
    
@SUCCESS:
    mov result, ax              ; Store bytes written in result
    clc                         ; Clear carry flag for success
    jmp @END
    
@ERROR:
    mov result, ax              ; Store partial bytes or error code
    ; Carry flag already set by INT 21h or cmp/jne
    
@END:
    pop dx
    pop cx
    pop bx                      ; Restore registers
endm


; Macro: File_Read_Corrected
; Reads a specified number of bytes from an open file into a buffer using DOS INT 21h, 3Fh.
; CORRECTED VERSION - Does not overwrite AX return value.
;
; @param fileHandle        WORD    The handle of the file to read from (obtained from File_Open/File_Create).
; @param bytesToRead       WORD    The maximum number of bytes to attempt reading.
; @param buffer            PTR BYTE The memory buffer where the read data will be stored. (array of bytes)
;                                  The buffer must be large enough to hold 'bytesToRead' bytes.
;
; @return AX               WORD    On success (CF=0): Contains the actual number of bytes read.
;                                  This might be less than 'bytesToRead' if EOF was reached before
;                                  reading the requested number of bytes.
;                                  On failure (CF=1): Contains the DOS error code.
; @modifies AX, Flags           ; Modifies AX with result/error and updates Flags (esp. Carry Flag).
; @preserves BX, CX, DX         ; Preserves registers used as input/clobbered by int 21h.
;
File_Read macro fileHandle, bytesToRead, buffer
    push bx                 ; Preserve registers used for int 21h input
    push cx
    push dx

    mov ah, 03Fh            ; DOS function: Read from file/device
    mov bx, fileHandle      ; File handle in BX (from parameter)
    mov cx, bytesToRead     ; Max bytes to read in CX (from parameter)
    lea dx, buffer          ; Address of buffer in DS:DX (from parameter)
    int 21h                 ; Call DOS service
                            ; AX now holds bytes read or error code, CF is set on error

    pop dx                  ; Restore registers in reverse order
    pop cx
    pop bx
    ; AX and Flags remain untouched and hold the actual result from int 21h
endm

; Corrected File_MovePointerInside macro
; Moves the file pointer to a specified position based on typeOfMove
; @param typeOfMove - 1 byte 
;   00h - Start at the beginning of the file
;   01h - Start from current position
;   02h - Start at the end of the file
; @param fileHandle - 2 bytes (word) Handler of the opened file
; @param movePointerDistance_HIGH - 2 bytes (high word of 32-bit offset)
; @param movePointerDistance_LOW - 2 bytes (low word of 32-bit offset)
; @returns CF - 0 if no errors, 1 if error
; @returns DX - High word of new pointer position
; @returns AX - Low word of new pointer position
; @note If CF=1, AX contains the error code (e.g., 06h = invalid handle)
; @destructive ax, bx, cx, dx
File_MovePointerInside macro typeOfMove, fileHandle, movePointerDistance_HIGH, movePointerDistance_LOW
    mov ah, 42h                    ; DOS function: Move file pointer
    mov al, typeOfMove             ; Use the provided typeOfMove (00h, 01h, or 02h)
    mov bx, fileHandle             ; File handle
    mov cx, movePointerDistance_HIGH ; High word of offset
    mov dx, movePointerDistance_LOW  ; Low word of offset
    int 21h                        ; DOS interrupt
endm


; File_Append macro - Appends data to the end of an open file
; @param fileHandle - Word (2 bytes) file handle from File_Open or File_Create
; @param bytesToWrite - Word (2 bytes) number of bytes to write
; @param dataPtr - Pointer to byte array containing data to append
; @param result - Word (2 bytes) variable to store bytes written or error code (output)
; @returns in result:
;   - Number of bytes written if successful (matches bytesToWrite)
;   - 0 if move to end failed, or partial bytes written if write failed
; @returns carry flag:
;   - Cleared on success
;   - Set on error (check AH for code: 05h = access denied, 06h = invalid handle)
; @preserves: BX, CX, DX (original values restored after execution)
File_Append macro fileHandle, bytesToWrite, dataPtr, result
    local @MOVE_ERROR, @WRITE_ERROR, @SUCCESS, @END
    
    push bx
    push cx
    push dx
    
    ; Step 1: Move pointer to end of file using File_MovePointerInside
    File_MovePointerInside 02h, fileHandle, 0, 0  ; 02h = end of file, offset = 0
    jc @MOVE_ERROR                                ; Jump if error moving pointer
    
    ; Step 2: Write data at the end
    mov ah, 40h                 ; DOS function: Write to file/device
    mov bx, fileHandle          ; File handle
    mov cx, bytesToWrite        ; Bytes to write
    lea dx, dataPtr             ; Address of data
    int 21h                     ; Execute write
    jc @WRITE_ERROR             ; Jump if error writing
    cmp ax, cx                  ; Verify all bytes written
    jne @WRITE_ERROR            ; Partial write is an error
    
@SUCCESS:
    mov result, ax              ; Store bytes written in result
    clc                         ; Clear carry flag for success
    jmp @END
    
@MOVE_ERROR:
    mov result, 0               ; Set result to 0 on move error
    jmp @END                    ; Carry flag already set by int 21h
    
@WRITE_ERROR:
    mov result, ax              ; Store partial bytes or 0 on write error
    ; Carry flag already set by int 21h or cmp/jne
    
@END:
    pop dx
    pop cx
    pop bx                      ; Restore registers
endm

; File_GetSize macro - Returns the size of an open file in bytes
; @param fileHandle - Word (2 bytes) file handle from File_Open or File_Create
; @param sizeLow - Word (2 bytes) variable to store the low word of the file size (output)
; @param sizeHigh - Word (2 bytes) variable to store the high word of the file size (output)
; @returns carry flag:
;       - CF = 0 if successful
;       - CF = 1 if error occurred (e.g., invalid handle)
; @returns in sizeLow:
;       - Low word (bits 0-15) of the file size if successful
; @returns in sizeHigh:
;       - High word (bits 16-31) of the file size if successful
; @note If CF=1, AX contains the error code (e.g., 06h = invalid handle)
; @preserves: BX (restored after execution)
; @destructive: AX, CX, DX (modified by int 21h via File_MovePointerInside)
File_GetSize macro fileHandle, sizeLow, sizeHigh
    push bx          ; Preserve BX
    
    ; Move pointer to end of file to get size
    File_MovePointerInside 02h, fileHandle, 0, 0
    ; 02h = move relative to end, offset 0:0
    ; Returns DX:AX = file size, CF = 0 on success, CF = 1 on error
    
    jc @ERROR        ; Skip storing if error occurred
    mov sizeLow, ax  ; Store low word of size
    mov sizeHigh, dx ; Store high word of size
    jmp @ENDMACRO
@ERROR:
    mov sizeLow, -1
    mov sizeHigh, -1
    
@ENDMACRO:
    ; CF remains set/cleared from File_MovePointerInside
    pop bx           ; Restore BX
endm


; Macro: File_DisplayContents
; Displays file contents char by char using only library macros
; @param fileHandle - File handler (word)
File_DisplayContents macro fileHandle
    local @read_loop, @end_read
    pusha
    
    ; Initialize file pointer to start
    File_MovePointerInside 0, fileHandle, 0, 0  ; Move to start
    
@read_loop:
    ; Use a temporary register to pass the address
    push ax                ; Save AX and create space on stack
    mov si, sp             ; SI points to stack location
    File_Read fileHandle, 1, ss:[si]  ; Read 1 byte to stack
    
    jc @end_read           ; Exit on error
    cmp ax, 0              ; Check if EOF
    je @end_read
    
    ; Display the character
    pop ax                 ; Get character in AL
    Console_WriteChar al   ; Display it
    
    jmp @read_loop
    
@end_read:
    add sp, 2              ; Clean up stack
    popa
endm
