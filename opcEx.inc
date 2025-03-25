GUESS_NUMBER MACRO
    LOCAL game_loop, win, too_high, too_low

    ; Generar n?mero aleatorio (suponiendo una funci?n definida)
    CALL generate_random_number  ; Devuelve el n?mero en EAX
    MOV secret_number, EAX       ; Guardar el n?mero secreto

    MOV attempts, 0              ; Inicializar contador de intentos

game_loop:
    ; Mostrar mensaje para adivinar
    PRINT_STRING guess_prompt

    ; Leer entrada del usuario
    CALL read_integer            ; Devuelve la adivinanza en EAX
    MOV guess, EAX               ; Guardar la adivinanza

    ; Incrementar contador de intentos
    INC attempts

    ; Comparar adivinanza con el n?mero secreto
    MOV EAX, guess
    CMP EAX, secret_number
    JE win                       ; Si igual, gana
    JG too_high                  ; Si mayor, demasiado alto
    JL too_low                   ; Si menor, demasiado bajo

too_high:
    PRINT_STRING too_high_msg    ; Mensaje "demasiado alto"
    JMP game_loop                ; Volver al bucle

too_low:
    PRINT_STRING too_low_msg     ; Mensaje "demasiado bajo"
    JMP game_loop                ; Volver al bucle

win:
    PRINT_STRING win_msg         ; Mensaje de victoria
    PRINT_STRING attempts_msg    ; Mostrar intentos
    MOV EAX, attempts
    CALL print_integer           ; Imprimir n?mero de intentos
ENDM