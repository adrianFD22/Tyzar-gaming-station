
#include <stdlib.h>

#define size_of_array(x) sizeof(x)/sizeof((x)[0])
#define MAX_WALLS 10

int** generate_room();


// Un array de rooms. Cada room es un array indicando las paredes.
// Las paredes se indican con 4 enteros representando pos_x
// (de 1 a WIDTH-2), pos_y (de 1 a HEIGHT-2), orientación
// (hacia abajo, DOWN o hacia la derecha, RIGHT) y el tamaño.
struct room {
    int n_walls;
    int walls[MAX_WALLS][4];
};

const struct room poole_rooms[ ] = {
    // Room 1 (split in 2)
    {
        .n_walls = 2,
        .walls = {
            {WIDTH/3, 1, DOWN, HEIGHT/2 - 2},
            {WIDTH/3, HEIGHT/2+1, DOWN, HEIGHT/2 - 2}
        }
    }
};
const int n_rooms = size_of_array(poole_rooms);

// Genera las paredes de una sala
int** generate_room() {
    int **room;
    int random_room, n_walls, wall_pos_x, wall_pos_y, wall_orientation, wall_length;

    // Inicializar la sala sin paredes
    room = (int**) malloc(HEIGHT * sizeof(int*));
    for (int row = 0; row < HEIGHT; row++) {
        room[row] = (int*) calloc(WIDTH, sizeof(int));
    }

    // Añadir las paredes exteriores
    for (int row = 0; row < HEIGHT; row++) {
        room[row][0] = 1;
        room[row][WIDTH - 1] = 1;
    }
    for (int col = 0; col < WIDTH; col++) {
        room[0][col] = 1;
        room[HEIGHT - 1][col] = 1;
    }

    // Añadir las paredes interiores
    random_room = rand() % n_rooms;    // Elegir una room aleatoria del poole. No es perfectamente aleatorio porque esto genera un número entre 0 y MAX_RAND
    n_walls = poole_rooms[random_room].n_walls;
    for (int i = 0; i < n_walls; i++) {
        // Extraer características de la pared
        wall_pos_x = poole_rooms[random_room].walls[i][0];
        wall_pos_y = poole_rooms[random_room].walls[i][1];
        wall_orientation = poole_rooms[random_room].walls[i][2];
        wall_length = poole_rooms[random_room].walls[i][3];

        // Añadir la pared
        if (wall_orientation == DOWN) {
            for (int j = 0; j < wall_length; j++) {
                room[wall_pos_y+j][wall_pos_x] = 1;
            }

        } else { // if wall_orientation == RIGHT
            for (int j = 0; j < wall_length; j++) {
                room[wall_pos_y][wall_pos_x+j] = 1;
            }
        }
    }

    return room;
}
