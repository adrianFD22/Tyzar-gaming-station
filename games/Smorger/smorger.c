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

// Mi intención para esta noche:
//  - [x] Programar la función de mostrar sala. Imprime la sala para poder visualizarla
//  - [ ] Programar un prototipo del jugador

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

// Estilo. Caracteres que representan cada entidad de la sala
#define WALL "##"   // Igual podemos poner caracteres sólidos
#define DOOR "  "

// Distintas orientaciones del smorger-hero, sujeto a ideas
#define PLAYER_UP    "^|"
#define PLAYER_DOWN  "|v"
#define PLAYER_LEFT  "<-"
#define PLAYER_RIGHT "->"
#define UP      0
#define LEFT    1
#define DOWN    2
#define RIGHT   3

struct player {
    int position[2];    // Coordenadas del jugador
    int orientation;    // Hacia donde mira (UP, LEFT, DOWN, RIGHT)
    int color;          // Color del personaje
    char** winkies;     // Lista de powerups llamados winkies
    int winkies_count;  // Número de winkies que tiene el jugador
};
typedef struct player player;

void print_room(WINDOW* win, int** room, player user);
int** generate_room();
void handle_input(player* user, int input, int** room);
void print_player(WINDOW* win, player user, int corner_y, int corner_x);
void change_room(player* user, int direction);

