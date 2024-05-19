#include <stdlib.h>   // para rand(), exit(), atoi()
#include <time.h>     // para srand(), cronómetro
#include <ncurses.h>
#include <unistd.h>   // para sleep() / usleep()
#include <pthread.h>  // para los hilos
#include <sys/time.h>
#include <string.h>

#define MAX_DIFICULTAD 10

volatile int detener_hilo = 0; //variable global que detendrá la barra cuando el juego termine
pthread_mutex_t ncurses_mutex = PTHREAD_MUTEX_INITIALIZER; //para evitar el acceso concurrente de hilos en ciertas secciones

// Función para hacer reset de todos los records

void reset() {
    FILE *archivo;
    archivo = fopen("score_tiempo.txt", "r+");

    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char linea[100]; // Suponemos que cada línea tiene un máximo de 100 caracteres

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Mover el puntero de archivo al principio de la línea actual
        fseek(archivo, -(long)strlen(linea), SEEK_CUR);

        // Escribir el nuevo valor, rellenando el espacio hasta el final de la línea original
        fprintf(archivo, "20.0000");
        
        // Si la línea original era más larga, rellenar con espacios para evitar residuos
        for (size_t i = strlen("20.0000"); i < strlen(linea) - 1; i++) {
            fputc(' ', archivo);
        }

        // Escribir un salto de línea al final
        fputc('\n', archivo);
        
        // Mover el puntero de archivo al inicio de la siguiente línea
        fseek(archivo, 0, SEEK_CUR);
    }

    fclose(archivo);
}

//Función para seleccionar la dificultad
int selec() {
    clear();
    char cadena[10];
    int dificultad;

    int term_size_x = getmaxx(stdscr);  
    int term_size_y = getmaxy(stdscr);  

    int to_right = (term_size_x - 32)/2;
    int to_down = (term_size_y)/2 - 2;

    mvprintw(to_down-1, to_right, "Selecciona la dificultad: (3-10)\n");
    mvprintw(to_down+1, to_right-9, "Puedes pulsar '1' para resetear todos los records.");

    int y, x;
    getyx(stdscr, y, x);
    int centerX = term_size_x/2 - 1;
    move(y+1, centerX);

    getnstr(cadena, sizeof(cadena));  // lo hago así para obtener dos cifras en caso de que sea 10
    dificultad = atoi(cadena);        // convertir en int

    to_right = (term_size_x - 44)/2;
    if (dificultad == 1) {
        reset();
        mvprintw(to_down+1, to_right-9, "     Todos los records reseteados. Pulsa cualquier tecla.");
        getch();
        return selec();
    } else if (dificultad < 3 || dificultad > MAX_DIFICULTAD) {
        clear();
        mvprintw(to_down, to_right, "Dificultad no válida. Pulsa cualquier tecla.\n");
        getch();
        clear();
        return selec();
    }

    return dificultad;
}

//Función para mostrar la cadena de números aleatorios
void mostrarArray(int array[], int tamaño, int posUsuario) {
    int term_size_x = getmaxx(stdscr);  
    int term_size_y = getmaxy(stdscr);  
    int to_down = (term_size_y)/2;

    pthread_mutex_lock(&ncurses_mutex); // Hacemos lock al principio de la función (por seguridad)

    int to_right = (term_size_x - 28)/2;
    mvprintw(to_down-1, to_right, "Array de números aleatorios:\n");

    to_right = (term_size_x - tamaño)/2;
    for (int i = 0; i < tamaño; i++) {
        if (posUsuario == i) {  // se muestra resaltado el carácter sobre el que estamos
            attron(A_REVERSE);
            mvprintw(to_down+1, to_right+i, "%d ", array[i]);
            attroff(A_REVERSE);
        } else {
            mvprintw(to_down+1, to_right+i, "%d ", array[i]);
        }
    }
    printw("\n\n");
    refresh();

    pthread_mutex_unlock(&ncurses_mutex);  // hacemos unlock al final de la función
}

// función para comprobar si el array de números está ordenado
int estaOrdenado(int arr[], int longitud) {
    for (int i = 0; i < longitud - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1; // condición de victoria
}

// función que hará el hilo -> barra decreciente por tiempo
void *tarea1(void *arg) {

    int term_size_x = getmaxx(stdscr);  
    int to_right = (term_size_x - 18)/2;
    int term_size_y = getmaxy(stdscr);  
    int to_down = (term_size_y)/2 - 2;

    pthread_mutex_lock(&ncurses_mutex);  // lock  
    initscr();                           // iniciar ncurses con el lock activo
    use_default_colors();                
    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_MAGENTA, -1);
    init_pair(3, COLOR_GREEN, -1);
    pthread_mutex_unlock(&ncurses_mutex);// unlock

    //barra
    //-------------------------------------------------  
    int to_right2 = (term_size_x - 20)/2;
    int to_down2 = (term_size_y)/2 + 4;

    char barra[24] = "------------------------";
    sleep(1);

    for (int i = 10; i > 0; i--) {
        if (detener_hilo != 1) {
            pthread_mutex_lock(&ncurses_mutex);

            barra[i*2] = '\0';
            barra[i*2-1] = '\0';
            mvprintw(to_down2, to_right2-1, "[                    ]");
            if (barra[12] == '-') {
                attron(COLOR_PAIR(3));
                mvprintw(to_down2, to_right2, "%s",barra);
                attroff(COLOR_PAIR(3));
            }
            else if (barra[6] == '-') {
                attron(COLOR_PAIR(2));
                mvprintw(to_down2, to_right2, "%s",barra);
                attroff(COLOR_PAIR(2));
            } else {
                attron(COLOR_PAIR(1));
                mvprintw(to_down2, to_right2, "%s",barra);
                attroff(COLOR_PAIR(1));
            }

            refresh();
            pthread_mutex_unlock(&ncurses_mutex);
            sleep(1);
        } else {
            break;
        }
    }
    //-------------------------------------------

    if (detener_hilo == 0) {  // si el jugador no ha ganado, salimos del juego
        endwin();
        exit(0);
    }

    return NULL;
}

