
// NIM: the game, no introductions are needed.

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define NIM_VALUE      // Variable for conditional compiling

#define TOKEN "_"
#define SEL_TOKEN "-"
#define MAX_HEIGHT 10
#define MAX_NIMBERS 7
#define MIN_NIMBERS 3

#define N_TOKEN 4
#define N_SEP_TOKEN 2

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) < (y) ? (y) : (x))

int main() {
#ifdef NIM_VALUE
    int nim_value;
#endif
    int remaining_cols, turn;
    int term_size_x, term_size_y, bottom, left;

    int n_nimbers, curr_height;
    int* game;

    int sel_nimber, sel_height;
    int old_sel_nimber;
    wchar_t pressed_key;

    int curr_bottom, curr_left;         // Variables for printing

    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    while (1) {
    // Print init screen
    term_size_x = getmaxx(stdscr);
    term_size_y = getmaxy(stdscr);

    left = (term_size_x / 2);
    bottom = (term_size_y / 2) + (MAX_HEIGHT/2);

    clear();
    mvprintw(bottom-10,left-13, " __   _    _    __     __");
    mvprintw(bottom-9, left-13, "|  \\ | |  | |  |  \\   /  |");
    mvprintw(bottom-8, left-13, "| |\\ | |  | |  | | \\ / | |");
    mvprintw(bottom-7, left-13, "| | \\  |  | |  | | \\ / | |");
    mvprintw(bottom-6, left-13, "|_|  \\_|  |_|  |_|     |_|");
    attron(A_BLINK);
    mvprintw(bottom-3, left-5, "Press enter");
    attroff(A_BLINK);

    do {    // Wait till player press enter
        pressed_key = getch();
        if (pressed_key == 'q') goto end_game;
    } while (pressed_key != '\n');

    // Create a random array of numbers containing the heights of a game of NIM
    srand(time(NULL));
    n_nimbers = (rand() % (MAX_NIMBERS-MIN_NIMBERS)) + MIN_NIMBERS;
    game = malloc(sizeof(unsigned int) * n_nimbers);

    for (int i = 0; i < n_nimbers; i++) {
        curr_height = (rand() % (MAX_HEIGHT-1)) + 1;
        game[i] = curr_height;
    }

    remaining_cols = n_nimbers;

    // While game is not over, play alternating turns
    turn = 2;
    sel_nimber = 0;


    while (remaining_cols != 0) {
        term_size_x = getmaxx(stdscr);
        term_size_y = getmaxy(stdscr);

        left = (term_size_x / 2) - (n_nimbers*N_TOKEN/2) - (n_nimbers-1)*N_SEP_TOKEN/2;
        bottom = (term_size_y / 2) + (MAX_HEIGHT/2);

        // Readjust variables sel_nimber and sel_height after last play
        while (game[sel_nimber] == 0) {
            sel_nimber = (sel_nimber + 1) % n_nimbers;
        }
        while (sel_height > game[sel_nimber] - 1) {
            sel_height--;
        }

        turn = turn == 2? 1 : 2;            // Alternates turns. This expression can be slightly simple but in detriment of readibility

        // Print game info
        clear();
        mvprintw(bottom+2, left + (n_nimbers*N_TOKEN+(n_nimbers-1)*N_SEP_TOKEN)/2 - 4, "Player %d", turn);
#ifdef NIM_VALUE
        // Compute nim value and print it
        nim_value = 0;
        for (int i = 0; i < n_nimbers; i++) {
            nim_value ^= game[i];
        }
        mvprintw(bottom+3, left + (n_nimbers*N_TOKEN+(n_nimbers-1)*N_SEP_TOKEN)/2 - 6, "Nim value: %d", nim_value);
#endif

        // Select nimber and play
        do {
            // Print game
            for (int curr_nimber = 0; curr_nimber < n_nimbers; curr_nimber++) {
                curr_left = left + curr_nimber*(N_TOKEN+N_SEP_TOKEN);
                curr_bottom = bottom;

                for (int curr_height = 0; curr_height < game[curr_nimber]; curr_height++) {
                    curr_bottom--;
                    if (curr_nimber == sel_nimber) {
                        if (curr_height >= sel_height) {
                            for (int i = 0; i < N_TOKEN; i++) {
                                attron(A_BLINK);
                                mvprintw(curr_bottom, curr_left+i, SEL_TOKEN);
                                attroff(A_BLINK);
                            }
                        } else {
                            for (int i = 0; i < N_TOKEN; i++) {
                                attron(A_BOLD);
                                mvprintw(curr_bottom, curr_left+i, TOKEN);
                                attroff(A_BOLD);
                            }
                        }
                    } else {
                        for (int i = 0; i < N_TOKEN; i++) {
                            attron(A_BOLD);
                            mvprintw(curr_bottom, curr_left+i, TOKEN);
                            attroff(A_BOLD);
                        }
                    }
                }
            }
            refresh();

            // Get play
            pressed_key = getch();
            switch (pressed_key) {
                case KEY_LEFT:
                case 'h':
                    old_sel_nimber = sel_nimber;
                    do {
                        sel_nimber = (sel_nimber - 1 + n_nimbers) % n_nimbers;
                    } while (game[sel_nimber] == 0);
                    if (old_sel_nimber < sel_nimber) sel_nimber = old_sel_nimber;
                    sel_height = MIN(sel_height, game[sel_nimber]-1);
                    break;

                case KEY_RIGHT:
                case 'l':
                    old_sel_nimber = sel_nimber;
                    do {
                        sel_nimber = (sel_nimber + 1) % n_nimbers;
                    } while (game[sel_nimber] == 0);
                    if (old_sel_nimber > sel_nimber) sel_nimber = old_sel_nimber;
                    sel_height = MIN(sel_height, game[sel_nimber]-1);
                    break;

                case KEY_DOWN:
                case 'j':
                    sel_height = MAX(sel_height-1, 0);
                    break;

                case KEY_UP:
                case 'k':
                    sel_height = MIN(sel_height+1, game[sel_nimber]-1);
                    break;
            }
            if (pressed_key == 'q') goto end_game;
        } while (pressed_key != '\n');

        // Update game
        game[sel_nimber] = sel_height;
        if (sel_height == 0) remaining_cols--;
    }
    }
    end_game:

    endwin();
}
