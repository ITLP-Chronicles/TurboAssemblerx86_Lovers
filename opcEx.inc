FACTORIAL_CALCULATOR MACRO number, buffer
    LOCAL calculate_factorial, multiply_loop, end_macro, zero_factorial, invalid_input

    ; Cargar el n?mero en AX
    MOV AX, number               ; El n?mero se pasa como par?metro

    ; Validar que el n?mero sea no negativo
    CMP AX, 0
    JL invalid_input             ; Si es negativo, mostrar error
    JE zero_factorial            ; Si es 0, factorial es 1

    ; Inicializar variables para el c?lculo
    MOV CX, AX                   ; CX = number (contador para el bucle)
    MOV AX, 1                    ; AX = 1 (acumulador del factorial)

calculate_factorial:
    MUL CX                       ; AX = AX * CX
    LOOP calculate_factorial     ; Decrementa CX y repite hasta que CX = 0

    MOV [buffer], AX             ; Almacenar el resultado en la direcci?n del buffer

    PRINT_STRING result_msg      ; Imprimir mensaje "Resultado: "
    MOV AX, [buffer]             ; Cargar el resultado en AX para imprimirlo
    CALL print_integer           ; Llamar a subrutina para imprimir el n?mero
    JMP end_macro

zero_factorial:
    ; Si el n?mero es 0, factorial es 1
    MOV AX, 1
    MOV [buffer], AX             ; Guardar 1 en el buffer
    PRINT_STRING result_msg      ; Imprimir mensaje "Resultado: "
    CALL print_integer           ; Imprimir el resultado (1)
    JMP end_macro

invalid_input:
    ; Si el n?mero es negativo, mostrar mensaje de error
    PRINT_STRING error_msg       ; Imprimir mensaje "Error: n?mero inv?lido"

end_macro:
ENDM

