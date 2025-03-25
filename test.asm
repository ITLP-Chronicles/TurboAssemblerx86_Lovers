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

    ; Define the input buffer (15 characters + 1 for null terminator)
    palindrome_input db 20 dup(0)  ; Reserve 16 bytes, initialized to 0
    
    
    ;================== Vowel Counter ===================
    vowel_line1 db "N", 0A3h, "mero de vocales", 0
    vowel_line2 db "Cadena: ", 0
    vowel_line3 db " vocales", 0

    ; Define the input buffer (15 characters + 1 for null terminator)
    vowel_input db 16 dup(0)  ; Reserve 16 bytes, initialized to 0
    
    
    ;================= substring ========================
    ; Define the strings (null-terminated) with special characters
    substring_line1 db "Obtener subcadena", 0
    substring_line2 db "Cadena: ", 0
    substring_line3 db "Inicio: ", 0
    substring_line4 db "T", 0A2h, "rmino: ", 0  ; T?rmino (? = 0A2h in code page 437)
    substring_line5 db "Subcadena = ", 0

    ; Define the input buffers
    substring_input db 21 dup(0)  ; Reserve 21 bytes for the input string (20 chars + null)
    start_input db 3 dup(0)       ; Reserve 3 bytes for start index (2 chars + null)
    end_input db 3 dup(0)         ; Reserve 3 bytes for end index (2 chars + null)

    ; Define the result buffer (max 15 characters + 1 for null terminator)
    substring_result db 16 dup(0)  ; Reserve 16 bytes for the substring
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
        Draw_Menu 0 0 20 10
        mov al, input_buffer[0]
        cmp al, '1'
        ;je
        ;Console_WriteBlankLine
        
        ;Draw_SubstringInput 10 10 
        ;Console_WriteBlankLine
       
        
        ;=== End Code ====
        App_Exit

;====================================== PROCEDURES =====================================

code ends
end start