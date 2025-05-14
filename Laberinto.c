#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>

// Códigos ANSI para colores
#define COLOR_RESET   "\x1b[0m"
#define COLOR_WALL    "\x1b[90m"  // Gris
#define COLOR_PLAYER  "\x1b[92m"  // Verde
#define COLOR_EXIT    "\x1b[91m"  // Rojo
#define COLOR_TITLE   "\x1b[96m"  // Cian
#define COLOR_BORDER  "\x1b[93m"  // Amarillo

#define FILAS 10
#define COLUMNAS 10

extern int mover_jugador_asm(char** mapa, int y, int x, char direccion);

// Laberinto
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

void mostrar_mapa() {
    system("cls");
    printf(COLOR_BORDER "----------------------\n" COLOR_RESET);
    for (int i = 0; i < FILAS; i++) {
        printf(COLOR_BORDER "| " COLOR_RESET);
        for (int j = 0; j < COLUMNAS; j++) {
            char c = mapa[i][j];
            if (c == '|') printf(COLOR_WALL "%c " COLOR_RESET, c);
            else if (c == 'P') printf(COLOR_PLAYER "%c " COLOR_RESET, c);
            else if (c == 'X') printf(COLOR_EXIT "%c " COLOR_RESET, c);
            else printf("%c ", c);
        }
        printf(COLOR_BORDER "|\n" COLOR_RESET);
    }
    printf(COLOR_BORDER "----------------------\n" COLOR_RESET);
    printf("Usa las flechas para moverte. ESC para salir.\n");
}

void mostrar_animacion_ganaste() {
    system("cls");
    printf(COLOR_TITLE);
    printf("  ██████╗  █████╗ ███╗   ██╗███████╗████████╗███████╗\n");
    printf(" ██╔════╝ ██╔══██╗████╗  ██║██╔════╝╚══██╔══╝██╔════╝\n");
    printf(" ██║  ███╗███████║██╔██╗ ██║███████╗   ██║   █████╗  \n");
    printf(" ██║   ██║██╔══██║██║╚██╗██║╚════██║   ██║   ██╔══╝  \n");
    printf(" ╚██████╔╝██║  ██║██║ ╚████║███████║   ██║   ███████╗\n");
    printf("  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝   ╚═╝   ╚══════╝\n");
    printf(COLOR_RESET);
    Sleep(1000);  // Pausa antes de limpiar la pantalla y finalizar
    system("cls");
    Sleep(500);  // Pausa adicional para dar un efecto de animación
}

void sonido() {
    Beep(750, 300);
}

void ascii_art_line(const char* text) {
    printf(COLOR_TITLE "\n\n     █████████████████████████████████████████████\n" COLOR_RESET);
    printf(COLOR_TITLE "     █ " COLOR_RESET "%-40s" COLOR_TITLE " █\n" COLOR_RESET, text);
    printf(COLOR_TITLE "     █████████████████████████████████████████████\n" COLOR_RESET);
}

void animacion_inicio() {
    system("cls");
    ascii_art_line("CARGANDO...");
    Sleep(1000);
    system("cls");
    ascii_art_line("PREPARANDO JUEGO");
    Sleep(1000);
    system("cls");
    ascii_art_line("¡LISTO!");
    Sleep(1000);
    system("cls");
}

int main() {
    system("chcp 65001"); 

    animacion_inicio();

    int pos_y = 1, pos_x = 1;
    bool en_juego = true;

    while (en_juego) {
        mostrar_mapa();

        int tecla = _getch();

        if (tecla == 27) break;

        if (tecla == 0 || tecla == 224) {
            tecla = _getch();

            char direccion;
            switch (tecla) {
                case 72: direccion = 'W'; break;
                case 80: direccion = 'S'; break;
                case 75: direccion = 'A'; break;
                case 77: direccion = 'D'; break;
                default: continue;
            }

            int nuevo_y = pos_y, nuevo_x = pos_x;
            if (direccion == 'W') nuevo_y--;
            if (direccion == 'S') nuevo_y++;
            if (direccion == 'A') nuevo_x--;
            if (direccion == 'D') nuevo_x++;

            if (mapa[nuevo_y][nuevo_x] == '|') {
                printf(COLOR_EXIT "Movimiento inválido. Hay una pared.\n" COLOR_RESET);
                Sleep(500);
                continue;
            }

            int estado = mover_jugador_asm(mapa, pos_y, pos_x, direccion);

            switch (estado) {
                case 0:
                    printf(COLOR_EXIT "Movimiento inválido.\n" COLOR_RESET);
                    Sleep(500);
                    break;
                case 1:
                    for (int i = 0; i < FILAS; i++)
                        for (int j = 0; j < COLUMNAS; j++)
                            if (mapa[i][j] == 'P') {
                                pos_y = i;
                                pos_x = j;
                            }
                    sonido();
                    break;
                case 2:
                    mostrar_mapa();
                    mostrar_animacion_ganaste();
                    en_juego = false;
                    break;
            }
        }
    }

    return 0;
}
