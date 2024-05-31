/*
Notas de versión de Ahorcado
versión 2: lista de múltiples palabras secretas
versión 3: ncurses + inicio letras gigantes + muneco
versión 4: muneco rojo + solucionado bug letras usadas + fichero txt externo
*/

#include <stdio.h>
#include <string.h>  // para strlen()
#include <stdlib.h>
#include <time.h>    // para aleatoriedad
#include <ncurses.h>

#define MAX_INTENTOS 7
#define MAX_LENGTH 100

// Función para comprobar si la letra está en la palabra secreta y revelar la letra
int comprobarLetra(char *palabra, char letra, char *palabraAdivinada) {
    int aciertos = 0;
    for (int i = 0; i < strlen(palabra); i++) {
        if (palabra[i] == letra) {
            palabraAdivinada[i] = letra;
            aciertos++;
        }
    }
    return aciertos;
}

// Inicio con letras gigantes
void inicio() {
    int term_size_x = getmaxx(stdscr);       // para centrar
    int term_size_y = getmaxy(stdscr);       // la pantalla
    int to_right = (term_size_x - 47)/2 - 1;
    int to_down = (term_size_y - 12)/2;

    mvprintw(0+to_down, 1+to_right, "  //      //  //    //////    //////    //////");
    mvprintw(1+to_down, 1+to_right, " // //    //  //    //  //    //  //    //");
    mvprintw(2+to_down, 1+to_right, "//////    //////    //  //    /////     //");
    mvprintw(3+to_down, 1+to_right, "/    /    //  //    //  //    //  //    //");
    mvprintw(4+to_down, 1+to_right, "/    /    //  //    //////    //   /    //////");

     mvprintw(6+to_down, 1+to_right, "  //      /////     //////         |---      ");
     mvprintw(7+to_down, 1+to_right, " // //    //   /    //  //         0         ");
     mvprintw(8+to_down, 1+to_right, "//////    //   /    //  //        -|-        ");
     mvprintw(9+to_down, 1+to_right, "/    /    //   /    //  //        / \\        ");
    mvprintw(10+to_down, 1+to_right, "/    /    /////     //////                   ");

    mvprintw(12+to_down, 3, "Presiona cualquier tecla para continuar...");
    getch();
    clear();
}

void dibujarMuneco(int intentos) {
    int term_size_x = getmaxx(stdscr);
    int to_right = (term_size_x - 2)/2;

    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, -1); // color rojo para el muneco

    if (intentos > 0) {
        attron(COLOR_PAIR(1));
        mvprintw(9, 0+to_right, "|---");
        attroff(COLOR_PAIR(1));
    }
    if (intentos > 1) {
        attron(COLOR_PAIR(1));
        mvprintw(10, 0+to_right, "0");
        attroff(COLOR_PAIR(1));
    }
    if (intentos > 2) {
        attron(COLOR_PAIR(1));
        mvprintw(11, 0+to_right, "|");
        attroff(COLOR_PAIR(1));
    }
    if (intentos > 3) {
        attron(COLOR_PAIR(1));
        mvprintw(11, 0+to_right-1, "-|");
        attroff(COLOR_PAIR(1));
    }
    if (intentos > 4) {
        attron(COLOR_PAIR(1));
        mvprintw(11, 0+to_right-1, "-|-");
        attroff(COLOR_PAIR(1));
    }
    if (intentos > 5) {
        attron(COLOR_PAIR(1));
        mvprintw(12, 0+to_right-1, "/");
        attroff(COLOR_PAIR(1));
    }
    if (intentos > 6) {
        attron(COLOR_PAIR(1));
        mvprintw(12, 0+to_right-1, "/ \\");
        attroff(COLOR_PAIR(1));
    }

    printw("\n\n");
}

