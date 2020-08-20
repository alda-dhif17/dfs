#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "graph.h"

int main(int argc, char** argv) {
    FILE* fp = fopen("./01.txt", "r");
    Graph<int>* graph = loadIntGraphFromFile(fp);

    // print the graph to the console ... 
    graph->print();

    /*
     * End-Result:
     *                |---|
     *      A ------> B -|
     *      |      /  \       
     *      |    /     \
     *      |  /        \
     *      C            D
     */

    // -- MESS WITH GRAPH -- //

    graph->addNode(new Node<int>("E"));
    graph->addNode(new Node<int>("F"));
    graph->addEdge("A", "E");
    graph->addEdge("E", "F");
    graph->addEdge("B", "E");
    graph->delNode("A");

    // --------------------- //

    // graph->print();    

    /*
     * End-Result:
     *      B  ------ C
     *      |\
     *      | \
     *      |  \
     *      |   v
     *      D   E ---> F
     */

    // -- PERFORM SEARCHES ON THE GRAPH -- //

    printf("  -- SEARCHES -- \n");

    printf(" Searching: B -> F\n");
    std::pair<int, std::vector<Node<int> *>> res = graph->defise("B", "F");

    if (res.second.empty()) {
        printf("No path found!\n");
    } else {
        printf(" Total cost: %d\n", res.first);
        printf(" Path: ");
        for (int i = 0; i < res.second.size()-1; i++) {
            printf("%s -> ", res.second[i]->getName());
        }
        printf("%s\n", res.second.back()->getName());
    }

    graph->unvisit("B");
    printf("\n Searching: F -> D\n");
    res = graph->defise("F", "D");

    if (res.second.empty()) {
        printf(" No path found!\n");
    } else {
        printf(" Total cost: %d\n", res.first);
        printf(" Path: ");
        for (int i = 0; i < res.second.size()-1; i++) {
            printf("%s -> ", res.second[i]->getName());
        }
        printf("%s\n", res.second.back()->getName());
    }

    graph->unvisit("F");
    return 0;
}