;============ Template ==============
data segment

data ends

code segment
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        ;=== Start Code ===
        
        ;=== End Code ====
        mov ax, 4C00h
        int 21h
code ends
end start
;=========== End Template ================

;============ Template With macros ==============
INCLUDE krsv2.inc
data segment

data ends

code segment
    .386
    start:
        assume cs:code, ds:data
        mov ax, data
        mov ds, ax
        ;=== Start Code ===
        
        ;=== End Code ====
        App_Exit
code ends
end start
;=========== End Template ================



;========= Style ===========
mov ax, 0600h       
mov bh, 1Fh         ; Blue Blackground, Grey Font Color
mov cx, 0000h 
mov dx, 184fh
int 10h             ; BIO Interruption
;========= End Style ==========

;======== Change Position ===========
mov ah, 02h ; Cursor Position parameter
mov bh, 0 ; Default page
mov dh, 10 ; Row
mov dl, 10 ; Column
int 10h

;====== Write String ========
mov ah, 9h
lea dx, VARIABLE ;Name of variable in DS
int 21h 

;====== Write Char on actual cursor =========
mov ah, 02h
mov dl , ASCII8BITESVALUE
int 21h

;====== Input() =========
lea dx, VARIABLE
mov VARIABLE, 28 ;Number of chars accepted (cadena must be +2) (byes)
mov ah, 0Ah
int 21h


;======= Input(1) ========= (With echo)
mov ah, 01h 
int 21h

; ==================== SIMPLE IF TEMPLATE =====================
;Pseudocode
if (var == op)
    codigo_si
else
    codigo_no
; Start COPY
cmp var, op
je eti_si
jmp eti_no

eti_si:
    codigo_si
eti_no:
    codigo_no
fin:
; End COPY


; =================== IF ELSE IF ELSE TEMPLATE ====================
if (var == op)
    codigo_op1
else if (var == op2)
    codigo_op2
else
    codigo_else
    
    
cmp var, op1
je eti_op1

cmp var, op2
je eti_op2

;cmp var, op3  ;Add more if needed
;je eti_op3

jmp eti_else

eti_op1:
    codigo_op1
    jmp fin
eti_op2:
    codigo_op2
    jmp fin
;eti_op3:   ;Add more if needed
    ;codigo_op3
    ;jmp fin
eti_else:
    codigo_else
    jmp fin ;Optional
fin:
    
    
;================= AND TEMPLATE ==================
if (var1 == op1 && var2 == op2)
    codigo_si
else
    codigo_no

cmp var1, op1   ;First and
jne eti_no

cmp var2, op2   ;Second and
jne eti_no

;cmp var3, op3  ;Add more if needed
;jne eti_no

jmp eti_si

eti_si:
    codigo_si
    jmp fin
eti_no:
    codigo_no
    jmp fin ;Optional
fin:
    
    
; ====================  OR TEMPLATE =================

cmp var, op1 ; First op
je eti_si

cmp var, op2  ; Second Op
je eti_si

jmp eti_no

eti_si:
    codigo_si
    jmp fin
eti_no:
    codigo_no
    jmp fin ;Optional
fin:
    
    
    