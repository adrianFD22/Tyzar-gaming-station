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

// Chorradas: el juego tiene que tener muuuchos nombres extraños para que al
// jugarlo digamos cosas como "mi smorger franchula 4 merters pero solo extrae 2"

// Notas:
//  - Por favor (no me pidas que deje de ser hombre), primero el main y luego las funciones
//  - Hay que separar las cosas lógicas (mover personaje, colisión de proyectil, color de
//      las cosas...) de las cosas de visualizar (mostrar pantalla, print).
//  - Quizás este es un buen proyecto para usar distintas windows (ncurses), para no
//      estar todo el rato refrescando la pantalla
//  - Idea: podemos hacer un nivel que para pasártelo haya que hacer la pantalla
//      pequeña o algo así. Aunque, una vez resuelto ya no tiene gracia. Lo suyo
//      es que este juego sea procedural, no se acabe.
//  - Tengo la duda de si hacer que los enemigos y el jugador tengan el mismo tipo,
//      "objetos" del mismo struct. Creo que no es buena idea porque el jugador
//      podría tener cosas como "munición" que los enemigos no.

//NOTAS DIEGO:
// - Powerups llamados winkies que hagan cosas, tal vez acumulables como en el isaac.
// - He puesto un marquito para saber las dimensiones de la sala, que tendrá que ser un tamaño fijo.
//      Actualización: igual quito el marquito porque me está molestando con las puertas, descubrirás de qué
//      hablo en las notas de abajo, te me cuidas makina un salu2.
// - El movimiento del personaje va en una ventana distinta de ncurses para no tener que referscar todo.
//      - ADRIAN: Buena idea!
// - Implementadas las orientciones del movimiento del smorger.
// - No sé por que no se ven las puertas de las salas y no se puede cruzar jeje, en realidad al cruzar
//      la puerta (cuando se arregle el bug) solo apareces por la puerta contraria, hay que mejorarlo para
//      que se tenga en cuenta que cada sala es diferente con bjetos y eso.
// - He quitado el marquito ese de mierda y en su lugar hay '##'
// - Quiero aprovechar el espacio fuera de la sala en la terminal para poner cosas como info del smorger-hero
//       o los winkies (me ha encantado escribir esta frase)

#include <ncurses.h>
#include <stdlib.h>

#define WIDTH 40    // Dimensiones de cada sala (en principio todas las salas tienen el mismo tamaño)
#define HEIGHT 20   // Intentar que sean pares para no marearnos al dividir cuando haya que centrar la pantalla

#define STATS_WIDTH 30
#define STATS_HEIGHT 10

#define WINKIES_LENGTH 100

// Estilo. Caracteres que representan cada entidad de la sala
<<<<<<< HEAD
#define WALL "##"   // Igual podemos poner caracteres sólidos
#define DOOR "  "
=======
//#define WALL ACS_BLOCK
//#define DOOR "  "
>>>>>>> 85a409d5aced580fbb2dd2e9d04671fc44dc2e3e

// Distintas orientaciones del smorger-hero, sujeto a ideas
#define PLAYER_UP    "^|"
#define PLAYER_DOWN  "|v"
#define PLAYER_LEFT  "<-"
#define PLAYER_RIGHT "->"
#define UP      0
#define LEFT    1
#define DOWN    2
#define RIGHT   3

struct smorger {
    int position[2];    // Coordenadas del jugador
    int orientation;    // Hacia donde mira (UP, LEFT, DOWN, RIGHT)
    int color;          // Color del personaje
<<<<<<< HEAD
    char** winkies;     // Lista de powerups llamados winkies
=======
    // Lista de powerups (winkies)
    wchar_t winkies[WINKIES_LENGTH];     // Lista de powerups llamados winkies
>>>>>>> 85a409d5aced580fbb2dd2e9d04671fc44dc2e3e
    int winkies_count;  // Número de winkies que tiene el jugador
};
typedef struct smorger smorger;

