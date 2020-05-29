#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "graph.h"

//
// DIRECTED, WEIGHTED GRAPH CREATION (from file)
// - May 2020
// - David Weinhandl, Nico Pessnegger, Moritz Rief, Paul Weiß, Philipp Zach und Matthias Monschein
//

// NUMBER OF NODES ...
int N;

int main(int argc, char** argv) {
    // open the target file ...
    FILE* fp = fopen("./in.txt", "r");
    // read the number of nodes from the file ...
    fscanf(fp, "%d", &N);
    Graph<int>* graph = (Graph<int>*) malloc(sizeof(Graph<int>));
    int n;
    // iterate over all nodes ...
    for (int i = 0; i < N; i++) {
        char* tmp = (char*) malloc(sizeof(char)*MAX_NAME_LEN);
        fscanf(fp, "%s", tmp);
        Node<int>* ntmp = graph->getNode(tmp);
        if (ntmp == (Node<int>*) NULL) {
            ntmp = new Node<int>(tmp);
            graph->addNode(ntmp);
        }
        // read the number of edges from the file ...
        fscanf(fp, "%d", &n);
        // iterate over node's edges ...
        for (int j = 0; j < n; j++) {
            tmp = (char*) malloc(sizeof(char)*MAX_NAME_LEN);
            // read the target node ...
            fscanf(fp, "%s", tmp);
            Node<int>* nei = graph->getNode(tmp);
            if (nei == (Node<int>*) NULL) {
                nei = new Node<int>(tmp);
                graph->addNode(nei);
            }
            if (!ntmp->isNeighbour(nei->getName())) ntmp->addNeighbour(nei);
            if (!nei->isNeighbour(ntmp->getName())) nei->addNeighbour(ntmp);
        }
    }
    
    // print the graph to the console ... 
    // graph->print();

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

    // graph->addNode(new Node<int>("E"));
    // graph->addNode(new Node<int>("F"));
    // graph->addEdge("A", "E");
    // graph->addEdge("E", "F");
    // graph->addEdge("B", "E");
    // graph->delNode("A");

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

    // std::vector<Node<int> *> path = graph->defise("B", "F");

    // if (path.empty()) {
    //     printf("No path found!\n");
    // } else {
    //     printf(" ");
    //     for (int i = 0; i < path.size()-1; i++) {
    //         printf("%s -> ", path[i]->getName());
    //     }
    //     printf("%s\n", path.back()->getName());
    // }

    // graph->unvisit("B");
    // path = graph->defise("F", "D");

    // if (path.empty()) {
    //     printf(" No path found!\n");
    // } else {
    //     printf(" ");
    //     for (int i = 0; i < path.size()-1; i++) {
    //         printf("%s -> ", path[i]->getName());
    //     }
    //     printf("%s\n", path.back()->getName());
    // }

    // graph->unvisit("F");

    std::vector<Node<int> *> path = graph->defise("A", "F");
    if (path.empty()) {
        printf("No path found!\n");
    } else {
        printf(" ");
        for (int i = 0; i < path.size()-1; i++) {
            printf("%s -> ", path[i]->getName());
        }
        printf("%s\n", path.back()->getName());
    }

    // --------------------- //
    
    return 0;
}