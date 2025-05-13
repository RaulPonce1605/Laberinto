#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>

#define FILAS 10
#define COLUMNAS 10

// Declaración de la función ensamblador
extern int mover_jugador_asm(char** mapa, int y, int x, char direccion);

// Laberinto más visual
char r0[COLUMNAS + 1] = "+--------+";
char r1[COLUMNAS + 1] = "|P  |    |";  
char r2[COLUMNAS + 1] = "|||-| || |";
char r3[COLUMNAS + 1] = "| | | |  |";
char r4[COLUMNAS + 1] = "| |   || |";
char r5[COLUMNAS + 1] = "| |||    |";
char r6[COLUMNAS + 1] = "|   |||| |";
char r7[COLUMNAS + 1] = "||| |    |";
char r8[COLUMNAS + 1] = "|     ||X|";  // Objetivo 'X'
char r9[COLUMNAS + 1] = "+--------+";

char* mapa[FILAS] = {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9};

// Imprime el laberinto en consola
void mostrar_mapa() {
    system("cls");
    printf("----------------------\n");
    for (int i = 0; i < FILAS; i++) {
        printf("| ");
        for (int j = 0; j < COLUMNAS; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("|\n");
    }
    printf("-----------------------\n");
    printf("Usa las flechas para moverte. ESC para salir.\n");
}

// Muestra una animación sencilla cuando el jugador gana
void mostrar_animacion_ganaste() {
    system("cls");
    printf("|--------------------|\n");
    printf("|    GANASTEEEEEE    |\n");
    printf("|--------------------|\n");
    Sleep(1000);  // Pausa antes de limpiar la pantalla y finalizar
    system("cls");
    Sleep(500);  // Pausa adicional para dar un efecto de animación
}

// Sonido cuando el jugador se mueve
void sonido() {
    Beep(750, 300);  // Emite un sonido de 750Hz durante 300ms
}

void animacion_inicio() {
    system("cls");
    printf("Cargando");
    for (int i = 0; i < 3; i++) {
        Sleep(500);
        printf(".");
    }
    Sleep(800);
    system("cls");
    printf("Preparando el laberinto");
    for (int i = 0; i < 3; i++) {
        Sleep(500);
        printf(".");
    }
    Sleep(800);
    system("cls");
    printf("¡Listo!\n");
    Sleep(1000);
}


int main() {
    animacion_inicio();
          // Llamada a la función de animación de inicio
    int pos_y = 1, pos_x = 1;
    bool en_juego = true;

    while (en_juego) {
        mostrar_mapa();

        int tecla = _getch();  // Captura la tecla

        if (tecla == 27) {  // ESC
            break;
        }

        if (tecla == 0 || tecla == 224) {  // Tecla especial (flecha)
            tecla = _getch();  // Leer el código real

            char direccion;

            switch (tecla) {
                case 72: direccion = 'W'; break;  // Flecha ↑
                case 80: direccion = 'S'; break;  // Flecha ↓
                case 75: direccion = 'A'; break;  // Flecha ←
                case 77: direccion = 'D'; break;  // Flecha →
                default: continue;
            }

            // Verificar si el siguiente movimiento es válido
            int nuevo_y = pos_y, nuevo_x = pos_x;
            switch (direccion) {
                case 'W': nuevo_y--; break;  // Arriba
                case 'S': nuevo_y++; break;  // Abajo
                case 'A': nuevo_x--; break;  // Izquierda
                case 'D': nuevo_x++; break;  // Derecha
            }

            // Comprobar si el siguiente espacio está bloqueado por una pared
            if (mapa[nuevo_y][nuevo_x] == '|') {
                printf("Movimiento inválido. No puedes pasar por las paredes.\n");
                Sleep(500);  // Pausa para que el jugador vea el mensaje
                continue;  // No hacer el movimiento
            }

            // Si no es una pared, mover al jugador
            int estado = mover_jugador_asm(mapa, pos_y, pos_x, direccion);

            switch (estado) {
                case 0:
                    printf("Movimiento inválido.\n");
                    Sleep(500);
                    break;
                case 1:
                    for (int i = 0; i < FILAS; i++) {
                        for (int j = 0; j < COLUMNAS; j++) {
                            if (mapa[i][j] == 'P') {  
                                pos_y = i;
                                pos_x = j;
                            }
                        }
                    }
                    sonido();  // Sonido al mover
                    break;
                case 2:
                    mostrar_mapa();
                    mostrar_animacion_ganaste();  // Muestra la animación de ganar
                    en_juego = false;
                    break;
            }
        }
    }

    return 0;
}
