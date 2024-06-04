// Smorger: un juego sobre explorar una mazmorra. Aún está por desarrollar la idea.
// El funcionamiento de cada sala es: todo se mueve cuando el jugador se mueve.

// Implementación: la sala es un grafo, cuyos vértices pueden tener peso 1 o 0
// dependiendo si se trata de una coordenada en la que hay o no una pared,
// respectivamente. El jugador tiene asociadas unos atributos de posición y
// orientación (hacia donde mira, para poder disparar). Los enemigos también.
// Los enemigos estarán en una lista para poder iterar a través de ellos cuando
// toque actualizar su posición. Mantendremos una lista de proyectiles para
// lo mismo (disparos del jugador y disparos de los enemigos). El movimiento de
// los enemigos vendrá dado por un algoritmo de búsqueda en ANCHURA o similar,
// pudiendo especificar una profundidad máxima para que enemigos lejanos no
// vayan a por el jugador.


#define WIDTH 40    // Dimensiones de cada sala (en principio todas las salas tienen el mismo tamaño)
#define HEIGHT 20   // Intentar que sean pares para no marearnos al dividir cuando haya que centrar la pantalla

#define UP      0
#define LEFT    1
#define DOWN    2
#define RIGHT   3

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>  // para memset()
#include "room.h"

#define STATS_WIDTH 30
#define STATS_HEIGHT 10

#define WINKIES_LENGTH 100
#define MAX_PROJECTILES 100 //arbitrario
                            //posible mecánica para recargar los proyectiles y poder volver a disparar

// Estilo. Caracteres que representan cada entidad de la sala
//#define WALL ACS_BLOCK
//#define DOOR "  "

// Distintas orientaciones del smorger-hero, sujeto a ideas
#define PLAYER_UP    "^|"
#define PLAYER_DOWN  "|v"
#define PLAYER_LEFT  "<-"
#define PLAYER_RIGHT "->"

#define PROJECTILE_CHAR "oo" //diseño sujeto a cambios

struct smorger {
    int position[2];                    // Coordenadas del jugador
    int orientation;                    // Hacia donde mira (UP, LEFT, DOWN, RIGHT)
    int color;                          // Color del personaje
    int ammo;                           // Número de disparos en el cargador
    wchar_t winkies[WINKIES_LENGTH];    // Lista de powerups llamados winkies
    int winkies_count;                  // Número de winkies que tiene el jugador
};
typedef struct smorger smorger;

struct projectile {
    int position[2];    // Coordenadas del proyectil
    int orientation;    // Dirección del proyectil (UP, LEFT, DOWN, RIGHT)
    int active;         // Estado del proyectil (activo o no)
    int color;          // Todavía no implementado
    //incluiremos cosas que lo harán más smórgico
};
typedef struct projectile projectile;

void print_room(WINDOW* win, int** room, smorger player, projectile projectiles[], int num_projectiles);
void print_stats(WINDOW* win, smorger player);
void handle_input(smorger* player, int input, int** room, projectile projectiles[], int* num_projectiles);
void print_player(WINDOW* win, smorger player, int corner_y, int corner_x);
void change_room(smorger* player, int orientation);
void update_projectiles(projectile projectiles[], int* num_projectiles, int** room);
void check_size();

int main() {
    int **room;
    smorger player;
    projectile projectiles[MAX_PROJECTILES];
    int num_projectiles = 0;

    // Inicializar smorger. Podría ser una función si lo necesitamos en un futuro
    player.ammo = 20;
    player.winkies_count = 0;   // En realidad esto es innecesario porque el valor por defecto ya es 0

    initscr();
    curs_set(0);
    noecho();             // No muestra los caracteres mientras se escriben

    check_size();

    // Crear ventanas
    WINDOW* win_room = newwin(HEIGHT, WIDTH * 2, (LINES - HEIGHT) / 2, (COLS - STATS_WIDTH) / 2 - 3 - WIDTH);       // Sala
    WINDOW* win_stats = newwin(STATS_HEIGHT, STATS_WIDTH, (LINES - HEIGHT) / 2, (COLS - STATS_WIDTH) / 2 + WIDTH + 3);      // Estadísticas

    keypad(win_room, TRUE); // Permite el uso de las teclas de flecha. Hay que activarlo en la ventana

    // Provisional para probar el imprimir pantalla
    room = generate_room();
    player.position[0] = WIDTH / 2;
    player.position[1] = HEIGHT / 2;

    // Provisional. Añadir winkies
    player.winkies[0] = ACS_PI;
    player.winkies[1] = ACS_CKBOARD;
    player.winkies[2] = ACS_PLMINUS;
    player.winkies[3] = ACS_NEQUAL;
    player.winkies_count = 4;

    int ch;
    print_room(win_room, room, player, projectiles, num_projectiles);
    print_stats(win_stats, player);

    while ((ch = wgetch(win_room)) != 'q') { // Presiona 'q' para salir
        handle_input(&player, ch, room, projectiles, &num_projectiles);
        update_projectiles(projectiles, &num_projectiles, room);

        // Detectar si el smorger-hero cruza una puerta y cambiar de sala
        if (player.position[1] == 0 && player.position[0] == WIDTH / 2) {
            change_room(&player, UP);
        } else if (player.position[1] == HEIGHT - 1 && player.position[0] == WIDTH / 2) {
            change_room(&player, DOWN);
        } else if (player.position[0] == 0 && player.position[1] == HEIGHT / 2) {
            change_room(&player, LEFT);
        } else if (player.position[0] == WIDTH - 1 && player.position[1] == HEIGHT / 2) {
            change_room(&player, RIGHT);
        }
        print_room(win_room, room, player, projectiles, num_projectiles);
        print_stats(win_stats, player);
    }

    endwin();

    return 0;
}

