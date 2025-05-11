section .text
%define FILAS 10
%define COLUMNAS 10
global mover_jugador_asm

; Función: mover_jugador_asm
; Argumentos:
;   RCX = puntero al laberinto (char**)
;   RDX = fila actual
;   R8  = columna actual
;   R9  = dirección del movimiento (char)
mover_jugador_asm:
    push rbp
    mov rbp, rsp

    ; Elegir movimiento según la dirección (WASD)
    cmp r9b, 'W'
    je .ir_arriba
    cmp r9b, 'S'
    je .ir_abajo
    cmp r9b, 'A'
    je .ir_izquierda
    cmp r9b, 'D'
    je .ir_derecha
    jmp .mov_invalido

.ir_arriba:
    dec edx        ; fila - 1
    jmp .validar_limite

.ir_abajo:
    inc edx        ; fila + 1
    jmp .validar_limite

.ir_izquierda:
    dec r8d        ; col - 1
    jmp .validar_limite

.ir_derecha:
    inc r8d        ; col + 1
    jmp .validar_limite

.validar_limite:
    ; Revisar que fila y columna estén dentro del laberinto
    cmp edx, 0
    jl .mov_invalido
    cmp edx, FILAS - 1
    jg .mov_invalido
    cmp r8d, 0
    jl .mov_invalido
    cmp r8d, COLUMNAS - 1
    jg .mov_invalido

    ; Cargar la celda destino
    mov rax, [rcx + rdx*8]    ; Obtener dirección de la fila
    mov bl, [rax + r8]        ; Cargar caracter destino

    ; Evaluar contenido de la casilla
    cmp bl, '#'
    je .mov_invalido
    cmp bl, 'X'
    je .meta_alcanzada

    ; Buscar la posición actual del jugador ('P')
    mov rsi, rcx      ; rsi = inicio del laberinto
    xor rdi, rdi      ; fila contador
.localizar_p:
    mov rax, [rsi + rdi*8]
    xor rbx, rbx      ; columna contador
.buscar_columna:
    cmp byte [rax + rbx], 'P'
    je .jugador_encontrado
    inc rbx
    cmp ebx, COLUMNAS
    jl .buscar_columna
    inc rdi
    cmp edi, FILAS
    jl .localizar_p

.jugador_encontrado:
    ; Borrar la posición anterior y colocar nueva
    mov byte [rax + rbx], '.'         ; Borra 'P' anterior
    mov rax, [rsi + rdx*8]
    mov byte [rax + r8], 'P'          ; Coloca 'P' en nueva posición
    mov eax, 1                        ; Retornar 1 (mov válido)
    jmp .salir

.meta_alcanzada:
    mov eax, 2                        ; Retornar 2 (llegó a 'X')
    jmp .salir

.mov_invalido:
    xor eax, eax                      ; Retornar 0 (movimiento inválido)

.salir:
    pop rbp
    ret
