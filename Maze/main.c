
// MAZED: a game about solving a laberynth. A maze is generated using a random deep-
// first algorithm to explore the graph of all possible chambers (see wikipedia). The
// player has to find the exit to the laberynth.

// idea: hacer que la interfaz de la play6 te dé una descripción de cada juego al poner
// el cursor encima y que diga qué mente maestra lo ha escrito (nuevos nombres de
// programadores yuju!).

#include <time.h>
#include <stdlib.h>
#include <ncurses.h>


#define WIDTH 10        // width of a maze
#define HEIGHT 10       // height of a maze

// Macros for generate mazes
#define OPEN_LEFT 1
#define OPEN_RIGHT (1 << 1)
#define OPEN_UP (1 << 2)
#define OPEN_DOWN (1 << 3)

// Macros for computing corners:
// open_t + 2*open_r + 4*open_b + 8*open_l
#define CORNER_0 0
#define CORNER_T 1
#define CORNER_R 2
#define CORNER_B 4
#define CORNER_L 8
#define CORNER_TR 3 // (1 + 2)
#define CORNER_TB 5 // (1 + 4)
#define CORNER_TL 9 // (1 + 8)
#define CORNER_RB 6 // (2 + 4)
#define CORNER_RL 10 // (2 + 8)
#define CORNER_BL 12 // (4 + 8)
#define CORNER_TRB 7 // (1 + 2 + 4)
#define CORNER_TRL 11 // (1 + 2 + 8)
#define CORNER_TBL 13 //(1 + 4 + 8)
#define CORNER_RBL 14 //(2 + 4 + 8)
#define CHAMBER_WIDTH 3
#define CHAMBER_HEIGHT 1

// User skin
#define SKIN ACS_BLOCK
//#define SKIN " O "

// A maze is given by an array representing closed/open walls.
//typedef int maze[WIDTH][HEIGHT];

int** generate_maze();
void print_maze(int** maze, int top_border, int left_border);

int main() {
    int term_size_x, term_size_y, top_border, left_border;
    int center_x, center_y;

    int** maze;
    int pos_x, pos_y;
    int real_pos_x, real_pos_y;
    wchar_t pressed_key;

    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();

    while (1) {
        //---------------------------
        //       Welcome screen
        //---------------------------

        term_size_x = getmaxx(stdscr);
        term_size_y = getmaxy(stdscr);
        center_x = term_size_x / 2;
        center_y = term_size_y / 2;

        clear();
        mvprintw(center_y-6, center_x-19, " _       _    ______    _____    ____");
        mvprintw(center_y-5, center_x-19, "|  \\   /  |  |  __  |  |___  |  |   _|");
        mvprintw(center_y-4, center_x-19, "| | \\ / | |  | |__| |     / /   |  |_");
        mvprintw(center_y-3,   center_x-19, "| | \\ / | |  | |  | |   / / _   |  |_");
        mvprintw(center_y-2, center_x-19, "|_|     |_|  |_|  |_|  |_____|  |____|");
        attron(A_BLINK);
        mvprintw(center_y+1, center_x-5, "Press enter");
        attroff(A_BLINK);

        do {
            pressed_key = getch();
            if (pressed_key == 'q') goto end_game;
        } while (pressed_key != '\n');

        maze = generate_maze();
        pos_x = 0;
        pos_y = 0;


        //---------------------------
        //        Game screen
        //---------------------------

        // Play game
        while (1) {
            // Compute borders of the maze
            term_size_x = getmaxx(stdscr);
            term_size_y = getmaxy(stdscr);
            center_x = term_size_x / 2;
            center_y = term_size_y / 2;

            top_border = center_y - ((CHAMBER_HEIGHT+1)*HEIGHT/2);
            left_border = center_x - ((CHAMBER_WIDTH+1)*WIDTH/2);

            // Print game
            clear();
            print_maze(maze, top_border, left_border);      // Prints the walls of the maze

            if (pos_x == WIDTH) goto win_game;      // Handle game winning
            real_pos_x = left_border + (CHAMBER_WIDTH+1)*pos_x + 1;
            real_pos_y = top_border + (CHAMBER_HEIGHT+1)*pos_y + 1;
            //mvprintw(real_pos_y, real_pos_x, SKIN);         // Prints the player fire skin
            mvaddch(real_pos_y+(CHAMBER_HEIGHT/2), real_pos_x+(CHAMBER_WIDTH/2), SKIN);

            // Get user's input
            pressed_key = getch();
            switch (pressed_key) {
                case KEY_LEFT:
                case 'h':
                    if ( pos_x > 0 && maze[pos_y][pos_x] & OPEN_LEFT) pos_x--;
                    break;

                case KEY_DOWN:
                case 'j':
                    if ( maze[pos_y][pos_x] & OPEN_DOWN) pos_y++;
                    break;

                case KEY_UP:
                case 'k':
                    if ( maze[pos_y][pos_x] & OPEN_UP) pos_y--;
                    break;

                case KEY_RIGHT:
                case 'l':
                    if ( maze[pos_y][pos_x] & OPEN_RIGHT) pos_x++;
                    break;

                case 'q':
                    goto end_game;
                    break;
            }
        }

        //---------------------------
        //       Winning screen
        //---------------------------

        // Print message after winning
        win_game:
        term_size_x = getmaxx(stdscr);
        term_size_y = getmaxy(stdscr);
        center_x = term_size_x / 2;
        center_y = term_size_y / 2;

        mvprintw(center_y-1, center_x-7,   "               ");
        mvprintw(center_y,   center_x-7,     "               ");
        mvprintw(center_y+1, center_x-7,   "               ");
        mvprintw(center_y+2, center_x-7,   "               ");
        mvprintw(center_y+3, center_x-7,   "               ");
        mvprintw(center_y, center_x-3, "You won!");
        mvprintw(center_y+2, center_x-5, "Press enter");

        do {
            pressed_key = getch();
            if (pressed_key == 'q') goto end_game;
        } while (pressed_key != '\n');
    }

    end_game:
    endwin();
}


