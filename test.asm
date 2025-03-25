INCLUDE krsv2.inc
data segment
 menu_line1 db "Men", 0A3h, " de Cadenas", 0
menu_line2 db "1. ", 0A8h, "Es pal", 0A1h, "ndromo?", 0
menu_line3 db "2. N", 0A3h, "mero de vocales", 0
menu_line4 db "3. Obtener subcadena", 0
menu_line5 db "4. ?", 0
menu_line6 db "<ESC> Para Salir", 0
data ends

code segment
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        ;=== Start Code ===
        Theme_SetRed
        Draw_Menu 0 0 20 10
        Console_WriteBlankLine
        Console_WriteBlankLine
        Console_WriteBlankLine
        ;=== End Code ====
        App_Exit
code ends
end start