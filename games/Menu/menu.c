#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>   // para caracteres especiales
#include <unistd.h>   // para sleep() / usleep()
#include "games.h"

#define COMPILE_DIR "compiled/"
#define STR_LEN 100
#define incrementar_var(x, top) (x) = (x) + 1 <= (top) ? (x) + 1 : (x)
#define decrementar_var(x, bottom) (x) = (x) - 1 >= (bottom) ? (x) - 1 : (x)

#define printopt(opt, x, string) mvprintw(center_y - NUM_GAMES/2 + (opt), center_x + (x), (string)) // Prints the given game centered in the menu

void inicioLetras() {
    int term_size_x = getmaxx(stdscr);   // para centrar
    int term_size_y = getmaxy(stdscr);   // la pantalla

    int to_right = (term_size_x - 68)/2 -1;
    int to_down = (term_size_y - 20)/2;

    mvprintw(0+to_down, 1+to_right, "   //////       //   //        //////        //////       //////    ");
    mvprintw(1+to_down, 1+to_right, "     //          // //            //         //  //       //  //    ");
    mvprintw(2+to_down, 1+to_right, "     //           //          ---//---       //////       /////     ");
    mvprintw(3+to_down, 1+to_right, "     //           //            //           //  //       //  //    ");
    mvprintw(4+to_down, 1+to_right, "     //           //           //////        //  //       //   //   ");

    mvprintw(6+to_down, 1+to_right, "  //////     //////      //    //     //////     //   //     ////// ");
    mvprintw(7+to_down, 1+to_right, "  //         //  //      ///  ///       //       ///  //     //     ");
    mvprintw(8+to_down, 1+to_right, "  // ///     //////      // // //       //       // / //     // /// ");
    mvprintw(9+to_down, 1+to_right, "  //  //     //  //      //    //       //       //  ///     //  // ");
    mvprintw(10+to_down, 1+to_right,"  //////     //  //      //    //     //////     //   //     ////// ");

    mvprintw(12+to_down, 1+to_right," //////    //////    //////    //////    //////    //////    //   //");
    mvprintw(13+to_down, 1+to_right," //          //      //  //      //        //      //  //    ///  //");
    mvprintw(14+to_down, 1+to_right," //////      //      //////      //        //      //  //    // / //");
    mvprintw(15+to_down, 1+to_right,"     //      //      //  //      //        //      //  //    //  ///");
    mvprintw(16+to_down, 1+to_right," //////      //      //  //      //      //////    //////    //   //");

    mvprintw(18+to_down, 1+2, "Powered by TYZAR Games");
    mvprintw(19+to_down, 1+2, "Pulsa cualquier tecla para continuar...");
    getch(); // Esperar a que el usuario presione una tecla antes de continuar
    clear();
}

