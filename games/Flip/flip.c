
// B&W: a board consists of white and black squares. Player can select
// an upper left rectangular region and flip the colors of it. The objective is to
// flip all the colors to only black.

#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h> // Just for using sleep, wow

#define STR_LEN 100
#define MENU_SIZE 3
#define SLEEP_TIME 1000000

// Data structure and macros for handling options
struct opt {
    int* values;
    int* top_values;
    int len;
    int sel;
};

typedef struct opt opt;
#define inc_sel_value(options) if ((options).top_values[(options).sel] == -1 || (options).values[(options).sel] < (options).top_values[(options).sel] - 1) (options).values[(options).sel]++
#define dec_sel_value(options) if ((options).values[(options).sel] > 0) (options).values[(options).sel]--
#define inc_selected(options) if ((options).sel < (options).len-1) (options).sel++
#define dec_selected(options) if ((options).sel > 0) (options).sel--

#define MODE        options.values[0]
#define ENTROPY     options.values[1]
#define BOARD       options.values[2]
#define N_ROWS      options.values[3]
#define N_COLS      options.values[4]

#define TOP_MODE        options.top_values[0]
#define TOP_ENTROPY     options.top_values[1]
#define TOP_BOARD       options.top_values[2]
#define TOP_N_ROWS      options.top_values[3]
#define TOP_N_COLS      options.top_values[4]

#define OPTIONS_OFFSET 8

void rectangle(int y1, int x1, int y2, int x2);
void save_board(int **board, int n_rows, int n_cols);
int load_board(int*** board, char* file_name);
int get_entropy(int** board, int n_rows, int n_cols);