// ADRIAN: esta parte igual se puede mejorar un poco. En cada case, comprobar si hay o no pared con el operador ternario. o igual está mejor así, no sé
// Maneja la entrada del usuario y mueve al player, además de los proyectiles.
void handle_input(smorger* player, int input, int** room, projectile projectiles[], int* num_projectiles) {
    int new_x = player->position[0];
    int new_y = player->position[1];

    switch(input) {
        case KEY_UP:
            new_y--;
            player->orientation = UP;
            break;

        case KEY_RIGHT:
            new_x++;
            player->orientation = RIGHT;
            break;

        case KEY_DOWN:
            new_y++;
            player->orientation = DOWN;
            break;

        case KEY_LEFT:
            new_x--;
            player->orientation = LEFT;
            break;

        case ' ': // Disparo
            if ((*player).ammo > 0) {
                projectiles[*num_projectiles].position[0] = player->position[0];
                projectiles[*num_projectiles].position[1] = player->position[1];
                projectiles[*num_projectiles].orientation = player->orientation;
                projectiles[*num_projectiles].active = 1;
                (*num_projectiles)++;
                (*player).ammo--;
            }
            break;
    }

    // Verificar si el nuevo movimiento es válido (no hay colisión con una pared)
    if (new_x >= 0 && new_x < WIDTH && new_y >= 0 && new_y < HEIGHT) {
        // Permitir el movimiento si es una puerta o no es el borde
        if ((new_y == 0 && new_x == WIDTH / 2) || (new_y == HEIGHT - 1 && new_x == WIDTH / 2) ||
            (new_x == 0 && new_y == HEIGHT / 2) || (new_x == WIDTH - 1 && new_y == HEIGHT / 2) ||
            room[new_y][new_x] == 0) {
            player->position[0] = new_x;
            player->position[1] = new_y;
        }
    }
}

// Cambia de sala cuando el smorger-hero cruza una puerta, en realidad solo te pone en
// la puerta contraria, ya se cambiará
// ADRIAN: por qué esto es un puntero?
void change_room(smorger* player, int orientation) {
    switch(orientation) {
        case UP:
            // El jugador cruza la puerta superior y aparece en la puerta inferior
            player->position[1] = HEIGHT - 2; // Altura menos 2 para evitar sobreescribir la pared inferior
            player->position[0] = WIDTH / 2;  // Centrado en la puerta
            break;
        case DOWN:
            // El jugador cruza la puerta inferior y aparece en la puerta superior
            player->position[1] = 1;         // Justo debajo de la pared superior
            player->position[0] = WIDTH / 2; // Centrado en la puerta
            break;
        case LEFT:
            // El jugador cruza la puerta izquierda y aparece en la puerta derecha
            player->position[0] = WIDTH - 2;  // Anchura menos 2 para evitar sobreescribir la pared derecha
            player->position[1] = HEIGHT / 2; // Centrado en la puerta
            break;
        case RIGHT:
            // El jugador cruza la puerta derecha y aparece en la puerta izquierda
            player->position[0] = 1;          // Justo a la derecha de la pared izquierda
            player->position[1] = HEIGHT / 2; // Centrado en la puerta
            break;
    }
}

