/*
Notas de versión de Colorea Como Puedas
versión 2: ncurses
versión 3: modo multijugador
versión 4: ventana instrucciones + teclado hjk
versión 5: modificaciones teclado hl k + condiciones de victoria + inicio letras gigantes
           solucionado bug número de turnos
           solucionado bug volver del manual
           solucionado bug cerrar el path (no está muy fino aún, primera pantalla falla)
versión 6: Interfaz centrada + colores (ahora sí es colorea como puedas)
*/

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LONGITUD 100 // arbitrariamente grande

// Función para determinar vía usuario el tamaño del path
int selec() {
    char cadena[MAX_LONGITUD];
    int nodos;

    int term_size_x = getmaxx(stdscr);   // para centrar
    int term_size_y = getmaxy(stdscr);   // la pantalla
    int to_right = (term_size_x - 36)/2;
    int to_down = (term_size_y)/2;

    mvprintw(to_down-3, to_right, " ¡Bienvenido a Colorea Como Puedas!");
    mvprintw(to_down-2, to_right-11," Selecciona el número de nodos que tendrá el path: (2-10)\n");

    int y, x;
    getyx(stdscr, y, x);
    int centerX = term_size_x/2 - 1;
    move(y+1, centerX);  // para centrar el cursor

    getnstr(cadena, sizeof(cadena)); // para aceptar el 10 (dos cifras)
    nodos = atoi(cadena); // convierte a tipo int

    if (nodos < 2 || nodos > 10) { // arbitrariamente
        clear();
        mvprintw(to_down-3, to_right-2, "No se ha seleccionado un número válido.\n");
        mvprintw(to_down-2, to_right-11, "         Pulsa cualquier tecla para continuar...         ");
        move(y+1, centerX);  // para centrar el cursor
        getch();
        clear();
        return selec();
    }

    return nodos;
}