int main() {
    int menu_sel;
    int menu_board[MENU_SIZE][2*MENU_SIZE];

    opt options;
    int option_chosen;

    //char** board_names;
    char *board_name;
    int board_name_len;

    int n_white;
    int **board, **board_backup;
    int n_rows, n_cols;
    int sel_col, sel_row;
    wchar_t pressed_key;

    int center_x, center_y;
    int c1_to_print, c2_to_print;
    int pos_x, pos_y;
    int border_top, border_right, border_bottom, border_left;

    int entropy;

    srand(time(NULL));

    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    // Initialize options with default values
    options.sel = 0;
    options.len = 5;
    options.values = (int*) malloc(options.len * sizeof(int));
    MODE        = 0;                  // Mode, by default normal
    ENTROPY     = 0;                  // Entropy, by default false
    BOARD       = 0;                  // Board, by default random
    N_ROWS      = 4;                  // Number of rows, by default
    N_COLS      = 4;                  // Number of columns, by default

    options.top_values = (int*) malloc(options.len * sizeof(int));
    TOP_MODE        = 2;              // Mode, normal or zen
    TOP_ENTROPY     = 2;              // Entropy, true or false
    TOP_BOARD       = 2;              // Board, random or load from file
    TOP_N_ROWS      = -1;             // Number of rows
    TOP_N_COLS      = -1;             // Number of columns

    board_name = (char*) calloc(STR_LEN, sizeof(char));
    board_name_len = 0;

    // Initialize menu board
    for (int row = 0; row < MENU_SIZE; row++) {
        for (int col = 0; col < 2*MENU_SIZE; col++) {
            menu_board[row][col] = rand() % 2;
        }
    }

    // -------------------------
    //          Menu
    // -------------------------

    menu:

    if (board_name_len == 0) BOARD = 0; // If mode == load but no board is loaded

    // Get user input
    menu_sel = 0;
    while (1) {
        center_y = getmaxy(stdscr) / 2;
        center_x = getmaxx(stdscr) / 2;

        border_top = center_y - MENU_SIZE;
        border_bottom = center_y;
        border_right = center_x + MENU_SIZE;
        border_left = center_x - MENU_SIZE;

        clear();
        // Print random board for aesthetics
        for (int row = 0; row < border_bottom - border_top; row++) {
            for (int col = 0; col < border_right - border_left; col++) {
                if (menu_board[row][col] == 1) attron(A_STANDOUT);
                mvaddch(row+border_top, col+border_left, ' ');
                attroff(A_STANDOUT);
            }
        }

        rectangle(border_top-1, border_left-2, border_bottom, border_right+1);

        // Print menu
        mvprintw(center_y+2, center_x-4, "Play game");
        mvprintw(center_y+3, center_x-4, "Options");
        mvaddch(center_y+2+menu_sel, center_x-7, ACS_RARROW);

        pressed_key = getch();
        switch (pressed_key) {
            case 'j':
                if (menu_sel == 0) menu_sel++;
                break;

            case 'k':
                if (menu_sel == 1) menu_sel--;
                break;

            case 'q':
                goto end_game;
                break;

            case 'l':
            case '\n':
                if (menu_sel == 0) goto start_game;     // If cursor is in start game
                goto options;                           // if cursor is in options
                break;
        }
    }

    // -------------------------
    //         Options
    // -------------------------
    options:

    option_chosen = 0;

    while (1) {
        center_y = getmaxy(stdscr) / 2;
        center_x = getmaxx(stdscr) / 2;

        // Kind of trash patch for choosing the length of the menu
        options.len = BOARD == 0 ? 5 : 4;

        // Print options
        clear();
        mvprintw(center_y-4, center_x-OPTIONS_OFFSET, "Mode:");
        mvprintw(center_y-4, center_x+OPTIONS_OFFSET, MODE == 0? "normal" : "zen");

        mvprintw(center_y-3, center_x-OPTIONS_OFFSET, "Entropy:");
        mvprintw(center_y-3, center_x+OPTIONS_OFFSET, ENTROPY == 0? "no" : "yes");

        mvprintw(center_y-2, center_x-OPTIONS_OFFSET, "Board:");
        mvprintw(center_y-2, center_x+OPTIONS_OFFSET, BOARD == 0? "random" : "saved");

        if (BOARD == 0) { // If board == random
            mvprintw(center_y-1, center_x-OPTIONS_OFFSET, "Rows:");
            mvprintw(center_y-1, center_x+OPTIONS_OFFSET, "%d", N_ROWS);

            mvprintw(center_y, center_x-OPTIONS_OFFSET, "Cols:");
            mvprintw(center_y, center_x+OPTIONS_OFFSET, "%d", N_COLS);

        } else {
            mvprintw(center_y-1, center_x-OPTIONS_OFFSET, "Load:");
            mvprintw(center_y-1, center_x+OPTIONS_OFFSET, "%s", board_name);
        }

        attron(A_UNDERLINE);
        mvprintw(center_y+4, center_x-4, "Game controls");
        attroff(A_UNDERLINE);
        mvprintw(center_y+6, center_x-16, "move (arrows, hjkl), save (w), exit q");

        mvaddch(center_y-4+options.sel, center_x-OPTIONS_OFFSET-3+2*OPTIONS_OFFSET*option_chosen, ACS_RARROW);  // Print selector

        // Get user input
        pressed_key = getch();

        if (option_chosen == 1) {
            // If selected field is "Enter file name to load"
            if (BOARD == 1 && options.sel == 3) {
                switch (pressed_key) {
                    case '\n':
                        // If file does not exist, reset to random board
                        if (load_board(&board_backup, board_name)) {
                            options.sel = 2;
                            BOARD = 0;
                            for (int i=0; i< board_name_len; i++) {
                                board_name[i] = '\0';
                            }
                            board_name_len = 0;
                        } else {
                            n_rows = board_backup[0][0];
                            n_cols = board_backup[0][1];
                            board_backup++;
                        }
                        option_chosen = 0;
                        break;

                    case KEY_BACKSPACE:
                        if (board_name_len > 0) {
                            board_name[board_name_len-1] = '\0';
                            board_name_len--;
                        }
                        break;

                    default:
                        if (board_name_len < STR_LEN-1) {
                            *(board_name+board_name_len) = pressed_key;
                            board_name_len++;
                        }
                        break;
                }
            }
            // If selected field is anything else
            else switch (pressed_key) {
                case KEY_DOWN:
                case 'j':
                    dec_sel_value(options);
                    break;

                case KEY_UP:
                case 'k':
                    inc_sel_value(options);
                    break;

                case '\n':
                case KEY_LEFT:
                case 'h':
                    option_chosen = 0;
                    break;
                case 'q':
                    goto menu;
            }
        } else {
            switch (pressed_key) {
                case KEY_DOWN:
                case 'j':
                    inc_selected(options);
                    break;

                case KEY_UP:
                case 'k':
                    dec_selected(options);
                    break;

                case KEY_RIGHT:
                case KEY_ENTER:
                case 'l':
                    option_chosen = 1;
                    break;

                case 'q':
                case 'h':
                    goto menu;
                    break;
            }
        }
    }

    // -------------------------
    //          Game
    // -------------------------
    start_game:

    // If board == random, generate random board
    if (BOARD == 0) {
        n_rows = N_ROWS;
        n_cols = N_COLS;

        board = (int**) malloc(n_rows * sizeof(int*));

        for (int row = 0; row < n_rows; row++) {
            board[row] = (int*) malloc(n_cols * sizeof(int));
            for (int col = 0; col < n_cols; col++) {
                board[row][col] = rand() % 2;
            }
        }
    } else {
        board = (int**) malloc(n_rows * sizeof(int*));

        for (int row = 0; row < n_rows; row++) {
            board[row] = (int*) malloc(n_cols * sizeof(int));
            for (int col = 0; col < n_cols; col++) {
                board[row][col] = board_backup[row][col];
            }
        }
    } // If board == load, load from board_backup,

    sel_col = 0;
    sel_row = 0;

    while (1) {
        // Print board and count white
        center_y = getmaxy(stdscr) / 2;
        center_x = getmaxx(stdscr) / 2;

        n_white = 0;
        clear();
        for (int row = 0; row < n_rows; row++) {
            for (int col = 0; col < n_cols; col++) {
                if (row == sel_row && col == sel_col) {
                    c1_to_print = ACS_HLINE;
                    c2_to_print = ACS_LRCORNER;

                } else if (col == sel_col && row < sel_row) {
                    c1_to_print = ' ';
                    c2_to_print = ACS_VLINE;

                } else if (row == sel_row && col < sel_col) {
                    c1_to_print = ACS_HLINE;
                    c2_to_print = ACS_HLINE;

                } else {
                    c1_to_print = ' ';
                    c2_to_print = ' ';
                }

                // Count number of white squares if mode == normal
                if (board[row][col] == 1) {
                    if (MODE == 0) n_white++;
                    attron(A_STANDOUT);
                }


                // Print currente square centered
                pos_y = center_y - (n_rows/2 - row);
                pos_x = center_x - 2*(n_cols/2 - col);
                mvaddch(pos_y, pos_x, c1_to_print);
                mvaddch(pos_y, pos_x+1, c2_to_print);

                attroff(A_STANDOUT);
            }
        }

        // Print borders
        border_top = center_y - n_rows/2 - 1;
        border_right = center_x - 2*(n_cols/2 - n_cols) + 1;
        border_bottom = center_y - (n_rows/2 - n_rows) + 1;
        border_left = center_x - 2*(n_cols/2) - 1;

        // Print entropy
        if (ENTROPY == 1) {
            entropy = get_entropy(board, n_rows, n_cols);
            mvprintw(border_bottom+1, center_x-5, "Entropy: %d", entropy);
        }

        rectangle(border_top, border_left-1, border_bottom-1, border_right);


        // If mode == normal, check if game is over and get user input
        if (MODE == 0 && n_white == 0) {
            goto menu;
        }

        // Get user pressed key and update board
        else {
            pressed_key = getch();
            switch (pressed_key) {
                // if key of move, move the board
                case KEY_UP:
                case 'k':
                    if (sel_row > 0) sel_row--;
                    break;

                case KEY_DOWN:
                case 'j':
                    if (sel_row < n_rows-1) sel_row++;
                    break;

                case KEY_LEFT:
                case 'h':
                    if (sel_col > 0) sel_col--;
                    break;

                case KEY_RIGHT:
                case 'l':
                    if (sel_col < n_cols-1) sel_col++;
                    break;

                // if key is enter, flip selected region
                case '\n':
                    /* mvprintw(0,0, "sel_row = %d", sel_row);
                    mvprintw(1,0, "sel_col = %d", sel_col); */
                    for (int row = 0; row <= sel_row; row++) {
                        for (int col = 0; col <= sel_col; col++) {
                            board[row][col] = (board[row][col] + 1) % 2;   // Flip the color using modulus 2 arithmetic. Report if you find a different way of doing this. Maybe bit logic xor.
                        }
                    }
                    break;

                // If key is s, save the board
                case 'w':
                    save_board(board, n_rows, n_cols);
                    mvprintw(center_y-2, center_x-3,    "        ");
                    mvprintw(center_y-1, center_x-3,    " Saved! ");
                    mvprintw(center_y, center_x-3,      "        ");
                    refresh();
                    usleep(SLEEP_TIME);
                    break;

                // if key is exit, exit
                case 'q':
                    goto menu;
                    break;
            }
        }
    }

    end_game:
    endwin();
}