int** generate_maze() {
    int stack[HEIGHT*WIDTH][2];     // Stack of chambers given by its coordinates with pending unvisited neighbors
    int len_stack;                  // Number of chambers currently in the stack, lenght of the stack

    int curr_chamber_data;          // The data stored in gen_maze of current chamber
    int i, j;                       // Coodinates of current chamber

    char unvisited_neigh[4];        // Unvisited neighbors of current chamber
    int n_unvisited_neigh;          // Number of unvisited neighbors of current chamber
    int rand_index;                 // Index of a random neighbor to visit

    // Create maze
    int** maze = (int**) malloc(sizeof(int*) * HEIGHT);                  // The maze to return at the end of the function
    for (int i = 0; i < HEIGHT; i++) {
        maze[i] = (int*) calloc(WIDTH, sizeof(int));    // I think that malloc was problematic because the value after allocation is not zero. I use calloc to fix a bug
    }

    srand(time(NULL));

    // Mark exit wall as open
    maze[HEIGHT-1][WIDTH-1] |=  OPEN_RIGHT;

    // Add initial chamber to the stack
    len_stack = 1;
    stack[len_stack-1][0] = HEIGHT-1;
    stack[len_stack-1][1] = WIDTH-1;

    // While stack is not empty
    while (len_stack != 0) {
        // Get chamber from stack (not pop)
        i = stack[len_stack-1][0];
        j = stack[len_stack-1][1];

        curr_chamber_data = maze[i][j];

        // Get all unvisited neighbors of current chamber.
        // In each direction up, right, down and left, check if
        // the neighbor is not at the border of the maze and that
        // is not visited yet.
        n_unvisited_neigh = 0;

        if ( (i > 0) && (maze[i-1][j] == 0) ) {             // Check top neighbor
            n_unvisited_neigh++;
            unvisited_neigh[n_unvisited_neigh-1] = 'T';             // Mark Top as unvisited
        }

        if ( (j < WIDTH-1) && (maze[i][j+1] == 0) ) {       // Check right neighbor
            n_unvisited_neigh++;
            unvisited_neigh[n_unvisited_neigh-1] = 'R';             // Mark Right as unvisited
        }

        if ( (i < HEIGHT-1) && (maze[i+1][j] == 0) ) {      // Check bottom neighbor
            n_unvisited_neigh++;
            unvisited_neigh[n_unvisited_neigh-1] = 'B';             // Mark Bottom as unvisited
        }

        if ( (j > 0) && (maze[i][j-1] == 0) ) {             // Check left neighbor
            n_unvisited_neigh++;
            unvisited_neigh[n_unvisited_neigh-1] = 'L';             // Mark Left as unvisited
        }

        if (n_unvisited_neigh == 0) {
            // If all neighbors are visited, remove current chamber from stack
            len_stack--;
        } else {
            len_stack++;

            // Pick random unvisited neighbor
            rand_index = rand() % n_unvisited_neigh;

            // Mark corresponding wall as open and add new neighbor to stack
            switch (unvisited_neigh[rand_index]) {
                case 'T':
                    maze[i][j]   |= OPEN_UP;
                    maze[i-1][j] |= OPEN_DOWN;

                    stack[len_stack-1][0] = i-1;
                    stack[len_stack-1][1] = j;
                    break;

                case 'R':
                    maze[i][j]   |= OPEN_RIGHT;
                    maze[i][j+1] |= OPEN_LEFT;

                    stack[len_stack-1][0] = i;
                    stack[len_stack-1][1] = j+1;
                    break;

                case 'B':
                    maze[i][j]   |= OPEN_DOWN;
                    maze[i+1][j] |= OPEN_UP;

                    stack[len_stack-1][0] = i+1;
                    stack[len_stack-1][1] = j;
                    break;

                case 'L':
                    maze[i][j]   |= OPEN_LEFT;
                    maze[i][j-1] |= OPEN_RIGHT;

                    stack[len_stack-1][0] = i;
                    stack[len_stack-1][1] = j-1;
                    break;
            }
        }
    }
    // Mark entry chamber as visited
    maze[0][0]              |=  OPEN_LEFT;

    return maze;
}