int main() {
    int **room;
    player user;    // Este nombre no me gusta para la variable user xd 
                    // Yo propongo smorger-hero

    // Inicializar la lista de winkies, se irán recogiendo por el mapa
    user.winkies = NULL;
    user.winkies_count = 0;

    initscr();
    curs_set(0);
    keypad(stdscr, TRUE); // Permite el uso de las teclas de flecha
    noecho();             // No muestra los caracteres mientras se escriben

    // Crear una nueva ventana para el movimiento del smorger-hero
    WINDOW* win = newwin(HEIGHT, WIDTH * 2, (LINES - HEIGHT) / 2, (COLS - WIDTH * 2) / 2);
    wrefresh(win);

    // Provisional para probar el imprimir pantalla
    room = generate_room();
    user.position[0] = WIDTH / 2;
    user.position[1] = HEIGHT / 2;

    int ch;
    while ((ch = getch()) != 'q') { // Presiona 'q' para salir
    handle_input(&user, ch, room);

    // Detectar si el smorger-hero cruza una puerta y cambiar de sala
    if (user.position[1] == 0 && user.position[0] == WIDTH / 2) {
        change_room(&user, UP);
    } else if (user.position[1] == HEIGHT - 1 && user.position[0] == WIDTH / 2) {
        change_room(&user, DOWN);
    } else if (user.position[0] == 0 && user.position[1] == HEIGHT / 2) {
        change_room(&user, LEFT);
    } else if (user.position[0] == WIDTH - 1 && user.position[1] == HEIGHT / 2) {
        change_room(&user, RIGHT);
    }
    print_room(win, room, user);
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

    // Añadir las puertas
    room[0][WIDTH / 2] = 0;               // Puerta superior
    room[HEIGHT - 1][WIDTH / 2] = 0;      // Puerta inferior
    room[HEIGHT / 2][0] = 0;              // Puerta izquierda
    room[HEIGHT / 2][WIDTH - 1] = 0;      // Puerta derecha

    return room;
}

// Maneja la entrada del usuario y mueve al smorger-hero (y a futuro los enemigos)
void handle_input(player* user, int input, int** room) {
    int new_x = user->position[0];
    int new_y = user->position[1];

    switch(input) {
        case KEY_UP:  // Ponte el modo vim como tú quieras
            new_y--;
            user->orientation = UP;
            break;
        case KEY_DOWN:
            new_y++;
            user->orientation = DOWN;
            break;
        case KEY_LEFT:
            new_x--;
            user->orientation = LEFT;
            break;
        case KEY_RIGHT:
            new_x++;
            user->orientation = RIGHT;
            break;
    }

    // Verificar si el nuevo movimiento es válido (no hay colisión con una pared)
    if (new_x >= 0 && new_x < WIDTH && new_y >= 0 && new_y < HEIGHT) {
        // Permitir el movimiento si es una puerta o no es el borde
        if ((new_y == 0 && new_x == WIDTH / 2) || (new_y == HEIGHT - 1 && new_x == WIDTH / 2) ||
            (new_x == 0 && new_y == HEIGHT / 2) || (new_x == WIDTH - 1 && new_y == HEIGHT / 2) ||
            room[new_y][new_x] == 0) {
            user->position[0] = new_x;
            user->position[1] = new_y;
        }
    }
}

// Cambia de sala cuando el smorger-hero cruza una puerta, en realidad solo te pone en 
// la puerta contraria, ya se cambiará
void change_room(player* user, int direction) {
    switch(direction) {
        case UP:
            // El jugador cruza la puerta superior y aparece en la puerta inferior
            user->position[1] = HEIGHT - 2; // Altura menos 2 para evitar sobreescribir la pared inferior
            user->position[0] = WIDTH / 2;  // Centrado en la puerta
            break;
        case DOWN:
            // El jugador cruza la puerta inferior y aparece en la puerta superior
            user->position[1] = 1;         // Justo debajo de la pared superior
            user->position[0] = WIDTH / 2; // Centrado en la puerta
            break;
        case LEFT:
            // El jugador cruza la puerta izquierda y aparece en la puerta derecha
            user->position[0] = WIDTH - 2;  // Anchura menos 2 para evitar sobreescribir la pared derecha
            user->position[1] = HEIGHT / 2; // Centrado en la puerta
            break;
        case RIGHT:
            // El jugador cruza la puerta derecha y aparece en la puerta izquierda
            user->position[0] = 1;          // Justo a la derecha de la pared izquierda
            user->position[1] = HEIGHT / 2; // Centrado en la puerta
            break;
    }
}

// Muestra la sala. Cada coordenada se muestra como dos caracteres. Esta función también
// se encargará de mostrar los demás objetos (jugador, enemigos, proyectiles...)
void print_room(WINDOW* win, int** room, player user) {
    int corner_x, corner_y;

    corner_y = 0; // Ajustado a la esquina superior de la ventana win
    corner_x = 0; // Ajustado a la esquina superior de la ventana win

    // Limpiar la ventana antes de redibujar
    werase(win);

    // Mostrar la lista de winkies en la parte superior de la pantalla principal
    mvprintw(0, 0, "Winkies: ");
    for (int i = 0; i < user.winkies_count; i++) {
        printw("%s ", user.winkies[i]);
    }

    // -----------
    //    Sala
    // -----------
    // Muestra los bordes de la sala dentro de la ventana y añade puertas
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            // Mostrar las puertas
            if ((row == 0 && col == WIDTH / 2) || (row == HEIGHT - 1 && col == WIDTH / 2) ||
                (col == 0 && row == HEIGHT / 2) || (col == WIDTH - 1 && row == HEIGHT / 2)) {
                mvwprintw(win, corner_y + row, corner_x + 2 * col, DOOR);
            } else if (room[row][col] == 1) {
                // Mostrar las paredes
                mvwprintw(win, corner_y + row, corner_x + 2 * col, WALL);
            }
        }
    }

    // -----------
    //    Player
    // -----------
    print_player(win, user, corner_y, corner_x);

    wrefresh(win); // Actualiza la ventana con los nuevos cambios
}

// Imprime el jugador con la orientación adecuada
void print_player(WINDOW* win, player user, int corner_y, int corner_x) {
    switch(user.orientation) {
        case UP:
            mvwprintw(win, corner_y + user.position[1], corner_x + 2 * user.position[0], PLAYER_UP);
            break;
        case DOWN:
            mvwprintw(win, corner_y + user.position[1], corner_x + 2 * user.position[0], PLAYER_DOWN);
            break;
        case LEFT:
            mvwprintw(win, corner_y + user.position[1], corner_x + 2 * user.position[0], PLAYER_LEFT);
            break;
        case RIGHT:
            mvwprintw(win, corner_y + user.position[1], corner_x + 2 * user.position[0], PLAYER_RIGHT);
            break;
    }
}
