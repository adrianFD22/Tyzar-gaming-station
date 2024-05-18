#include <ncurses.h>
#include <unistd.h> // para usleep()
#include <locale.h> // para la estrella y caracteres especiales
#include <string.h> // para memset()
#include <stdlib.h> // para rand() y srand()
#include <time.h>   // para time()

#define PADDLE_WIDTH 1
#define BALL "●"    // Pelota
#define PADDLE "█"  // Pala

typedef struct {
    int x, y;
    int width, height;
    const char *display;
} Paddle;

typedef struct {
    int x, y;
    int dir_x, dir_y;
    const char *display;
    int delay;
} Ball;

void init_paddle(Paddle *paddle, int x, int y, int height) {
    paddle->x = x;
    paddle->y = y;
    paddle->width = PADDLE_WIDTH;
    paddle->height = height;
    paddle->display = PADDLE;
}

void init_ball(Ball *ball, int x, int y, int delay) {
    ball->x = x;
    ball->y = y;
    ball->dir_x = (rand() % 2) == 0 ? 1 : -1;  // dirección aleatoria en x
    ball->dir_y = (rand() % 2) == 0 ? 1 : -1;  // dirección aleatoria en y
    ball->display = BALL;
    ball->delay = delay;
}

void draw_paddle(Paddle *paddle) {
    for (int i = 0; i < paddle->height; i++) {
        mvaddstr(paddle->y + i, paddle->x, paddle->display);
    }
}

void draw_ball(Ball *ball) {
    mvaddstr(ball->y, ball->x, ball->display);
}

void move_paddle(Paddle *paddle, int dir, int up_key, int down_key) {
    if (dir == up_key && paddle->y > 4) { // No permitir que la pala suba por encima del techo
        paddle->y--;
    } else if (dir == down_key && paddle->y < LINES - paddle->height) {
        paddle->y++;
    }
}

int move_ball(Ball *ball) {
    ball->x += ball->dir_x;
    ball->y += ball->dir_y;

    // Rebota tres filas antes de la parte superior (techo)
    if (ball->y <= 3 || ball->y >= LINES - 1) {
        ball->dir_y *= -1;
    }

    if (ball->x <= 0) {
        return 1; // Punto para el jugador 2
    } else if (ball->x >= COLS - 1) {
        return 2; // Punto para el jugador 1
    }

    return 0; // No hay puntos
}

int check_collision(Ball *ball, Paddle *paddle) {
    // Verificar si la pelota está dentro del rango vertical de la paleta
    if (ball->y >= paddle->y && ball->y < paddle->y + paddle->height) {
        // Verificar colisión con la paleta izquierda
        if (ball->dir_x < 0 && ball->x == paddle->x + paddle->width) {
            ball->dir_x *= -1;
            ball->x = paddle->x + paddle->width; // Corregir la posición de la pelota
            return 1;
        }
        // Verificar colisión con la paleta derecha
        if (ball->dir_x > 0 && ball->x == paddle->x - 1) {
            ball->dir_x *= -1;
            ball->x = paddle->x - 1; // Corregir la posición de la pelota
            return 1;
        }
    }
    return 0;
}

// Función que dibuja el marcador
void draw_score(int score1, int score2) {
    mvprintw(0, COLS / 2 - 7, "___________");
    mvprintw(2, COLS / 2 - 20, "J1: w/s      █ %d  VS %d █      J2: ↑/↓", score1, score2);
    char techo[COLS + 1];       // +1 para el carácter nulo de final de cadena
    memset(techo, '_', COLS);   // Para llenar la fila de '_'
    techo[COLS] = '\0';         // Agrega el carácter nulo al final de la cadena
    mvprintw(3, 0, "%s", techo);
    mvprintw(3, COLS / 2 - 7, "█_________█");
    if (score1 > score2) {
        mvprintw(1, COLS / 2 - 7, "█ ★       █");
    } else if (score2 > score1) {
        mvprintw(1, COLS / 2 - 7, "█       ★ █");
    } else {
        mvprintw(1, COLS / 2 - 7, "█         █");
    }
}

// Selector de dificultad antes de empezar, afecta a la velocidad y el tamaño de las palas
void select_difficulty(int *paddle_height, int *ball_delay) {
    char *choices[] = {"1. Fácil", "2. Normal", "3. Difícil"};
    int choice, highlight = 0;

    while (1) {
        erase();
        mvprintw(LINES / 2 - 2, COLS / 2 - 10, "Selecciona la dificultad:");
        for (int i = 0; i < 3; ++i) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(LINES / 2 + i, COLS / 2 - 10, "%s", choices[i]);
            attroff(A_REVERSE);
        }
        choice = getch();

        switch (choice) {
            case KEY_UP:
            case 'h':
                highlight = (highlight == 0) ? 2 : highlight - 1;
                break;
            case KEY_DOWN:
            case 'l':
                highlight = (highlight == 2) ? 0 : highlight + 1;
                break;
            case 10: // Enter
            case 'j':
            case 'k':
                if (highlight == 0) {
                    *paddle_height = 5;
                    *ball_delay = 60000;
                } else if (highlight == 1) {
                    *paddle_height = 4;
                    *ball_delay = 40000;
                } else if (highlight == 2) {
                    *paddle_height = 3;
                    *ball_delay = 20000;
                }
                return;
        }
    }
}

int main() {
    Paddle paddle1, paddle2;
    Ball ball;
    int ch, score1 = 0, score2 = 0, point;
    int paddle_height, ball_delay;

    setlocale(LC_ALL, "");  // caracteres especiales (estrella y círculo)
    initscr();              // ncurses
    noecho();               // no ver las entradas de teclado del usuario
    curs_set(FALSE);        // cursor invisible
    keypad(stdscr, TRUE);   // activar las flechas del teclado
    timeout(0);
    srand(time(NULL));      // inicializar números aleatorios

    select_difficulty(&paddle_height, &ball_delay);

    init_paddle(&paddle1, 2, LINES / 2 - paddle_height / 2, paddle_height);
    init_paddle(&paddle2, COLS - 3, LINES / 2 - paddle_height / 2, paddle_height);
    init_ball(&ball, COLS / 2, LINES / 2, ball_delay);

    while ((ch = getch()) != 'q') {  // pulsar 'q' para salir del juego en cualquier momento
        clear();
        
        move_paddle(&paddle1, ch, 'w', 's');
        move_paddle(&paddle2, ch, KEY_UP, KEY_DOWN);
        
        point = move_ball(&ball);
        if (point == 1) {
            score2++;
            mvprintw(LINES / 2, COLS / 2 - 10, "Punto para el Jugador 2!");
            refresh();
            getchar(); // Pausa y espera a que el jugador presione una tecla
            init_ball(&ball, COLS / 2, (rand() % (LINES - 4)) + 3, ball_delay); // Reposiciona la pelota
        } else if (point == 2) {
            score1++;
            mvprintw(LINES / 2, COLS / 2 - 10, "Punto para el Jugador 1!");
            refresh();
            getchar(); // Pausa y espera a que el jugador presione una tecla
            init_ball(&ball, COLS / 2, LINES / 2, ball_delay); // Reposiciona la pelota
        }
        
        check_collision(&ball, &paddle1);
        check_collision(&ball, &paddle2);

        draw_paddle(&paddle1);
        draw_paddle(&paddle2);
        draw_ball(&ball);
        draw_score(score1, score2);

        refresh();
        usleep(ball.delay);
    }

    endwin();
    return 0;
}