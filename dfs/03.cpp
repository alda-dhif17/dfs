#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "graph.h"

int main(int argc, char** argv) {
    // open the target file ...
    FILE* fp = fopen("./03.txt", "r");
    // read the number of nodes from the file ...
    Graph<int>* graph = loadIntGraphFromFile(fp);

    // std::pair<int, std::vector<Node<int> *>> res = graph->pr_defise("A", "F");
    std::pair<int, std::vector<Node<int> *>> res = graph->pr_defise("A", "D");
    
    if (res.second.empty()) {
        printf("No path found!\n");
    } else {
        printf("Total cost: %d\nPath: ", res.first);
        for (int i = 0; i < res.second.size()-1; i++) {
            printf("%s -> ", res.second[i]->getName());
        }
        printf("%s\n", res.second.back()->getName());
    }

    graph->unvisit("A");

    // --------------------- //
    
    return 0;
}