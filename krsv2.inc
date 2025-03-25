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

; @param xPosition | 1byte |
; @param yPosition | 1byte |
; @param width | 1byte |
; @param heigth | 1byte |
; @param title | byte[] with 0 terminated |
; @param string_array_pointers | 2byte[] with 0 separated and 1 terminated |
; @param padding | 1byte | CSS Concept
; @param isNumberedMode | 1 byte (boolean) 1 or 0 | 

Draw_RectangleWithText macro xPosition, yPosition, width, heigth, title, string_array_pointers, padding, isNumberedMode
    LOCAL TopCycle, FromTopToBottomCycle, FromTopToBottomCycleInner, FromTopToBottomEmptySpace, BottomCycle, IsTitleSection, IsTitleBlankLineSection, IsNotTitleSection, EndMacro
    pusha
    mov ax, 0
    mov bx, 0
    mov cx, 0
    mov dx, 0
   
    ;============= Drawing the top rectangle ==============
    ;       Variables for this section
    ; AX free to use
    ; BX free to use
    ; DX free to use
    ; CX free to use
    ; stack available
    
    mov cl, width
    push cx
    Cursor_MoveTo xPosition, yPosition
    Console_WriteChar 0C9h ;Top left corner
TopCycle:
    Console_WriteChar 0CDh ;Straight horizonal line
    dec width
    cmp width, 0
jne TopCycle
    Console_WriteChar 0BBh ;Top right corner
    pop cx
    mov width, cl
    
    ;============= Drawing now from (top + 1) to (bottom - 1) rectangle ==============
    mov ah, 0
    mov al, xPosition ; Store xPosition in DX
    inc ax ; Advance one row down
    
    mov bh, 0
    mov bl, yPosition ; Store yPosition in DY
    
    ;Storing array length in dx using xPosition and stack as temporary storage
    push ax
    Get_PointerArray_Length string_array_pointers xPosition
    mov dh, 0
    mov dl, xPosition ; Moving 1byte xPosition into 2byte registry size to push it to stack
    pop ax
    mov xPosition, al ;Everything backToNormal
    

  
    
    ; ===== inner section: Printing options =======
    ;       Variables from this section
    ; AX holds actualrow
    ; BX holds actualcolumn
    ; DX has the length of the string array
    ; CX is used in inner padding cycle...
    ; Only stack to store variables?... fuck
    mov si, 0
 
    mov ch, 0
FromTopToBottomCycle: 
 
    mov cl, padding
    Cursor_MoveTo al, bl
    
    ;================ Printing both sides ==================
    ;First printing right side |
    push bx
    add bl, 1
    add bl, width
    Cursor_MoveTo al, bl
    Console_WriteChar 0BAh ;Vertical straight line
    pop bx
    
    ;Coming back to left side |
    Cursor_MoveTo al, bl
    Console_WriteChar 0BAh ;Vertical straight line
    ;================ Printing both sides ==================
    
    FromTopToBottomCycleInner:  ;Writing padding
        Console_WriteChar ' '
        sub cl, 1
        cmp cl, 0
    ja FromTopToBottomCycleInner
    
     ; ===== inner inner section: Printing title =======
     ;       Variables from this section same as outer section all used but stack, double fuck

    

    
     ;Just to index with SI * 2... holly molly
    ;it's the equivalent to => Console_WriteTextPointerWith0 string_array_pointers[si*2]
    push si                 ; Save original si
    push ax                 ; Save original ax
    push bx                 ; Save bx (we?ll use it temporarily)
    push dx
    mov ax, si              ; ax = si (index)
    mov bx, 2               ; bx = 2 (multiplier)
    mul bx                  ; ax = si * 2 (offset), dx = 0 (no overflow assumed)
    mov bx, ax              ; bx = offset
    mov bx, string_array_pointers[bx]  ; bx = pointer at string_array_pointers[si*2]
    Console_WriteTextPointerWith0 bx   ; Print the string using bx as the pointer
    pop dx                  ; Restore dx
    pop bx                  ; Restore bx
    pop ax                  ; Restore ax
    pop si                  ; Restore si
   
    inc ch
    inc xPosition ;IGNORE, it's being as a generic variable, not as "position"....
    inc si ;Increment indexer of the array of words
    inc ax ;Increment row
    dec dx ;decrement the consumed length array
    cmp dx, 0 ;Check if already ended array of words
jne FromTopToBottomCycle







    ; ===== inner section: Printing options =======
    ;       Variables from this section
    ; AX holds actualrow
    ; BX holds actualcolumn
    ; DX free to use
    ; CX Will be used as constant about how many blank rows there will be in the table
    ; Stack available
    mov dx, 0
    mov cx, 0
    
    mov cx, 5 ;CONSTANT VARIABLE: NUMBER OF BLANK ROWS BELOW OPTIONS! I've selected 5 just because
FromTopToBottomEmptySpace:

    ;================ Printing both sides ==================
    ;First printing right side |
    push bx
    add bl, 1
    add bl, width
    Cursor_MoveTo al, bl
    Console_WriteChar 0BAh ;Vertical straight line
    pop bx
    
    ;Coming back to left side |
    Cursor_MoveTo al, bl
    Console_WriteChar 0BAh ;Vertical straight line
    ;================ Printing both sides ================== 
    inc ax
    Cursor_MoveTo al, bl
    dec cx
    cmp cx, 0
ja FromTopToBottomEmptySpace

    ;============= Drawing now the bottom rectangle ==============
    ;So far, the table has advanced to a new blank line ready to use
    ;       Variables availables this section
    ; AX holds actualrow
    ; BX holds actualcolumn
    ; DX free to use
    ; CX free to use
    ; Stack available
    
    Console_WriteChar 0C8h
    mov ch, 0
    mov cl, width
BottomCycle:
        Console_WriteChar 0CDh
        dec cl
        cmp cl, 0
ja BottomCycle
    Console_WriteChar 0BCh
    
    
    
    
EndMacro:
    popa
endm


limpiarPartePantalla macro color, esqSupIzq, esqInfDer
    push ax
    push bh
    push cx
    push dx
        
    mov ax, 0600h
    mov bh, color
    mov cx, esqSupIzq
    mov dx, esqInfDer
    int 10h
    
    pop dx
    pop cx
    pop bh
    pop ax
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

