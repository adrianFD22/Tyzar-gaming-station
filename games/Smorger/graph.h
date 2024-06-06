#include <stdlib.h>
#include <time.h>

#define MAX_NODES 7 // Arbitrariamente, puede ser variable según la dificultad del nivel

// Para abrir/cerrar puertas
#define ARRIBA    0
#define ABAJO     1
#define IZQUIERDA 2
#define DERECHA   3

typedef struct {
    int id;                   // Identificador único de la sala
    int neighbors[MAX_NODES]; // Lista de salas adyacentes 
} Node;

typedef struct {
    Node nodes[MAX_NODES];
    int num_nodes;
} Graph;

// Función para crear un grafo mazmorra
Graph* create_graph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_nodes = MAX_NODES;                    // Se puede modificar para que vaya aumentando con los niveles.
    for (int i = 0; i < MAX_NODES; i++) {            // En un principio se genera sin aristas, las añadiremos
        graph->nodes[i].id = i;                      // posteriormente mediante un algoritmo para generar la mazmorra con sentido.
        for (int j = 0; j < MAX_NODES; j++) {
            graph->nodes[i].neighbors[j] = -1;       // -1 indica que no hay arista.
        }
    }
    return graph;
}

// Función para añadir una arista entre dos nodos
void add_edge(Graph* graph, int src, int dest) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (graph->nodes[src].neighbors[i] == -1) {
            graph->nodes[src].neighbors[i] = dest;
            break;
        }
    }
    for (int i = 0; i < MAX_NODES; i++) {
        if (graph->nodes[dest].neighbors[i] == -1) {
            graph->nodes[dest].neighbors[i] = src;
            break;
        }
    }
}

// Función para verificar si hay una conexión entre dos nodos
int is_connected(Graph* graph, int src, int dest) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (graph->nodes[src].neighbors[i] == dest) {
            return 1;
        }
    }
    return 0;
}

// Función para crear un grafo aleatorio con las restricciones mencionadas
Graph* create_random_graph() {
    srand(time(NULL)); // Inicializar la semilla para los números aleatorios
    Graph* graph = create_graph();

    // Iterar sobre cada nodo para asegurarse de que esté conectado al menos a una sala
    for (int i = 0; i < MAX_NODES; i++) {
        int num_connections = (rand() % 4) + 1; // Número aleatorio de conexiones entre 1 y 4
        int connections = 0;
        
        while (connections < num_connections) {
            int target = rand() % MAX_NODES;
            if (target != i && !is_connected(graph, i, target)) {
                add_edge(graph, i, target);
                connections++;
            }
        }
    }

    return graph;
}

// Función para determinar la dirección entre dos nodos
int determine_direction(int src, int dest) {
    if (src == dest - 1) return DERECHA;
    if (src == dest + 1) return IZQUIERDA;
    if (src == dest - MAX_NODES) return ABAJO;
    if (src == dest + MAX_NODES) return ARRIBA;
    return -1;
}

// Función para abrir o cerrar puertas según las conexiones
void configure_doors(Graph* graph, int*** rooms) {
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            if (graph->nodes[i].neighbors[j] != -1) {
                int direction = determine_direction(i, graph->nodes[i].neighbors[j]);
                if (direction != -1) {
                    switch (direction) {
                        case ARRIBA:
                            rooms[i][0][WIDTH / 2] = 0;
                            break;
                        case ABAJO:
                            rooms[i][HEIGHT - 1][WIDTH / 2] = 0;
                            break;
                        case IZQUIERDA:
                            rooms[i][HEIGHT / 2][0] = 0;
                            break;
                        case DERECHA:
                            rooms[i][HEIGHT / 2][WIDTH - 1] = 0;
                            break;
                    }
                }
            }
        }
    }
}