#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_NODES 7
#define MAX_EDGES 4

#define NONE -1
/*                 // Ya está definido en el main
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
*/

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
    if (graph->nodes[src].neighbor_count < MAX_EDGES && graph->nodes[dest].neighbor_count < MAX_EDGES) {
        graph->nodes[src].doors[direction] = dest;
        if (direction == UP) {
            graph->nodes[dest].doors[DOWN] = src;
        } else if (direction == DOWN) {
            graph->nodes[dest].doors[UP] = src;
        } else if (direction == LEFT) {
            graph->nodes[dest].doors[RIGHT] = src;
        } else if (direction == RIGHT) {
            graph->nodes[dest].doors[LEFT] = src;
        }
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
    srand(time(NULL));
    Graph* graph = create_graph();

    for (int i = 0; i < MAX_NODES; i++) {
        int num_connections = (rand() % 4) + 1;
        int connections = 0;

        while (connections < num_connections) {
            int target = rand() % MAX_NODES;
            if (target != i && !is_connected(graph, i, target)) {
                int direction = rand() % 4;
                if (graph->nodes[i].doors[direction] == NONE) {
                    add_edge(graph, i, target, direction);
                    connections++;
                }
            }
        }
    }

    // Cada nodo tiene entre 1 y 4 aristas
    for (int i = 0; i < MAX_NODES; i++) {
        if (graph->nodes[i].neighbor_count == 0) {
            int target;
            int direction;
            do {
                target = rand() % MAX_NODES;
                direction = rand() % 4;
            } while (target == i || is_connected(graph, i, target) || graph->nodes[i].doors[direction] != NONE || graph->nodes[target].neighbor_count >= MAX_EDGES);
            add_edge(graph, i, target, direction);
        }
    }

    return graph;
}