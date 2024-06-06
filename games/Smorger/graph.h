#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_NODES 7
#define MAX_EDGES 4

#define NONE -1 // puertas cerradas

typedef struct {
    int id;
    int doors[MAX_EDGES];
    int neighbor_count;
} Node;

typedef struct {
    Node nodes[MAX_NODES];
    int num_nodes;
} Graph;

Graph* create_graph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) {
        fprintf(stderr, "Failed to allocate memory for graph.\n");
        exit(EXIT_FAILURE);
    }
    graph->num_nodes = MAX_NODES;

    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i].id = i;
        graph->nodes[i].neighbor_count = 0;
        for (int j = 0; j < MAX_EDGES; j++) {
            graph->nodes[i].doors[j] = NONE;
        }
    }

    return graph;
}

void add_edge(Graph* graph, int src, int dest, int direction) {
    if (src < 0 || src >= MAX_NODES || dest < 0 || dest >= MAX_NODES) {
        fprintf(stderr, "Invalid node indices: src=%d, dest=%d\n", src, dest);
        return;
    }

    if (graph->nodes[src].neighbor_count >= MAX_EDGES || graph->nodes[dest].neighbor_count >= MAX_EDGES) {
        fprintf(stderr, "Cannot add edge: neighbor limit reached for src=%d or dest=%d\n", src, dest);
        return;
    }

    if (graph->nodes[src].doors[direction] == NONE && graph->nodes[dest].doors[(direction + 2) % 4] == NONE) {
        graph->nodes[src].doors[direction] = dest;
        graph->nodes[dest].doors[(direction + 2) % 4] = src;
        graph->nodes[src].neighbor_count++;
        graph->nodes[dest].neighbor_count++;
    }
}

int is_connected(Graph* graph, int src, int dest) {
    for (int i = 0; i < MAX_EDGES; i++) {
        if (graph->nodes[src].doors[i] == dest) {
            return 1;
        }
    }
    return 0;
}

Graph* create_random_graph() {
    srand((unsigned int)time(NULL)); // Inicializar la semilla aleatoria
    Graph* graph = create_graph();

    // Asegurar que cada nodo tenga al menos una conexión
    for (int i = 0; i < MAX_NODES; i++) {
        int attempts = 0;
        while (graph->nodes[i].neighbor_count == 0 && attempts < 100) {
            int target = rand() % MAX_NODES;
            int direction = rand() % 4;
            if (target != i && graph->nodes[i].doors[direction] == NONE && graph->nodes[target].neighbor_count < MAX_EDGES) {
                add_edge(graph, i, target, direction);
            }
            attempts++;
        }
    }

    // Agregar conexiones adicionales al azar para cada nodo
    for (int i = 0; i < MAX_NODES; i++) {
        int attempts = 0;
        while (graph->nodes[i].neighbor_count < MAX_EDGES && attempts < 100) {
            int target = rand() % MAX_NODES;
            int direction = rand() % 4;
            if (target != i && !is_connected(graph, i, target) && graph->nodes[i].doors[direction] == NONE && graph->nodes[target].neighbor_count < MAX_EDGES) {
                add_edge(graph, i, target, direction);
            }
            attempts++;
        }
    }

    return graph;
}