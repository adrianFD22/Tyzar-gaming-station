#include <stdlib.h>
#include <stdio.h>

#define MAX_NODES 9
#define MAX_EDGES 4

#define NONE -1
#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3

typedef struct {
    int id;
    int doors[MAX_EDGES];
} Node;

typedef struct {
    Node nodes[MAX_NODES];
} Graph;

Graph* create_graph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) {
        fprintf(stderr, "Failed to allocate memory for graph.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MAX_NODES; i++) {
        graph->nodes[i].id = i;
        for (int j = 0; j < MAX_EDGES; j++) {
            graph->nodes[i].doors[j] = NONE;
        }
    }

    return graph;
}

void add_manual_edges(Graph* graph) {
    graph->nodes[0].doors[UP] = -1;
    graph->nodes[0].doors[DOWN] = 3;
    graph->nodes[0].doors[LEFT] = -1;
    graph->nodes[0].doors[RIGHT] = 1;

    graph->nodes[1].doors[UP] = -1;
    graph->nodes[1].doors[DOWN] = 4;
    graph->nodes[1].doors[LEFT] = 0;
    graph->nodes[1].doors[RIGHT] = 2;

    graph->nodes[2].doors[UP] = -1;
    graph->nodes[2].doors[DOWN] = 5;
    graph->nodes[2].doors[LEFT] = 1;
    graph->nodes[2].doors[RIGHT] = -1;

    graph->nodes[3].doors[UP] = 0;
    graph->nodes[3].doors[DOWN] = 6;
    graph->nodes[3].doors[LEFT] = -1;
    graph->nodes[3].doors[RIGHT] = 4;

    graph->nodes[4].doors[UP] = 1;
    graph->nodes[4].doors[DOWN] = 7;
    graph->nodes[4].doors[LEFT] = 3;
    graph->nodes[4].doors[RIGHT] = 5;

    graph->nodes[5].doors[UP] = 2;
    graph->nodes[5].doors[DOWN] = 8;
    graph->nodes[5].doors[LEFT] = 4;
    graph->nodes[5].doors[RIGHT] = -1;

    graph->nodes[6].doors[UP] = 3;
    graph->nodes[6].doors[DOWN] = -1;
    graph->nodes[6].doors[LEFT] = -1;
    graph->nodes[6].doors[RIGHT] = 7;

    graph->nodes[7].doors[UP] = 4;
    graph->nodes[7].doors[DOWN] = -1;
    graph->nodes[7].doors[LEFT] = 6;
    graph->nodes[7].doors[RIGHT] = 8;

    graph->nodes[8].doors[UP] = 5;
    graph->nodes[8].doors[DOWN] = -1;
    graph->nodes[8].doors[LEFT] = 7;
    graph->nodes[8].doors[RIGHT] = -1;
}

void print_graph(Graph* graph) {
    printf("Grafo:\n");
    for (int i = 0; i < MAX_NODES; i++) {
        printf("Sala %d:", graph->nodes[i].id);
        for (int j = 0; j < MAX_EDGES; j++) {
            if (graph->nodes[i].doors[j] == NONE) {
                if (j == UP) {
                    printf(" Arriba->-1");
                } else if (j == DOWN) {
                    printf(" Abajo->-1");
                } else if (j == LEFT) {
                    printf(" Izquierda->-1");
                } else if (j == RIGHT) {
                    printf(" Derecha->-1");
                }
            } else {
                if (j == UP) {
                    printf(" Arriba->%d", graph->nodes[i].doors[j]);
                } else if (j == DOWN) {
                    printf(" Abajo->%d", graph->nodes[i].doors[j]);
                } else if (j == LEFT) {
                    printf(" Izquierda->%d", graph->nodes[i].doors[j]);
                } else if (j == RIGHT) {
                    printf(" Derecha->%d", graph->nodes[i].doors[j]);
                }
            }
        }
        printf("\n");
    }
}

int main() {
    Graph* graph = create_graph();
    add_manual_edges(graph);
    print_graph(graph);

    free(graph);
    return 0;
}