int** generate_room();
void print_room(WINDOW* win, int** room, smorger player);
void print_stats(WINDOW* win, smorger player);
void handle_input(smorger* player, int input, int** room);
void print_player(WINDOW* win, smorger player, int corner_y, int corner_x);
void change_room(smorger* player, int direction);

int main() {
    int **room;
    smorger player;

    // Inicializar la lista de winkies, se irán recogiendo por el mapa
    player.winkies_count = 0;

    initscr();
    curs_set(0);
    noecho();             // No muestra los caracteres mientras se escriben

    // Crear ventanas
    WINDOW* win_room = newwin(HEIGHT, WIDTH * 2, (LINES - HEIGHT) / 2, (COLS - STATS_WIDTH) / 2 - 3 - WIDTH);       // Sala
    WINDOW* win_stats = newwin(STATS_HEIGHT, STATS_WIDTH, (LINES - HEIGHT) / 2, (COLS - STATS_WIDTH) / 2 + WIDTH + 3);      // Estadísticas
    box(win_stats, 0, 0);
    wrefresh(win_room);

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
    while ((ch = wgetch(win_room)) != 'q') { // Presiona 'q' para salir
    handle_input(&player, ch, room);

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
    print_room(win_room, room, player);
    print_stats(win_stats, player);
    }

    endwin();

    return 0;
}


// Genera las paredes de una sala
int** generate_room() {
    int **room;

    // Inicializar la sala sin paredes
    room = (int**) malloc(HEIGHT * sizeof(int*));
    for (int row = 0; row < HEIGHT; row++) {
        room[row] = (int*) calloc(WIDTH, sizeof(int));
    }

    // Añadir las paredes
    for (int row = 0; row < HEIGHT; row++) {
        room[row][0] = 1;
        room[row][WIDTH - 1] = 1;
    }
    for (int col = 0; col < WIDTH; col++) {
        room[0][col] = 1;
        room[HEIGHT - 1][col] = 1;
    }

    return room;
}


// ADRIAN: por qué el smorger es un puntero?
// ADRIAN: creo que esto no tendría que manejar a los enemigos
// ADRIAN: esta parte igual se puede mejorar un poco. En cada case, comprobar si hay o no pared con el operador ternario. o igual está mejor así, no sé
// Maneja la entrada del usuario y mueve al smorger-hero (y a futuro los enemigos)
void handle_input(smorger* player, int input, int** room) {
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
void change_room(smorger* player, int direction) {
    switch(direction) {
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
void print_room(WINDOW* win, int** room, smorger player) {
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
    print_player(win, player, 0, 0);

    wrefresh(win); // Actualiza la ventana con los nuevos cambios
}

void print_stats(WINDOW* win, smorger player) {
    // Mostrar winkies.
    mvwprintw(win, 1, 2, "Winkies: ");
    for (int i = 0; i < player.winkies_count; i++) {
        waddch(win, player.winkies[i]);
        waddch(win, ' ');
        //wprintw(win, "%c ", player.winkies[i]);
    }

    wrefresh(win);
}

// ADRIAN: esto lo podemos meter dentro de print_room
// Imprime el jugador con la orientación adecuada
void print_player(WINDOW* win, smorger player, int corner_y, int corner_x) {
    switch(player.orientation) {
        case UP:
            mvwprintw(win, corner_y + player.position[1], corner_x + 2 * player.position[0], PLAYER_UP);
            break;
        case DOWN:
            mvwprintw(win, corner_y + player.position[1], corner_x + 2 * player.position[0], PLAYER_DOWN);
            break;
        case LEFT:
            mvwprintw(win, corner_y + player.position[1], corner_x + 2 * player.position[0], PLAYER_LEFT);
            break;
        case RIGHT:
            mvwprintw(win, corner_y + player.position[1], corner_x + 2 * player.position[0], PLAYER_RIGHT);
            break;
    }
}
