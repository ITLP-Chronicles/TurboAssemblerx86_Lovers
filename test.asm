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

    ; Define the input buffer (5 characters + 1 for null terminator)
    input_buffer db 6 dup(0)  ; Reserve 6 bytes, initialized to 0
    
    
    ;================== PALINDROME ===================
    palindrome_line1 db 0A8h, "Es pal", 0A1h, "ndromo?", 0
    palindrome_line2 db "Cadena: ", 0
    palindrome_yes   db "S", 0A1h, " es Pal", 0A1h, "ndromo", 0
    palindrome_not   db "No es Pal", 0A1h, "ndromo", 0

    ; Define the input buffer (15 characters + 1 for null terminator)
    palindrome_input db 20 dup(0)  ; Reserve 16 bytes, initialized to 0
    
    
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
        ;Draw_Menu 0 0 20 10
        ;Console_WriteBlankLine
        
        Draw_PalindromeCheck 10 10
        Console_WriteBlankLine
        ;mov al, input_buffer[0]
        ;cmp al, 27

        ;jne @MAIN
        
        ;=== End Code ====
        App_Exit
code ends
end start