// Draws a rectangle. Thanks to https://stackoverflow.com/questions/22399406/drawing-boxes-using-ncurses
void rectangle(int y1, int x1, int y2, int x2) {
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

void save_board(int** board, int n_rows, int n_cols) {
    int index;
    FILE *file_ptr;
    char *file_name;

    // Find the smallest available index for naming (for no ovewriting)
    index = 1;
    file_name = (char*) malloc(STR_LEN * sizeof(char));

    while (1) {
        sprintf(file_name, "board%d", index);
        file_ptr = fopen(file_name, "r");
        if (file_ptr == NULL) {
            break;
        }
        fclose(file_ptr);
        index++;
    }


    // Save: a file of 0s and 1s, first line containing rows and second columns
    file_ptr = fopen(file_name, "w");
    fprintf(file_ptr, "%d\n", n_rows);          // Write rows
    fprintf(file_ptr, "%d\n", n_cols);          // Write cols

    for (int row = 0; row < n_rows; row++) {
        for(int col = 0; col < n_cols; col++) {
            fprintf(file_ptr, "%d", board[row][col]);          // Write rows and cols
        }
        fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);
}

int load_board(int*** board_ptr, char* file_name) {
    FILE* file_ptr;
    int n_rows, n_cols;
    char raw_str[STR_LEN];
    int **board;

    file_ptr = fopen(file_name, "r");
    if (file_ptr == NULL) {
        return 1;
    }

    fgets(raw_str, STR_LEN, file_ptr);      // Read rows
    sscanf(raw_str, "%d", &n_rows);

    fgets(raw_str, STR_LEN, file_ptr);      // Read cols
    sscanf(raw_str, "%d", &n_cols);

    // Get board
    board = (int**) malloc((n_rows+1) * sizeof(int*));
    board[0] = (int*) malloc(2 * sizeof(int));
    board[0][0] = n_rows;
    board[0][1] = n_cols;

    for (int row = 1; row < n_rows+1; row++) {
        board[row] = (int*) malloc(n_cols * sizeof(int));
        for (int col = 0; col < n_cols; col++) {
            board[row][col] = fgetc(file_ptr) == '0' ? 0 : 1;
        }
        fgetc(file_ptr); // Read linebreak
    }

    *board_ptr = board;

    return 0;
}

// Compute the entropy, i.e., minimum number of moves (pending to prove)
// necessary for solving the game.
int get_entropy(int **board, int n_rows, int n_cols) {
    int entropy;
    int *curr_row, *curr_mask;
    int start_interval;

    entropy = 0;

    // Init curr_row
    curr_row = (int*) calloc(n_cols, sizeof(int));
    curr_mask = (int*) calloc(n_cols, sizeof(int));

    // For row backwards in board
    for (int row = n_rows-1; row >= 0; row--) {
        // For each column backwards in board
        start_interval = -1;
        for (int col = n_cols-1; col >= 0; col--) {
            // Update current square with previous moves
            curr_row[col] = (curr_mask[col] + board[row][col]) % 2;

            // Check if interval starts or ends for counting moves
            if (start_interval == -1 && curr_row[col] == 1) { // If interval starts
                entropy++;
                start_interval = col;

            } else if (start_interval != -1 && curr_row[col] == 0) { // If interval ends
                entropy++;
                for (int mask_col = col+1; mask_col <= start_interval; mask_col++) {
                    curr_mask[mask_col] = (curr_mask[mask_col] + 1) % 2;
                }
                start_interval = -1;
            }
        }

        if (start_interval != -1) {
            for (int mask_col = 0; mask_col <= start_interval; mask_col++) {
                curr_mask[mask_col] = (curr_mask[mask_col] + 1) % 2;
            }
        }
    }

    return entropy;
}