// Función para mostrar el path en pantalla, cada nodo tiene estructura "0--" para simular un grafo,
// por lo que nos moveremos de tres en tres caracteres a través de él
void mostrarPath(char array[], int longitud, int posUsuario) {      // array no es un nombre muy descriptivo
    clear();

    int term_size_x = getmaxx(stdscr);   // para centrar
    int term_size_y = getmaxy(stdscr);   // la pantalla
    int to_right = (term_size_x - longitud*3)/2 + 1;
    int to_down = (term_size_y - 8)/2;

    start_color();                              // estas cosas hay que inicializarlas cada vez? no se pueden poner en el main?
    use_default_colors();
    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_BLUE, -1);
    init_pair(3, COLOR_GREEN, -1);

    for (int i = 0; i < longitud * 3; i += 3) {
        if (i / 3 == posUsuario) {
            attron(A_REVERSE); // Invertimos los colores para resaltar la posición del usuario
            mvprintw(0+to_down, i+to_right, "%c", array[i]);
            attroff(A_REVERSE); // Restauramos los colores originales después de imprimir el número
        } else {
            mvprintw(0+to_down, i+to_right, "%c ", array[i]);
        }

        if ( i / 3 != longitud-1 ) {
            mvprintw(0+to_down, i+1+to_right, "%c%c ", array[i+1], array[i+2]);
        }

        if (array[i] == '1') {
            attron(COLOR_PAIR(1));
            if (i / 3 == posUsuario) {
                attron(A_REVERSE);
                mvprintw(0+to_down, i+to_right, "%c", array[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(0+to_down, i+to_right, "%c", array[i]);
            }
            attroff(COLOR_PAIR(1));
        }
        if (array[i] == '2') {
            attron(COLOR_PAIR(2));
            if (i / 3 == posUsuario) {
                attron(A_REVERSE);
                mvprintw(0+to_down, i+to_right, "%c", array[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(0+to_down, i+to_right, "%c", array[i]);
            }
            attroff(COLOR_PAIR(2));
        }
        if (array[i] == '3') {
            attron(COLOR_PAIR(3));
            mvprintw(0+to_down, i+to_right, "%c", array[i]);
            attroff(COLOR_PAIR(3));
        }
    }
    /* array[longitud*3-1] = '\0';   // cerrar el path visualmente, elimina
    array[longitud*3-2] = '\0';   // los dos últimos '--' */

    /* printw("\n\n"); */
    refresh();
}

// Manual de
void instrucciones() {
    int term_size_x = getmaxx(stdscr);   // Tamaño de la pantalla
    int term_size_y = getmaxy(stdscr); 

    // Crear una nueva ventana centrada
    WINDOW *manual_win = newwin(term_size_y, term_size_x, 0, 0);
    box(manual_win, 0, 0);  // Añadir un borde a la ventana
    wrefresh(manual_win);

    mvwprintw(manual_win, 1, 1, "Manual de instrucciones de Colorea Como Puedas: Digital Edition (2024)");
    mvwprintw(manual_win, 2, 1, "-------------------------------------------------------------------");
    mvwprintw(manual_win, 3, 1, "Movimiento:");
    mvwprintw(manual_win, 4, 1, "Derecha: 'l' o flecha derecha en el teclado");
    mvwprintw(manual_win, 5, 1, "Izquierda: 'h' o flecha izda en el teclado");
    mvwprintw(manual_win, 7, 1, "Tecla de acción: Pulsar 'k' o Enter en el teclado");
    mvwprintw(manual_win, 9, 1, "-------------------------------------------------------------------");
    mvwprintw(manual_win, 10, 1, "El objetivo de Colorea Como puedas es asignar un número a cada nodo");
    mvwprintw(manual_win, 11, 1, "del grafo con la tecla de acción, desplazándonos por el mismo con");
    mvwprintw(manual_win, 12, 1, "las teclas de movimiento. El número asignado al nodo será siempre");
    mvwprintw(manual_win, 13, 1, "el mínimo posible distinto del de sus vecinos.");
    mvwprintw(manual_win, 14, 1, "Un nodo vacío tiene número '0'");
    mvwprintw(manual_win, 15, 1, "Pierde el jugador que asigne el número 3 a un nodo o el que asigna");
    mvwprintw(manual_win, 16, 1, "el número del último nodo disponible.");

    mvwprintw(manual_win, 18, 1, "Pulsa cualquier tecla para volver a la partida...");
    wrefresh(manual_win);
    getch();

    // Eliminar la ventana de instrucciones y volver a la pantalla de juego
    delwin(manual_win);
    clear();
    refresh();
}

// Inicio extravagante y apoteósico
void inicio() {
    int term_size_x = getmaxx(stdscr);   // para centrar
    int term_size_y = getmaxy(stdscr);   // la pantalla

    int to_right = (term_size_x - 65)/2;
    int to_down = (term_size_y - 19)/2;

    mvprintw(0+to_down, 1+to_right, "//////    //////    //        //////    /////     /////      //");
    mvprintw(1+to_down, 1+to_right, "//        //  //    //        //  //    //  //    //        // //");
    mvprintw(2+to_down, 1+to_right, "//        //  //    //        //  //    ////      /////    //////");
    mvprintw(3+to_down, 1+to_right, "//        //  //    //        //  //    //  /     //       //   //");
    mvprintw(4+to_down, 1+to_right, "//////    //////    //////    //////    //  //    /////    //   //");

    mvprintw(6+to_down, 1+to_right, "           //////    //////    //  //    //////");
    mvprintw(7+to_down, 1+to_right, "           //        //  //    / // /    //  //");
    mvprintw(8+to_down, 1+to_right, "           //        //  //    /    /    //  //");
    mvprintw(9+to_down, 1+to_right, "           //////    //////    /    /    //////");

    mvprintw(11+to_down, 1+to_right, " //////     //  //     /////      /////        //        //////");
    mvprintw(12+to_down, 1+to_right, " //  //     //  //     //         //   /      // //      //");
    mvprintw(13+to_down, 1+to_right, " //////     //  //     /////      //   /     //////      //////");
    mvprintw(14+to_down, 1+to_right, " //         //  //     //         //   /     //   //         //");
    mvprintw(15+to_down, 1+to_right, " //         //////     /////      /////      //   //     //////");

    mvprintw(17+to_down, 1, "Digital Edition (2024)");
    mvprintw(19+to_down, 1, "Pulsa cualquier tecla para continuar...");
    getch();
    clear();
}

int main() {
    initscr();

    inicio();
    int longitud = selec();

    // Construir el path completo
    char path[MAX_LONGITUD] = "0";                 // primer nodo
    for (int i = 0; i < (longitud - 1) * 3; i++) { // resto de nodos
        strcat(path, "--0");
    }

    keypad(stdscr, TRUE);
    noecho();    // Desactivamos el eco de las teclas
    curs_set(0); // Escondemos el cursor

    int posUsuario = 0; // Inicializamos la posición del usuario al principio del path
    clear();            // Limpiamos la pantalla antes de mostrar el path

    int term_size_y = getmaxy(stdscr); // para centrar la pantalla
    int to_down = (term_size_y - 8) / 2;
    int term_size_x = getmaxx(stdscr);
    int to_right = (term_size_x - 20) / 2;

    int turno = 0;               // para pasar el turno
    int comprobacion = longitud; // para condición de victoria
    int ch;                      // para introducir las teclas
    while (comprobacion > 0) {
        mostrarPath(path, longitud, posUsuario);
        mvprintw(3 + to_down, 0 + to_right, "Turno del jugador: %d", turno % 2);
        mvprintw(6 + to_down, 1, "Pulsa 'i' para abrir el manual de instrucciones.");
        mvprintw(7 + to_down, 1, "Pulsa 'q' para salir del juego.");
        ch = getch();
        switch (ch) {
            case 'h':
            case KEY_LEFT:
                if (posUsuario > 0) {
                    posUsuario--;
                }
                break;
            case 'l':
            case KEY_RIGHT:
                if (posUsuario < longitud - 1) {
                    posUsuario++;
                }
                break;
            case 'i':
                instrucciones();
                break;
            case 'q':
                comprobacion = 0; // salir del programa
                break;
            case 'k':
            case 10: // enter
                if (path[posUsuario * 3] == '0') { // para escribir un número el nodo ha de estar vacío
                    if (posUsuario == 0) { // si estamos en el primer nodo
                        if (path[(posUsuario + 1) * 3] == '1') {
                            path[posUsuario * 3] = '2';
                        }
                        else {
                            path[posUsuario * 3] = '1';
                        }
                    }
                    else if (posUsuario == longitud - 1) { // si estamos en el último nodo
                        if (path[(posUsuario - 1) * 3] == '1') {
                            path[posUsuario * 3] = '2';
                        }
                        else {
                            path[posUsuario * 3] = '1';
                        }
                    }
                    else { // en cualquier nodo intermedio
                        if (path[(posUsuario - 1) * 3] != '1' && path[(posUsuario + 1) * 3] != '1') {
                            path[posUsuario * 3] = '1';
                        }
                        else if (path[(posUsuario - 1) * 3] != '2' && path[(posUsuario + 1) * 3] != '2') {
                            path[posUsuario * 3] = '2';
                        }
                        else {
                            path[posUsuario * 3] = '3';
                            comprobacion = 0; // condición de fin del juego
                        }
                    }
                    turno++;        // cada vez que se pulsa enter se introduce un número en el nodo y se pasa el turno
                    comprobacion--; // al llegar a 0 se acaba la partida
                }
                break;
        }
    }

    mostrarPath(path, longitud, posUsuario);
    mvprintw(3 + to_down, 0 + to_right, "¡Gana el jugador: %d!", turno % 2);
    getch(); // Esperar a que el usuario presione una tecla para ver la pantalla de victoria
    endwin();

    return 0;
}