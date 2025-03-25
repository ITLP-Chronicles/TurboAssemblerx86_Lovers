crearRectangulo macro ancho, largo, fila, columna
    local cicloHorizontal, continuar, cicloVertical, fin
    push bx
    push cx
    mov bh, columna
    add bh, ancho
    mov ch, fila
    mov cl, columna
    posicionarCursor ch, cl
    desplegarCaracter 218
    add ch, largo
    posicionarCursor ch, cl
    desplegarCaracter 192
    sub ch, largo
    inc cl
    
    cicloHorizontal:
        cmp bh, cl
        je continuar
        posicionarCursor ch, cl
        desplegarCaracter 196
        add ch, largo
        posicionarCursor ch, cl
        desplegarCaracter 196
        sub ch, largo
        inc cl
        jmp cicloHorizontal
        
    continuar:
        posicionarCursor ch, cl
        desplegarCaracter 191
        add ch, largo
        posicionarCursor ch, cl
        desplegarCaracter 217
        sub ch, largo

        mov bh, fila
        add bh, largo
        mov ch, fila
        inc ch
        mov cl, columna
        
    cicloVertical:
        cmp bh, ch
        je fin
        posicionarCursor ch, cl
        desplegarCaracter 179
        add cl, ancho
        posicionarCursor ch, cl
        desplegarCaracter 179
        sub cl, ancho
        inc ch
        jmp cicloVertical
        
    fin:
        pop cx
        pop bx
endm