int main() {
    setlocale(LC_ALL, "");  // caracteres especiales

    // Inicializar
    initscr();              // Inicializa ncurses
    cbreak();               // Leer caracteres sin esperar \n
    noecho();               // Los caracteres introducidos por el usuario no se muestran
    keypad(stdscr, TRUE);   // Para usar las teclas de flecha del teclado
    curs_set(0);            // Oculta el cursor

    // Definir colores
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, -1);   // Para el color rojo
    init_pair(2, COLOR_BLUE, -1);  // Para el color azul
    init_pair(3, COLOR_GREEN, -1); // Para el color verde

    // Funcionalidades
    char *variantes_permuter[] = { // para funcionalidad permuter
        "Xx_Permuter_xX", "Xx_ePrmuter_xX", "Xx_erPmuter_xX", "Xx_ermPuter_xX",
        "Xx_ermuPter_xX", "Xx_ermutPer_xX", "Xx_ermutePr_xX", "Xx_ermuterP_xX"
    };
    int paridad = 0; // para mostrar permuter cambiando

    const char *n4s_text = "Need 4 Speed"; // para funcoinalidad N4S
    int n4s_text_len = strlen(n4s_text);

    // Opciones del menú
    char *opciones[NUM_GAMES];
    for (int i = 0; i < NUM_GAMES; i++) opciones[i] = (char*) malloc( STR_LEN * sizeof(char));
    opciones[N4S]      = "Need 4 Speed";
    opciones[AHORCADO] = "Ahorcado";
    opciones[NIM]      = "NIM";
    opciones[PERMUTER] = variantes_permuter[0];
    opciones[CCP]      = "Colorea Como Puedas: Digital Edition 2024";
    opciones[MAZE]     = "MAZE";
    opciones[FLIPIT]   = "crazy flip it";
    opciones[PONG]     = "Pong";
    opciones[SALIR]    = "Salir";

    char *nombres_juegos[NUM_GAMES-1];
    for (int i = 0; i < NUM_GAMES-1; i++) nombres_juegos[i] = (char*) malloc( STR_LEN * sizeof(char));
    nombres_juegos[N4S]      = "press_the_key";
    nombres_juegos[AHORCADO] = "ahorcado";
    nombres_juegos[NIM]      = "nim";
    nombres_juegos[PERMUTER] = "permuter";
    nombres_juegos[CCP]      = "ccp";
    nombres_juegos[MAZE]     = "maze";
    nombres_juegos[FLIPIT]   = "flip";
    nombres_juegos[PONG]     = "Pong";

    char *ruta_juego = (char*) malloc(STR_LEN * sizeof(char));

    int seleccion = 0;
    int user_input;

    inicioLetras();
    int term_size_x;
    int center_x;
    int term_size_y;
    int center_y;

    // Ciclo del menú
    while (1) {
        term_size_x = getmaxx(stdscr);
        center_x    = term_size_x/2;
        term_size_y = getmaxy(stdscr);
        center_y    = term_size_y/2;

        // Limpiar pantalla y mostrar opciones
        clear();

        for (int i = 0; i < 6; i++) {
            printopt(i + NUM_GAMES/2 -3, -30, icons[seleccion][i]);
        }

        for (int i = 0; i < 9; i++) {
            if (seleccion != i) {
                //mvprintw(i + 1, 1, "%s", opciones[i]);
                printopt(i, 0, opciones[i]);
            }
        }

        attron(A_REVERSE);
        switch (seleccion) {

            // velocidad en need 4 speed
            case N4S:
                attroff(A_REVERSE);
                for (int i = 0; i < center_x - n4s_text_len; i = i + 10) {
                    //mvprintw(N4S+1, i, n4s_text);
                    printopt(N4S, i, n4s_text);
                    refresh();
                    usleep(30000);
                    //mvprintw(1, i, "             ");
                    printopt(N4S, i, "             ");
                }
                attron(A_REVERSE);
                //mvprintw(N4S+1, 1, n4s_text); // Posición final
                printopt(N4S, 0, n4s_text); // Posición final
                break;

            // ahorcado tipografía del infierno
            case AHORCADO:
                attron(COLOR_PAIR(1));
                //mvprintw(AHORCADO+1, 1, "คђ๏гςค๔๏");
                printopt(AHORCADO, 0, "คђ๏гςค๔๏");
                //mvprintw(2, 1, "ą̴̰̤̹͚̗̑͊̆̊h̵̠̟̤̱̳͙̬͇͋͗̓̚o̸͈̣̲͗̂͋̔̓̿̔́̇͘̚͝ŗ̵̛̛̠͕̩̜̦͔̦̩̻͚͒̽͋̀̾͆̊̚͘c̸̢̫̠͎̬̖͎̼̩͑̽̊̑̌͆́̋̊̎̕̕͝͠ą̸͈̪̫̰͇̻̺̈́̒̓͒́͛̎͒͛̆͜͠d̷̟̗̙̰̲̅͐̈́̓̚̚͝o̵̡̡̳̜̟̻͙͎͋͂̄̂̀̊̇̍̿͆̂̌̾̄");
                attroff(COLOR_PAIR(1));
                break;

            // NIM parpadeante
            case NIM:
                attron(A_BLINK);
                //mvprintw(NIM+1, 1, "NIM");
                printopt(NIM, 0, "NIM");
                attroff(A_BLINK);
                break;

            // Permuter permutado, secuencia de permutaciones
            case PERMUTER:
                //mvprintw(PERMUTER+1, 1, variantes_permuter[paridad]);
                printopt(PERMUTER, 0, variantes_permuter[paridad]);
                opciones[3] = variantes_permuter[paridad];
                break;

            // CCP con colorinchis
            case CCP:
                attroff(A_REVERSE);
                attron(COLOR_PAIR(1));      printopt(CCP, 0, "Colorea");
                attroff(COLOR_PAIR(1));     printopt(CCP, 7, "--");
                attron(COLOR_PAIR(2));      printopt(CCP, 9, "Como");
                attroff(COLOR_PAIR(2));     printopt(CCP, 13, "--");
                attron(COLOR_PAIR(3));      printopt(CCP, 15, "Puedas:");
                attroff(COLOR_PAIR(3));     printopt(CCP, 22, "--");
                attron(COLOR_PAIR(1));      printopt(CCP, 24, "Digital");
                attroff(COLOR_PAIR(1));     printopt(CCP, 31, "--");
                attron(COLOR_PAIR(2));      printopt(CCP, 33, "Edition");
                attroff(COLOR_PAIR(2));     printopt(CCP, 40, "--");
                attron(COLOR_PAIR(1));      printopt(CCP, 42, "2024");
                attroff(COLOR_PAIR(1));
                break;

            // MAZE mazed
            case MAZE:
                //mvprintw(MAZE+1, 1, "|\\/| A Z E ");
                printopt(MAZE, 0, "|\\/| A Z E ");
                break;

            // volver loca la opción crazy flip it
            case FLIPIT:
                //mvprintw(FLIPIT+1, 0, " ʇı dılɟ ʎzɐɹɔ");
                printopt(FLIPIT, 0, "ʇı dılɟ ʎzɐɹɔ");
                break;

            case PONG:
                printopt(PONG, 0, "P● ng");
                break;

            case SALIR:
                //mvprintw(SALIR+1, 1, "Salir");
                printopt(SALIR, 0, "Salir");
                break;
        }

        attroff(A_REVERSE);
        refresh(); // Actualizar la pantalla

        user_input = getch(); // Capturar entrada del usuario
        switch (user_input) {
            case 'k':
            case KEY_UP:
                decrementar_var(seleccion, 0);
                if (seleccion == PERMUTER) paridad = (paridad+1) % 8; // para permuter
                break;

            case 'j':
            case KEY_DOWN:
                incrementar_var(seleccion, SALIR);
                if (seleccion == PERMUTER) paridad = (paridad+1) % 8; // para permuter
                break;

            case 'q':
                goto fin_partida;
                break;

            case 'l':
            case '\n':
                // Si la opción seleccionada es salir
                if (seleccion == SALIR) {
                    goto fin_partida;
                }

                // Si la opción seleccionada es un juego
                strcpy(ruta_juego, COMPILE_DIR);
                strcat(ruta_juego, nombres_juegos[seleccion]);
                endwin();

                system(ruta_juego);

                clear();
                refresh();
                //mvprintw(center_y, center_x, "Presiona cualquier tecla para continuar...");
                printopt(0, -21, "Presiona cualquier tecla para continuar...");
                getch(); // Esperar a que el usuario presione una tecla antes de volver al menú
                break;
        }
        // TODO: Vacíar el buffer de entrada del teclado
    }

    // Cerrar ncurses
    fin_partida:
    endwin();

    return 0;
}
