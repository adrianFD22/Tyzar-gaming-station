#include <ncurses.h>
#include <locale.h>   // para caracteres especiales

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
    setlocale(LC_ALL, ""); // caracteres especiales

    // Inicializar
    initscr();  // Inicializa ncurses
    cbreak();   // Leer caracteres sin esperar \n
    noecho();   // Los caracteres introducidos por el usuario no se muestran
    keypad(stdscr, TRUE);  // Para usar las teclas de flecha del teclado

    // Definir colores
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, -1); // Para el color rojo

    // Opciones del menú
    char *opciones[] = { // Arreglo de punteros
        "Need 4 Speed",
        "Ahorcado", // Opción 2 en color rojo
        "NIM",
        "Xx_permuter_xX",
        "Colorea Como Puedas: Digital Edition 2024",
        "MAZE",
        "crazy flip it",
        "Salir"
    };
    int seleccion = 0;
    int opcion;

    inicioLetras();
    int term_size_x = getmaxx(stdscr);  
    int to_right2 = (term_size_x - 42)/2;
    int term_size_y = getmaxy(stdscr);  
    int to_down2 = (term_size_y - 1)/2;

    // Ciclo del menú
    while (1) {
        // Limpiar pantalla y mostrar opciones
        clear();
        for (int i = 0; i < 8; i++) {
            if (seleccion == i) {
                attron(A_REVERSE);  // Inversión de color al pasar por una opción
            }
            mvprintw(i + 1, 1, "%s", opciones[i]);

            // volver loca la opción crazy flip it
            if (seleccion == 6) {
                attron(A_REVERSE);
                mvprintw(7, 0, " ʇı dılɟ ʎzɐɹɔ");
            }
            // ahorcado tipografía del infierno
            if (seleccion == 1) {
                attron(A_REVERSE);
                attron(COLOR_PAIR(1));
                mvprintw(2, 1, "คђ๏гςค๔๏");
                //mvprintw(2, 1, "ą̴̰̤̹͚̗̑͊̆̊h̵̠̟̤̱̳͙̬͇͋͗̓̚o̸͈̣̲͗̂͋̔̓̿̔́̇͘̚͝ŗ̵̛̛̠͕̩̜̦͔̦̩̻͚͒̽͋̀̾͆̊̚͘c̸̢̫̠͎̬̖͎̼̩͑̽̊̑̌͆́̋̊̎̕̕͝͠ą̸͈̪̫̰͇̻̺̈́̒̓͒́͛̎͒͛̆͜͠d̷̟̗̙̰̲̅͐̈́̓̚̚͝o̵̡̡̳̜̟̻͙͎͋͂̄̂̀̊̇̍̿͆̂̌̾̄");
                attroff(COLOR_PAIR(1));
            }
            attroff(A_REVERSE);  // Quita la inversión de color
        }
        refresh(); // Actualizar la pantalla
        
        opcion = getch(); // Capturar entrada del usuario
        switch (opcion) {
            case 'k':
            case KEY_UP:
                seleccion--;
                if (seleccion < 0) {
                    seleccion = 7;
                }
                break;
            case 'j':
            case KEY_DOWN:
                seleccion++;
                if (seleccion > 7) {
                    seleccion = 0;
                }
                break;
            case 'h':
            case 'l':
            case 10: // Tecla ENTER
                if (seleccion == 7) {
                    // Salir del ciclo si se selecciona "Salir"
                    endwin();
                    return 0;
                } else if (seleccion == 2) {
                    // Ejecutar NIM
                    clear();
                    refresh();
                    endwin();
                    system("./nim");
                    mvprintw(to_down2, to_right2, "Presiona cualquier tecla para continuar...");
                    getch(); // Esperar a que el usuario presione una tecla antes de volver al menú
                } else if (seleccion == 1) {
                    // Ejecutar ahorcado 2
                    clear();
                    refresh();
                    endwin();
                    system("./ahorcado");
                    mvprintw(to_down2, to_right2, "Presiona cualquier tecla para continuar...");
                    getch(); // Esperar a que el usuario presione una tecla antes de volver al menú
                } else if (seleccion == 0) {
                    // Ejecutar Need 4 Speed
                    clear();
                    refresh();
                    endwin();
                    system("./presskey");
                    mvprintw(to_down2, to_right2, "Presiona cualquier tecla para continuar...");
                    getch(); // Esperar a que el usuario presione una tecla antes de volver al menú
                }
                else if (seleccion == 3) {
                    // Ejecutar permuter
                    clear();
                    refresh();
                    endwin();
                    system("./ordenar");
                    mvprintw(to_down2, to_right2, "Presiona cualquier tecla para continuar...");
                    getch(); // Esperar a que el usuario presione una tecla antes de volver al menú
                }
                else if (seleccion == 4) {
                    // Ejecutar Colorea Como PUedas
                    clear();
                    refresh();
                    endwin();
                    system("./ccp");
                    mvprintw(to_down2, to_right2, "Presiona cualquier tecla para continuar...");
                    getch(); // Esperar a que el usuario presione una tecla antes de volver al menú
                }
                else if (seleccion == 5) {
                    // Ejecutar MAZE
                    clear();
                    refresh();
                    endwin();
                    system("./maze");
                    mvprintw(to_down2, to_right2, "Presiona cualquier tecla para continuar...");
                    getch(); // Esperar a que el usuario presione una tecla antes de volver al menú
                }
                else if (seleccion == 6) {
                    // Ejecutar Flip it
                    clear();
                    refresh();
                    endwin();
                    system("./flipit");
                    mvprintw(to_down2, to_right2, "Presiona cualquier tecla para continuar...");
                    getch(); // Esperar a que el usuario presione una tecla antes de volver al menú
                }
                break;
        }
    }

    // Cerrar ncurses
    endwin();

    return 0;
}
