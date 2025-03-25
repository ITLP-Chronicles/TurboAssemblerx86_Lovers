INCLUDE krsv2.inc
    
data segment   

; =========== Window Configuration ===========
    _arr1b_title db 'Selecciona una opcion:', '$' 
    
    ;By limitations of the mainmacro, you can shoose until 9 options until the numbering goes weird
    _arr1b_options db 'Es palindromo?', 0, 'Numeros de vocales', 0, 'Obtener subcadena', 0, '?',0, 'otra opcion',  1
    _arr2b_optionsarray dw 10 dup(0) ;The length of this variable has to be above than _arr1b_options 'length'. You shouldn't worry about this, it works by itself
    
    _1b_xPosition db 1
    _1b_yPosition db 1
    _1b_rectangleWidth db 27
    _1b_rectangleHeigth db 18
    _1b_left_padding db 2 
    
    _1b_withNumberedRows db 1
    _1b_withESCMessage db 1

; ============ Normal Variables ==============

    _arr1b_rawInput db 20 dup(0)
    _1b_selectedOption db 0
    _1b_maxCharsAccepted dw 17
    
; ============ Message UI Variables ==============
    _arr1b_initialMsg db 'Selecciona la opcion que quieras utilizar: ', 0
    _arr1b_invalidMsg db 'No has elegido una opci?n correcta, intenta de nuevo...', 0
    _arr1b_wantToReset db 'Presione enter para volver a la pantalla inicial...', 0
    _arr1b_feedBack db 'Se ha ingresado ', 0
    _arr1b_continue db 'Presiona enter para continuar...', 0
    
    ; palindromo
    resultado db 0
    miPalabra db 'laloll', 0   
    msg_si db 'Si es palindromo$', 1  ; Mensaje si es pal?ndromo
    msg_no db 'No es palindromo$', 1  ; Mensaje si no es pal?ndromo
data ends
 
code segment
    .386  
    start:
        assume cs:code, ds:data
        mov ax, data 
        mov ds, ax
        ;=== Start Code ===
        ;Setup
        String_Split _arr1b_options, _arr2b_optionsarray 
        
    MAINBUCLE:  
        call Main
        cmp _1b_selectedOption, '1'
        je OPTION1
        cmp _1b_selectedOption, '2'
        je OPTION2
        cmp _1b_selectedOption, '3'
        je OPTION3
        cmp _1b_selectedOption, '4'
        je OPTION4
        Console_WriteTextWith0 _arr1b_invalidMsg
        jmp MAINBUCLE
    OPTION1:
        call Option1Proc
        call EndOptionLogic
        jmp MAINBUCLE
    OPTION2:
    OPTION3:
    OPTION4:
    ENDAPP:   
        ;=== End Code ====
        App_Exit
        
    ;============ PROCEDURES ==================
    Main proc
        Theme_SetRed
        Cursor_MoveTo 0 0
        
        ;Stores the original xPosition, for some reason, Raw_RectangleWithText was modifying it by ref, when it shouldn't happen in first place.
        mov ah, 0
        mov al, _1b_xPosition
        push ax
        Draw_RectangleWithText _1b_xPosition _1b_yPosition _1b_rectangleWidth _1b_rectangleHeigth _arr1b_title _arr2b_optionsarray _1b_left_padding _1b_withNumberedRows _1b_withESCMessage
        pop ax
        mov _1b_xPosition, al ;Restoring original
        
        
        Console_WriteTextWith0 _arr1b_initialMsg
        call ReadSelectedInput
        
        ret
    Main endp
    
    ReadSelectedInput proc
        Console_ReadTextWith0 _arr1b_rawInput, _1b_maxCharsAccepted
        
        ;Select the first digit number 
        push ax 
        mov ah, 0
        mov al, _arr1b_rawInput[0] ;Only selecting the first char of all its text... (user experience..?)
        mov _1b_selectedOption, al
        pop ax
        
        cmp _1b_selectedOption, 10
        je IsEnter
        jmp IsNotEnter
    IsNotEnter:
        Console_WriteBlankLine
        Console_WriteTextWith0 _arr1b_feedBack
        Console_WriteChar _1b_selectedOption
        Console_WriteBlankLine
        Console_WriteBlankLine
        Console_WriteTextWith0 _arr1b_continue
        Console_ReadTextWith0 _arr1b_rawInput, _1b_maxCharsAccepted
        
        Theme_SetRed
        Cursor_MoveTo 0 0
        
    IsEnter:
        ret
    ReadSelectedInput endp
    
    EndOptionLogic proc
    
        Console_WriteBlankLine
        Console_WriteBlankLine
        Console_WriteTextWith0 _arr1b_wantToReset
        Console_ReadTextWith0 _arr1b_rawInput, _1b_maxCharsAccepted
        ret
    EndOptionLogic endp
    
    ;============ OPTIONS PROCEDURES ==================
    
    Option1Proc proc
       CheckPalindrome miPalabra, resultado       
                
       cmp resultado, 1
       je es_palindromo  
       jmp no_es_palindromo 

       es_palindromo:
           mov ah, 09h         
           lea dx, msg_si      
           int 21h             
           jmp fin
           
       no_es_palindromo:
           mov ah, 09h         
           lea dx, msg_no      
           int 21h             
           
       fin:
       ret
    endp
    
    Option2Proc proc
    
       ret
    endp
    
    Option3Proc proc
    
       ret
    endp
    
    Option4Proc proc
    
       ret
    endp
code ends
end start