
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

#include <ncurses.h>
#include <stdlib.h>


#define WIDTH 40    // Dimensiones de cada sala (en principio todas las salas tienen el mismo tamaño)
#define HEIGHT 20   // Intentar que sean pares para no marearnos al dividir cuando haya que centrar la pantalla

// Estilo. Caracteres que representan cada entidad de la sala
#define WALL "##"
#define PLAYER "|-"

#define UP      0
#define LEFT    1
#define DOWN    2
#define RIGHT   3

struct player {
    int position[2];    // Coordenadas del jugador
    int orientation;    // Hacia donde mira (UP, LEFT, DOWN, RIGHT)
    int color;          // Color del personaje

    // ...
    // Más movidas que harán el juego smórgico!
};
typedef struct player player;

void print_room(int** room, player user);
int** generate_room();


int main() {
    int **room;
    player user;    // Este nombre no me gusta para la variable user xd

    initscr();
    curs_set(0);

    // Provisional para probar el imprimir pantalla
    room = generate_room();
    user.position[0] = WIDTH/2;
    user.position[1] = HEIGHT/2;

    print_room(room, user);
    getch();

    endwin();

    return 0;
}

// Genera las paredes de una sala. Provisional: genera todo sin paredes
int** generate_room() {
    int **room;

    // Inicializar la sala sin paredes
    room = (int**) malloc(HEIGHT * sizeof(int**));
    for (int row = 0; row < HEIGHT; row++) {
        room[row] = (int*) calloc(WIDTH, sizeof(int*));
    }

    return room;
}

// Muestra la sala. Cada coordenada se muestra como dos caracteres. Esta función también
// se encargará de mostrar los demás objetos (jugador, enemigos, proyectiles...)
void print_room(int** room, player user) {
    int corner_x, corner_y;

    corner_y = (getmaxy(stdscr)/2) - HEIGHT/2;     // La esquina superior...
    corner_x = (getmaxx(stdscr)/2) - WIDTH;        // izquierda (cada coordenada son dos caracteres)

    // -----------
    //    Sala
    // -----------
    // Muestra los bordes de la sala
    for (int row = -1; row < HEIGHT+1; row++) {
        mvprintw(corner_y + row, corner_x-2, WALL);
        mvprintw(corner_y + row, corner_x+2*WIDTH, WALL);
    }
    for (int col = -1; col < WIDTH+1; col++) {
        mvprintw(corner_y-1, corner_x + 2*col, WALL);
        mvprintw(corner_y+HEIGHT, corner_x + 2*col, WALL);
    }

    // Muestra cada coordenada con las paredes pertinentes
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            if (room[row][col] == 1) mvprintw(corner_y+row, corner_x+2*col, WALL);
        }
    }

    // -----------
    //    Player
    // -----------
    mvprintw(corner_y + user.position[1], corner_x + 2*user.position[0], PLAYER);

}