void print_maze(int** maze, int top_border, int left_border) {
    int curr_x, curr_y;

    int open_t, open_r, open_b, open_l;
    chtype corner_char;


    // Draw the corners
    for (int i = 0; i < HEIGHT+1; i++) {
        for (int j = 0; j < WIDTH+1; j++) {
            open_t = 1;
            open_r = 1;
            open_b = 1;
            open_l = 1;

            // Get walls surrounding current corner
            if (i > 0) {            // Top wall
                open_t = j > 0 ? (maze[i-1][j-1] & OPEN_RIGHT) != 0 : (maze[i-1][j] & OPEN_LEFT) != 0;
            }

            if (j < WIDTH) {        // Right wall
                open_r = i > 0 ? (maze[i-1][j] & OPEN_DOWN) != 0: (maze[i][j] & OPEN_UP) != 0;
            }

            if (i < HEIGHT) {       // Bottom wall
                open_b = j > 0 ? (maze[i][j-1] & OPEN_RIGHT) != 0 : (maze[i][j] & OPEN_LEFT) != 0;
            }

            if (j > 0) {            // Left wall
                open_l = i > 0 ? (maze[i-1][j-1] & OPEN_DOWN) != 0 : (maze[i][j-1] & OPEN_UP) != 0;
            }

            // Choose corresponding character for printing the corner
            switch (open_t + 2*open_r + 4*open_b + 8*open_l) {
                case CORNER_0:
                    corner_char = ACS_PLUS;
                    break;
                case CORNER_T:
                    corner_char = ACS_TTEE;
                    break;
                case CORNER_R:
                    corner_char = ACS_RTEE;
                    break;
                case CORNER_B:
                    corner_char = ACS_BTEE;
                    break;
                case CORNER_L:
                    corner_char = ACS_LTEE;
                    break;
                case CORNER_TR:
                    corner_char = ACS_URCORNER;
                    break;
                case CORNER_TB:
                    corner_char = ACS_HLINE;
                    break;
                case CORNER_TL:
                    corner_char = ACS_ULCORNER;
                    break;
                case CORNER_RB:
                    corner_char = ACS_LRCORNER;
                    break;
                case CORNER_RL:
                    corner_char = ACS_VLINE;
                    break;
                case CORNER_BL:
                    corner_char = ACS_LLCORNER;
                    break;
                case CORNER_TRB:
                    corner_char = ACS_HLINE;
                    break;
                case CORNER_TRL:
                    corner_char = ACS_VLINE;
                    break;
                case CORNER_TBL:
                    corner_char = ACS_HLINE;
                    break;
                case CORNER_RBL:
                    corner_char = ACS_VLINE;
                    break;
            }

            // Finally print the corner!
            curr_x = left_border + (CHAMBER_WIDTH+1)*j;
            curr_y = top_border + (CHAMBER_HEIGHT+1)*i;

            mvaddch(curr_y, curr_x, corner_char);
        }
    }

    // Draw walls
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            curr_x = left_border + (CHAMBER_WIDTH+1)*j + 1;
            curr_y = top_border + (CHAMBER_HEIGHT+1)*i + 1;

            if (! (maze[i][j] & OPEN_LEFT)) {
                for (int k = 0; k < CHAMBER_HEIGHT; k++) {
                    mvaddch(curr_y+k, curr_x-1, ACS_VLINE);
                }
            }

            if (! (maze[i][j] & OPEN_DOWN)) {
                for (int k = 0; k < CHAMBER_WIDTH; k++) {
                    mvaddch(curr_y+CHAMBER_HEIGHT, curr_x+k, ACS_HLINE);
                }
            }

            if (! (maze[i][j] & OPEN_RIGHT)) {
                for (int k = 0; k < CHAMBER_HEIGHT; k++) {
                    mvaddch(curr_y+k, curr_x+CHAMBER_WIDTH, ACS_VLINE);
                }
            }

            if (! (maze[i][j] & OPEN_UP)) {
                for (int k = 0; k < CHAMBER_WIDTH; k++) {
                    mvaddch(curr_y-1, curr_x+k, ACS_HLINE);
                }
            }
        }
    }
}
