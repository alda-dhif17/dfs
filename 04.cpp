#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include "graph.h"

int main(int argc, char** argv) {
    FILE* fp = fopen("./04.txt", "r");
    Graph<int>* graph = loadIntGraphFromFile(fp);
    
    return 0;
}