// Muestra la sala. Cada coordenada se muestra como dos caracteres. Esta función también
// se encargará de mostrar los demás objetos (jugador, enemigos, proyectiles...)
void print_room(WINDOW* win, int** room, smorger player, projectile projectiles[], int num_projectiles) {
    // Limpiar la ventana antes de redibujar
    werase(win);

    // -----------
    //    Sala
    // -----------
    // Muestra los bordes de la sala
    wattron(win, A_STANDOUT);
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            // Mostrar las puertas
            if ((row == 0 && col == WIDTH / 2) || (row == HEIGHT - 1 && col == WIDTH / 2) ||
                (col == 0 && row == HEIGHT / 2) || (col == WIDTH - 1 && row == HEIGHT / 2)) {
                wattroff(win, A_STANDOUT);
                mvwprintw(win, row, 2 * col, "  "); // Si dejamos las paredes con este estilo, podemos mejorar este código
                wattron(win, A_STANDOUT);
            } else if (room[row][col] == 1) {
                // Mostrar las paredes
                mvwprintw(win, row, 2 * col, "  ");
            }
        }
    }
    wattroff(win, A_STANDOUT);

    // Muestra las puertas (mover lo arriba a aquí)
    // TODO: podemos hacer que la sala sea un struct room que tenga las
    // paredes en un array (no en una matriz. Esto tiene sus pegas y
    // sus contras) y un array con las puertas.

    // -----------
    //    Player
    // -----------
    switch(player.orientation) { //Imprime al player con la orientación adecuada
        case UP:
            mvwprintw(win, player.position[1], 2 * player.position[0], PLAYER_UP);
            break;
        case DOWN:
            mvwprintw(win, player.position[1], 2 * player.position[0], PLAYER_DOWN);
            break;
        case LEFT:
            mvwprintw(win, player.position[1], 2 * player.position[0], PLAYER_LEFT);
            break;
        case RIGHT:
            mvwprintw(win, player.position[1], 2 * player.position[0], PLAYER_RIGHT);
            break;
    }

    // -----------
    //    Proyectiles
    // -----------
    for (int i = 0; i < num_projectiles; i++) {
        if (projectiles[i].active) {
            mvwprintw(win, projectiles[i].position[1], 2 * projectiles[i].position[0], PROJECTILE_CHAR);
        }
    }

    wrefresh(win); // Actualiza la ventana con los nuevos cambios
}

void print_stats(WINDOW* win, smorger player) {
    werase(win);
    box(win, 0, 0);

    // Mostrar winkies.
    mvwprintw(win, 1, 2, "Winkies: ");
    for (int i = 0; i < player.winkies_count; i++) {
        waddch(win, player.winkies[i]);
        waddch(win, ' ');
        //wprintw(win, "%c ", player.winkies[i]);
    }

    // Mostrar munición
    mvwprintw(win, 3, 2, "Ketchungs: %d", player.ammo);

    wrefresh(win);
}

// Actualiza la posición de los proyectiles y devuelve el número de proyectiles en pantalla
void update_projectiles(projectile projectiles[], int* num_projectiles, int** room) {
    for (int i = 0; i < *num_projectiles; i++) {
        if (projectiles[i].active) {
            switch (projectiles[i].orientation) {
                case UP:
                    projectiles[i].position[1]--;
                    break;
                case DOWN:
                    projectiles[i].position[1]++;
                    break;
                case LEFT:
                    projectiles[i].position[0]--;
                    break;
                case RIGHT:
                    projectiles[i].position[0]++;
                    break;
            }

            // Verificar si el proyectil ha colisionado con una pared
            if (room[projectiles[i].position[1]][projectiles[i].position[0]] != 0) {
                projectiles[i].active = 0;
                (*num_projectiles)--;
            }
        }
    }
}

// Función para asegurar que el juego se puede iniciar según el tamaño de la terminal
// Se crea un marco con el tamaño mínimo de la terminal para jugar, el jugador puede comprobarlo
// antes de continuar con el juego para que no le crashee.
void check_size() {
    char horizontal[131];
    memset(horizontal, '-', 130);
    horizontal[130] = '\0'; // Carácter nulo al final

    char vertical[129];
    memset(vertical, ' ', 128);
    vertical[128] = '\0'; // Carácter nulo al final

    while (LINES < 30 || COLS < 130) {
        mvprintw(0, 0, "%s", horizontal);
        for (int i = 1; i < 30; i++) {
            mvprintw(i, 0, "|");
            mvprintw(i, 1, "%s", vertical);
            mvprintw(i, 129, "|");
        }
        mvprintw(30, 0, "%s", horizontal);

        mvprintw(LINES/2 - 1, (COLS - 77)/2, "Antes de continuar, comprueba que el marco cabe completamente en la terminal.");
        mvprintw(LINES/2 + 0, (COLS - 40)/2, "Redimensiona la terminal si lo necesitas.");
        mvprintw(LINES/2 + 1, (COLS - 35)/2, "Pulsa ENTER para volver a comprobar");

        getch();

        mvprintw(LINES/2 - 1, (COLS - 77)/2, "                                                                             ");
        mvprintw(LINES/2 + 0, (COLS - 40)/2, "                                         ");
        mvprintw(LINES/2 + 1, (COLS - 35)/2, "                                         ");
    }

    clear();
    refresh();
}