double obtener_tiempo() {
    struct timeval tiempo;
    gettimeofday(&tiempo, NULL);
    return tiempo.tv_sec + tiempo.tv_usec * 1e-6;
}

int main() {
    initscr();
    keypad(stdscr, TRUE);
    clear();

    int term_size_x = getmaxx(stdscr);  
    int to_right = (term_size_x - 15)/2;
    int term_size_y = getmaxy(stdscr);  
    int to_down = (term_size_y)/2;

    int dificultad = selec();
    int numeros[dificultad];
    curs_set(0);
    noecho();

    double inicio = obtener_tiempo();  // al seleccionar la dificultad se inicia el cronómetro

    pthread_t hilo1;  // inicializamos el hilo
    if (pthread_create(&hilo1, NULL, tarea1, NULL) != 0) {
        perror("Error al crear el hilo 1");
        return 1;
    }

    int comprobacion = 0; // condición de victoria
    srand(time(NULL));    // creamos el array aleatorio
    do {                  // nos aseguramos de que no sale ya ordenado
        for (int i = 0; i < dificultad; i++) {
        numeros[i] = rand() % 10;
        }
        comprobacion = estaOrdenado(numeros, dificultad);
    } while (comprobacion == 1);

    clear();
    int posUsuario = 0;
    mostrarArray(numeros, dificultad, posUsuario);

    int movimientos = 0;  // para score
    int ch;
    while (comprobacion == 0 && detener_hilo == 0) {
        ch = getch();
        switch (ch) {
            case 'q':    // salida auto del juego
                endwin();
                exit(0);
                break;
            case KEY_LEFT:
            case 'h':
                if (posUsuario > 0) {
                    posUsuario--;
                }
                break;
            case KEY_RIGHT:
            case 'l':
                if (posUsuario < dificultad - 1) {
                    posUsuario++;
                }
                break;
            case 10:
            case 'k':
                if (posUsuario < dificultad - 1) {
                    int temp = numeros[posUsuario];
                    numeros[posUsuario] = numeros[posUsuario + 1];
                    numeros[posUsuario + 1] = temp;
                    movimientos++;
                } else if (posUsuario == dificultad -1) {
                    int temp = numeros[posUsuario];
                    numeros[posUsuario] = numeros[0];
                    numeros[0] = temp;
                    movimientos++;
                }
                break;
        }
        if (ch != 'q') {
            mostrarArray(numeros, dificultad, posUsuario);
            comprobacion = estaOrdenado(numeros, dificultad);
        }
    }

    // TIME SCORE -----------------------------------------------------------------
    double fin = obtener_tiempo(); // al superar el juego se para el cronómetro
    double tiempo_transcurrido = fin - inicio;

    FILE *archivo;
    archivo = fopen("score_tiempo.txt", "r+");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    char linea[100]; // Suponemos que cada línea tiene un máximo de 100 caracteres
    int numero_linea = dificultad - 1; // Número de la línea a leer
    int linea_actual = 1;
    float record;

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (linea_actual == numero_linea) {
            if (sscanf(linea, "%f", &record) == 1) {
                if (tiempo_transcurrido < record) {
                    // Mover el puntero de archivo al principio de la línea
                    fseek(archivo, -(strlen(linea) + 1), SEEK_CUR);
                    // Escribir el nuevo tiempo transcurrido en la línea
                    char buffer[50];
                    sprintf(buffer, "%.2f", tiempo_transcurrido);
                    fprintf(archivo, "\n%s", buffer);
                    // Salir del bucle después de actualizar el registro
                    break;
                }
            }
        }
        linea_actual++;
    }

    fclose(archivo);

    // TIME SCORE ----------------------------------------------------------------

    clear();
    mvprintw(to_down-2, to_right, "¡Juego superado!");
    mvprintw(to_down, to_right+4, "Score:");
    mvprintw(to_down+2, to_right + 1, "Dificultad: %d", dificultad);
    mvprintw(to_down+3, to_right, "Movimientos: %d", movimientos);
    mvprintw(to_down+5, to_right-7, "Tu tiempo: %.2f   Record: %.2f", tiempo_transcurrido, record);

    
    detener_hilo = 1; // detener barra
    getch();
    clear();

    endwin();
    pthread_join(hilo1, NULL); // Esperar a que el hilo termine antes de salir (cuestión de 
    exit(0);                   // seguridad, es una buena práctica pero no es esencial)

    return 0;
}