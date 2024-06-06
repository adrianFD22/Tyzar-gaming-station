#include <stdlib.h>
#include <time.h>

#define size_of_array(x) sizeof(x)/sizeof((x)[0])
#define MAX_WALLS 22

int** generate_room();

// Un array de rooms. Cada room es un array indicando las paredes.
// Las paredes se indican con 4 enteros representando pos_x
// (de 1 a WIDTH-2), pos_y (de 1 a HEIGHT-2), orientación
// (hacia abajo, DOWN o hacia la derecha, RIGHT) y el tamaño.
struct room {
    int n_walls;
    int walls[MAX_WALLS][4];
    int doors[MAX_EDGES];
};

// NOTA: Se puede hacer mirror de las salas que lo permitan
// para tener más variedad.
const struct room poole_rooms[ ] = {
    // Room 0 (empty room)
    {
        .n_walls = 0,
        .walls = {
            
        }
    },

    // Room 1 (vertical wall, split in 2)
    {
        .n_walls = 2,
        .walls = {
            {WIDTH/3, 1, DOWN, HEIGHT/2 - 2},
            {WIDTH/3, HEIGHT/2+1, DOWN, HEIGHT/2 - 2}
        }
    },

    // Room 2 (cross walls, split in 4)
    {
        .n_walls = 2,
        .walls = {
            // Pared vertical de la cruz
            {WIDTH/2, 4, DOWN, HEIGHT - 8},  
            // Pared horizontal de la cruz
            {5, HEIGHT/2, RIGHT, WIDTH - 10}  
        }
    },

    // Room 3 (2 horizontal walls, split in 3)
    {
        .n_walls = 2,
        .walls = {
            {6, HEIGHT/3, RIGHT, 28},   // pared superior
            {6, 2*HEIGHT/3, RIGHT, 28}  // pared inferior
        }
    },

    // Room 4 (split in 3)
    {
        .n_walls = 3,
        .walls = {
            // Paredes verticales
            {2*WIDTH/3, 1, DOWN, 8},   {2*WIDTH/3, 12, DOWN, 7},

            // Pared horizontal
            {6, HEIGHT/2, RIGHT, 21}
            
        }
    },

    //------------------------------
    // ROOMS para niveles más altos
    //------------------------------

    // Room 5 (separated doors, groups of 2)
    {
        .n_walls = 6, 
        .walls = {
            // Pared superior
            {WIDTH/2 - 3, 1, DOWN, 4},  
            {WIDTH/2 - 3, HEIGHT/2 - 5, RIGHT, 18},
            {WIDTH - 6, 6, DOWN, 9},

            // Pared inferior
            {WIDTH/2 + 3, 15, DOWN, 4}, 
            {5, 14, RIGHT, 19},
            {5, 6, DOWN, 9},
        }
    },

    // Room 6 (corridors, little escape)
    {
        .n_walls = 22, 
        .walls = {
            //Bloque superior izquierdo      //Bloque superior derecho  
            {1, 1, RIGHT, 15},                {25, 1, RIGHT, 14},
            {1, 2, RIGHT, 15},                {25, 2, RIGHT, 14},
            {1, 3, RIGHT, 15},                {25, 3, RIGHT, 14},
            {1, 4, RIGHT, 15},                {25, 4, RIGHT, 14}, 
            {1, 5, RIGHT, 15},                {25, 5, RIGHT, 14}, 
            {1, 6, RIGHT, 15},                {25, 6, RIGHT, 14},

            //Bloque inferior izquierdo       //Bloque inferior derecho
            {1, 14, RIGHT, 15},               {25, 14, RIGHT, 14},
            {1, 15, RIGHT, 15},               {25, 15, RIGHT, 14},
            {1, 16, RIGHT, 15},               {25, 16, RIGHT, 14},
            {1, 17, RIGHT, 15},               {25, 17, RIGHT, 14},
            {1, 18, RIGHT, 15},               {25, 18, RIGHT, 14}
        }
    },

    // Room 7 (narrow path)  
    //  NOTA: Se pueden hacer 4 variaciones con las 4 direcciones, así
    //  hay más variedad de rooms.
    {
        .n_walls = 9, 
        .walls = {  
            // Paredes verticales
            {15, 1, DOWN, 5},
            {15, 14, DOWN, 5},

            // Paredes horizontales
            {15, 6, RIGHT, 14},   {15, 11, RIGHT, 14},
            {15, 7, RIGHT, 14},   {15, 12, RIGHT, 14},
            {15, 8, RIGHT, 14},   {15, 13, RIGHT, 14},
            {15, 9, RIGHT, 14}
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
    srand(time(NULL));
    random_room = rand() % n_rooms;    // Elegir una room aleatoria del poole. No es perfectamente aleatorio porque esto genera un número entre 0 y MAX_RAND
    //random_room = 0; // testeo de rooms
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