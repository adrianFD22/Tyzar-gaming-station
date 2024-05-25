#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>   // para caracteres especiales
#include <unistd.h>   // para sleep() / usleep()


#define COMPILE_DIR "compiled/"
#define STR_LEN 100
#define incrementar_var(x, top) (x) = (x) + 1 <= (top) ? (x) + 1 : (x)
#define decrementar_var(x, bottom) (x) = (x) - 1 >= (bottom) ? (x) - 1 : (x)

enum game {
    N4S,
    AHORCADO,
    NIM,
    PERMUTER,
    CCP,
    MAZE,
    FLIPIT,
    SALIR
};

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

    // Opciones del menú
    char *opciones[] = {
        "Need 4 Speed",
        "Ahorcado",
        "NIM",
        "Xx_Permuter_xX",
        "Colorea Como Puedas: Digital Edition 2024",
        "MAZE",
        "crazy flip it",
        "Salir"
    };

    char *nombres_juegos[] = {
        "press_the_key",
        "ahorcado",
        "nim",
        "permuter",
        "ccp",
        "maze",
        "flip"
    };
    int opciones_len = sizeof(opciones) / sizeof(opciones[0]);

    char *ruta_juego = (char*) malloc(STR_LEN * sizeof(char));

    int seleccion = 0;
    int opcion;
    int paridad = 0; // para mostrar permuter cambiando

    inicioLetras();
    int term_size_x = getmaxx(stdscr);
    int to_right2 = (term_size_x - 42)/2;
    int term_size_y = getmaxy(stdscr);
    int to_down2 = (term_size_y - 1)/2;

    char *variantes_permuter[] = { // para funcionalidad permuter
        "Xx_Permuter_xX", "Xx_ePrmuter_xX", "Xx_erPmuter_xX", "Xx_ermPuter_xX",
        "Xx_ermuPter_xX", "Xx_ermutPer_xX", "Xx_ermutePr_xX", "Xx_ermuterP_xX"
    };

    const char *n4s_text = "Need 4 Speed"; // para funcoinalidad N4S
    int text_len = strlen(n4s_text);

    // Ciclo del menú
    while (1) {
        // Limpiar pantalla y mostrar opciones
        clear();
        for (int i = 0; i < 8; i++) {
            if (seleccion != i) {
                mvprintw(i + 1, 1, "%s", opciones[i]);
            }
        }

        attron(A_REVERSE);
        switch (seleccion) {

            // velocidad en need 4 speed
            case N4S:
                attroff(A_REVERSE);
                for (int i = 0; i < COLS - text_len; i = i + 10) {
                    mvprintw(N4S+1, i, n4s_text);
                    refresh();
                    usleep(5000);
                    mvprintw(1, i, "             ");
                }
                attron(A_REVERSE);
                mvprintw(N4S+1, 1, n4s_text); // Posición final
                break;

            // ahorcado tipografía del infierno
            case AHORCADO:
                attron(COLOR_PAIR(1));
                mvprintw(AHORCADO+1, 1, "คђ๏гςค๔๏");
                //mvprintw(2, 1, "ą̴̰̤̹͚̗̑͊̆̊h̵̠̟̤̱̳͙̬͇͋͗̓̚o̸͈̣̲͗̂͋̔̓̿̔́̇͘̚͝ŗ̵̛̛̠͕̩̜̦͔̦̩̻͚͒̽͋̀̾͆̊̚͘c̸̢̫̠͎̬̖͎̼̩͑̽̊̑̌͆́̋̊̎̕̕͝͠ą̸͈̪̫̰͇̻̺̈́̒̓͒́͛̎͒͛̆͜͠d̷̟̗̙̰̲̅͐̈́̓̚̚͝o̵̡̡̳̜̟̻͙͎͋͂̄̂̀̊̇̍̿͆̂̌̾̄");
                attroff(COLOR_PAIR(1));
                break;

            // NIM parpadeante
            case NIM:
                attron(A_BLINK);
                mvprintw(NIM+1, 1, "NIM");
                attroff(A_BLINK);
                break;

            // Permutter permutado, secuencia de permutaciones
            case PERMUTER:
                mvprintw(PERMUTER+1, 1, variantes_permuter[paridad]);
                opciones[3] = variantes_permuter[paridad];
                break;

            // CCP con colorinchis
            case CCP:
                attroff(A_REVERSE);
                attron(COLOR_PAIR(1));      mvprintw(5, 1, "Colorea");
                attroff(COLOR_PAIR(1));     mvprintw(5, 8, "--");
                attron(COLOR_PAIR(2));      mvprintw(5, 10, "Como");
                attroff(COLOR_PAIR(2));     mvprintw(5, 14, "--");
                attron(COLOR_PAIR(3));      mvprintw(5, 16, "Puedas:");
                attroff(COLOR_PAIR(3));     mvprintw(5, 23, "--");
                attron(COLOR_PAIR(1));      mvprintw(5, 25, "Digital");
                attroff(COLOR_PAIR(1));     mvprintw(5, 32, "--");
                attron(COLOR_PAIR(2));      mvprintw(5, 34, "Edition");
                attroff(COLOR_PAIR(2));     mvprintw(5, 41, "--");
                attron(COLOR_PAIR(1));      mvprintw(5, 43, "2024");
                attroff(COLOR_PAIR(1));
                break;

            // MAZE mazed
            case MAZE:
                mvprintw(MAZE+1, 1, "MAZE");
                break;
                //attron(A_REVERSE);
                /* mvprintw(3, 1, "NIM                _______________");
                mvprintw(4, 1, "Xx_Permuter_xX     | ↓ ← ← ← ← ← |");
                mvprintw(5, 1, "Colorea Como Puedas: ↓ Digital ↑ Edition 2024");
                mvprintw(6, 1, "MAZE     ← ← ← ← ← | ↓ |     | ↑ ←  #");
                mvprintw(7, 1, "crazy flip it ˉ| ↑ ← ← |     ˉˉˉˉˉˉˉˉˉ");
                mvprintw(8, 1, "Salir          ˉˉˉˉˉˉˉˉˉ");
                attron(A_REVERSE);
                mvprintw(6, 1, "MAZE"); */

            // volver loca la opción crazy flip it
            case FLIPIT:
                mvprintw(FLIPIT+1, 0, " ʇı dılɟ ʎzɐɹɔ");
                break;

            case SALIR:
                mvprintw(SALIR+1, 1, "Salir");
                break;
        }

        attroff(A_REVERSE);
        refresh(); // Actualizar la pantalla

        opcion = getch(); // Capturar entrada del usuario
        switch (opcion) {
            case 'k':
            case KEY_UP:
                decrementar_var(seleccion, 0);
                if (seleccion == PERMUTER) paridad = (paridad+1) % 8; // para permuter
                break;

            case 'j':
            case KEY_DOWN:
                incrementar_var(seleccion, opciones_len-1);
                if (seleccion == PERMUTER) paridad = (paridad+1) % 8; // para permuter
                break;

            case 'q':
                goto fin_partida;
                break;

            case 'l':
            case '\n':
                // Si la opción seleccionada es salir
                if (seleccion == opciones_len-1) {
                    goto fin_partida;
                }

                // Si la opción seleccionada es un juego
                strcpy(ruta_juego, COMPILE_DIR);
                strcat(ruta_juego, nombres_juegos[seleccion]);
                endwin();

                system(ruta_juego);

                clear();
                refresh();
                mvprintw(to_down2, to_right2, "Presiona cualquier tecla para continuar...");
                getch(); // Esperar a que el usuario presione una tecla antes de volver al menú
                break;
        }
    }

    // Cerrar ncurses
    fin_partida:
    endwin();

    return 0;
}
