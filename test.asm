INCLUDE krsv2.inc

data segment
    ;================= MENU =======================
    menu_line1 db "Men", 0A3h, " de Cadenas", 0
    menu_line2 db "1. ", 0A8h, "Es pal", 0A1h, "ndromo?", 0
    menu_line3 db "2. N", 0A3h, "mero de vocales", 0
    menu_line4 db "3. Obtener subcadena", 0
    menu_line5 db "4. ?", 0
    menu_line6 db "<ESC> Para Salir", 0
    input_prompt db "Selecci", "o" ,"n: ", 0

    ; Define the input buffer (5 characters + 1 for null terminator)
    input_buffer db 6 dup(0)  ; Reserve 6 bytes, initialized to 0
    
    ;================== PALINDROME ===================
    palindrome_line1 db 0A8h, "Es pal", 0A1h, "ndromo?", 0
    palindrome_line2 db "Cadena: ", 0
    palindrome_yes   db "S", 0A1h, " es Pal", 0A1h, "ndromo", 0
    palindrome_not   db "No es Pal", 0A1h, "ndromo", 0
    palindrome_input db 20 dup(0)  ; Reserve 20 bytes, initialized to 0
    
    ;================== Vowel Counter ===================
    vowel_line1 db "N", 0A3h, "mero de vocales", 0
    vowel_line2 db "Cadena: ", 0
    vowel_line3 db " vocales", 0
    vowel_input db 16 dup(0)  ; Reserve 16 bytes, initialized to 0
    
    ;================= Substring ========================
    substring_line1 db "Obtener subcadena", 0
    substring_line2 db "Cadena: ", 0
    substring_line3 db "Inicio: ", 0
    substring_line4 db "T", 0A2h, "rmino: ", 0  ; T?rmino (? = 0A2h in code page 437)
    substring_line5 db "Subcadena = ", 0
    substring_input db 21 dup(0)  ; Reserve 21 bytes for the input string (20 chars + null)
    start_input db 3 dup(0)       ; Reserve 3 bytes for start index (2 chars + null)
    end_input db 3 dup(0)         ; Reserve 3 bytes for end index (2 chars + null)
    substring_result db 16 dup(0) ; Reserve 16 bytes for the substring

    ; Message for unavailable option
    not_available_msg db "Opci", 0A2h, "n no disponible", 0
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
        Draw_Menu 0 0 20 10  ; Draw the menu on the left side (columns 0 to 19)
        Cursor_MoveTo 10, 1  ; Position cursor for input prompt
        
        ; Clear the input area to remove the selection
        Clear_Rectangle 10, 1, 19, 1  ; Clear the input prompt and input
        
        ; Check the input
        mov al, input_buffer[0]
        
        ; Check for ESC (ASCII 27)
        cmp al, 27
        je @EXIT
        
        ; Clear only the right side of the screen (columns 26 to 79)
        Clear_Rectangle 0, 26, 54, 23  ; Width is 54 (80-26) to cover right side
        
        ; Check for option '1' (Palindrome Check)
        cmp al, '1'
        jne @CHECK_OPTION_2
        Draw_PalindromeCheck 0, 26     ; Draw on the right side
        jmp @MAIN
        
@CHECK_OPTION_2:
        ; Check for option '2' (Vowel Count)
        cmp al, '2'
        jne @CHECK_OPTION_3
        Draw_VowelCount 0, 26
        jmp @MAIN
        
@CHECK_OPTION_3:
        ; Check for option '3' (Substring Extraction)
        cmp al, '3'
        jne @CHECK_OPTION_4
        Draw_SubstringInput 0, 26
        jmp @MAIN
        
@CHECK_OPTION_4:
        ; Check for option '4' (Not available)
        cmp al, '4'
        jne @MAIN  ; If not '4', loop back to menu
        Cursor_MoveTo 12, 1  ; Position for message
        Console_WriteTextWith0 not_available_msg
        jmp @MAIN
        
@EXIT:
        ;=== End Code ====
        App_Exit

;====================================== PROCEDURES =====================================

code ends
end start