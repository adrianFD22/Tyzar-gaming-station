
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include "letters.h"

// idea: un juego de reflejos. El programa dibuja una letra y el jugador tiene que presionarla corriendo. Si no lo hace lo suficientemente rápido, pierde. Puede llevar la cuenta de cuántos puntos hace.
// También se puede hacer que se escriba en gigante

#define UPPER(c) ((c) >= 97 ? (c) - 32 : (c))
#define MAX_DIFFICULTY 5

char print_letter(char letter, int pos_x, int pos_y);

int main() {
    int term_size_x, term_size_y, center_x, center_y;

    char random_letter, user_input;
    int difficulty, pending_flag;
    int wait_time, score;

    srand(time(NULL));
    difficulty = 1;

    // Start ncurses mode
    initscr();
    curs_set(0);            // Hides the cursor
    keypad(stdscr, TRUE);   // Enable arrow keys
    noecho();               // Do not print pressed keys by the user

    // Get terminal size
    term_size_x = getmaxx(stdscr);
    term_size_y = getmaxy(stdscr);

    // Adjust for centering text
    center_x = term_size_x / 2;
    center_y = term_size_y / 2;

    // Menu
    // Print instructions
    mvprintw(center_y-2, center_x-3, "Welcome!");

    mvprintw(center_y, center_x-12, "Menu keys: j, k, q, Enter");
    mvprintw(center_y+1, center_x-5, "Press enter");

    pending_flag = 1;
    while (pending_flag) {
        user_input = getch();
        switch (user_input) {
            case '\n':
                pending_flag = 0;
                break;
            case 'q':
                goto endgame;
                break;
        }
    }

    // Loop between game and select difficulty
    while (1) {
        // Get terminal size
        term_size_x = getmaxx(stdscr);
        term_size_y = getmaxy(stdscr);

        // Adjust for centering text
        center_x = term_size_x / 2;
        center_y = term_size_y / 2;

        // Select difficulty
        pending_flag = 1;
        while (pending_flag) {
            clear();
            mvprintw(center_y, center_x-6, "Difficulty: %d", difficulty);
            user_input = getch();

            switch (user_input) {
                case 'k':
                    if (difficulty < MAX_DIFFICULTY) {
                        difficulty++;
                    }
                    break;
                case 'j':
                    if (difficulty > 0) {
                        difficulty--;
                    }
                    break;
                case '\n':
                    pending_flag = 0;
                    break;
                case 'q':
                    goto endgame;
            }
        }

        if (difficulty == 0) {
            wait_time = 0;
        } else {
            wait_time = (MAX_DIFFICULTY - difficulty + 1)*5;
        }

        // Dont know if to add a countdown (its the final countdown!)

        // ----------------- The actual game -----------------
        score = -1;
        do {
            clear();                                                // Clear the screen

            // Get terminal size
            term_size_x = getmaxx(stdscr);
            term_size_y = getmaxy(stdscr);

            // Adjust for centering text
            center_x = term_size_x / 2;
            center_y = term_size_y / 2;

            score++;
            random_letter = (rand() % 25) + 65;
            print_letter(random_letter, center_x-2, center_y-2);    // Print random letter
            halfdelay(wait_time);                                   // Wait for user input
            user_input = getch();                                   // Get user input
        } while( UPPER(user_input) == random_letter );

        // Print score
        clear();                                                    // Clear the screen
        mvprintw(center_y, center_x-5, "Score: %d\n", score);
        mvprintw(center_y + 1, center_x-5, "Press Enter");

        pending_flag = 1;
        while (pending_flag) {
            user_input = getch();
            switch (user_input) {
                case '\n':
                    pending_flag = 0;
                    break;
                case 'q':
                    goto endgame;
                    break;
            }
    }
    }

    // End ncurses mode
    endgame:
    endwin();
}

char print_letter(char letter, int pos_x, int pos_y) {
    int letter_index = UPPER(letter) - 65;

    for (int line = 0; line < 5; line++) {
        mvprintw(pos_y + line, pos_x, "%s\n", letters[letter_index][line]);
    }

    return letter + 64;
};
