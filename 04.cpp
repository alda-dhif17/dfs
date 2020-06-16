#include <stdio.h>
#include <string.h>

#include <vector>
#include <utility>
#include <algorithm>

#include "graph.h"

int main(int argc, char** argv) {
    FILE* fp = fopen("./04.txt", "r");
    Graph<int>* graph = loadIntGraphFromFile(fp);

    std::pair<int, std::vector<Node<int> *>> p = graph->dijkstra("A", "H");
    if (p.first == -1) {
        printf("No path found!\n");
    } else {
        printf("Total path cost: %d\nPath: ", p.first);
        for (int i = 0; i < p.second.size()-1; i++) {
            printf("%s -> ", p.second[i]->getName());
        }
        printf("%s\n", p.second.back()->getName());
    }
    return 0;
}