// Para leer las palabras del archivo externo palabras_ahorcado.txt
char** obtener_palabras(const char* nombreArchivo, int* numPalabras) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        exit(1);
    }

    // Crear un arreglo dinámico para almacenar las palabras
    char** palabras = malloc(sizeof(char*) * 100); // Inicialmente reservamos espacio para 100 palabras
    if (palabras == NULL) {
        perror("Error al reservar memoria");
        exit(1);
    }

    char palabra[MAX_LENGTH];
    *numPalabras = 0;

    // Leer palabras del archivo y almacenarlas en el arreglo
    while (fscanf(archivo, "%s", palabra) != EOF && *numPalabras < 100) {
        palabras[*numPalabras] = strdup(palabra); // strdup crea una copia de la palabra
        (*numPalabras)++;
    }

    // Cerrar el archivo
    fclose(archivo);

    return palabras;
}

int main() {
    int numPalabras;
    char** palabras = obtener_palabras("palabras_ahorcado.txt", &numPalabras);

    initscr(); // Iniciar el modo ncurses
    inicio();

    // Palabra elegida aleatoriamente, lo cual genera incertidumbre y diversión
    srand(time(NULL));
    int indice = rand() % numPalabras;
    char *palabraSecreta = palabras[indice];

    char palabraAdivinada[strlen(palabraSecreta) + 1];      // un carácter más para '\0'
    memset(palabraAdivinada, '_', strlen(palabraSecreta));  // palabra hecha de '_' para dar misterio
    palabraAdivinada[strlen(palabraSecreta)] = '\0';        // necesario para que no explote el mundo

    char letra;
    char letrasUsadas[20];
    int intentos = 0;

    while (intentos < MAX_INTENTOS && strcmp(palabraSecreta, palabraAdivinada) != 0) { //quedan intentos Y no has acertado la palabra
        int term_size_x = getmaxx(stdscr);
        int to_right = (term_size_x - (10+strlen(palabraSecreta)))/2;
        printw(" Intenta adivinar la palabra secreta. Son solo minusculas y no hay acentos.\n\n");
        mvprintw(3, 0+to_right, " Palabra: %s\n", palabraAdivinada);
        printw("\n Introduce una letra: ");
        printw("\n\n Puedes presionar 'q' para salir del juego.");
        printw("\n Letras usadas: %s ", letrasUsadas);
        dibujarMuneco(intentos);
        refresh();

        int y, x;
        getyx(stdscr, y, x);
        int centerX = term_size_x/2 - 1;
        move(y, centerX);

        scanw(" %c", &letra);
        if (letra == 'q') {
            break; //salimos del juego
        }

        int len = strlen(letrasUsadas);
        int rep = 0;
        for (int i = 0; i < len; i++) {
            if (letrasUsadas[i] == letra) {
                rep = 1;
                break;
            }
        }
        if (rep == 0) {                    // se añade la letra si no está repetida
            letrasUsadas[len] = letra;
            letrasUsadas[len + 1] = '\0';
        }

        if (comprobarLetra(palabraSecreta, letra, palabraAdivinada) == 0) {
            clear();
            intentos++;
            printw(" La letra '%c' no esta en la palabra.", letra);
            printw(" Te quedan %d intentos.\n", MAX_INTENTOS - intentos);
            refresh();
        } else {
            clear();
            printw(" ¡Acertaste una letra!\n");
            refresh();
        }
    }

    dibujarMuneco(intentos);

    int term_size_x = getmaxx(stdscr);
    int to_right = (term_size_x - 2)/2;

    curs_set(0);

    if (strcmp(palabraSecreta, palabraAdivinada) == 0) {
        mvprintw(14, 1, " ¡Felicidades! Has adivinado la palabra secreta: %s\n", palabraSecreta);

        attron(COLOR_PAIR(1));
        mvprintw(9, to_right, "    ");
        mvprintw(10, to_right, "0");
        mvprintw(11, 0+to_right-8, "¡SIGO  -|-  VIVO!");
        mvprintw(12, 0+to_right-1, "/ \\");
        attroff(COLOR_PAIR(1));
    } else {
        mvprintw(14, 1, " Se te acabaron los intentos. La palabra secreta era: %s\n", palabraSecreta);

        attron(COLOR_PAIR(1));
        mvprintw(10, to_right-9, "GAME");
        mvprintw(10, to_right+7, "OVER");
        attroff(COLOR_PAIR(1));
    }

    refresh();
    getch(); // Esperar a que el usuario presione una tecla para ver la pantalla final
    endwin(); 

    return 0;
}