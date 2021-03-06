#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <string.h>

#include <queue>
#include <vector>
#include <utility>
#include <algorithm>

//
// DIRECTED, WEIGHTED GRAPH
// - May 2020
// - David Weinhandl, Nico Pessnegger, Moritz Rief, Paul Weiß, Philipp Zach und Matthias Monschein
//

// ----------------------------------------------------------- DEFINITIONS ----------------------------------------------------------- //

#define MAX_NAME_LEN 64

template <class T>
struct Node;

template <class T>
struct Edge;

template <class T>
struct Graph;

// ----------------------------------------------------------- IMPLEMENTATIONS ----------------------------------------------------------- //

template <class T>
struct Node {
   private:
    char *name;
    T value;
    std::vector<Edge<T> *> edges;
    bool visited;
    Node<T> *prev;

   public:
    /**
     * Create a new node with the given name.
     * @param name The new node's name.
     */
    Node(char *name) {
        this->name = name;
    }
    /**
     * Get the name of the node.
     * @return Returns the name of the node.
     */
    char *getName() {
        return this->name;
    }
    /**
     * Get the previous node.
     * @return Returns a pointer to the previous node.
     */
    Node<T> *getPrev() {
        return this->prev;
    }
    /**
     * Visit the node.
     */
    void visit() {
        this->visited = true;
    }
    /**
     * Unvisit the node.
     */
    void unvisit() {
        this->visited = false;
    }
    /**
     * Has it been visited?
     */
    bool isVisited() {
        return visited;
    }
    /**
     * Set previous node
     * @param prev The previous node.
     */
    void setPrev(Node<T> *n) {
        this->prev = n;
    }
    /**
     * Get all neighbours of the given node.
     * @return All of the node's neighbours.
     */
    std::vector<Node<T> *> getAllNeighbours() {
        std::vector<Node<T> *> neighs;
        for (auto edge : this->edges) {
            neighs.push_back(edge->getTo());
        }
        return neighs;
    }
    /**
     * Get all of this node's edges.
     * @return All of the node's edges.
     */
    std::vector<Edge<T> *> getAllEdges() {
        return this->edges;
    }
    /**
     * Adds a neighbouring node.
     * @param n The new neighbour.
     */
    void addNeighbour(Node<T> *n) {
        this->edges.push_back(new Edge<T>(n, 1));
    }
    /**
     * Adds a neighbouring node.
     * @param n The new neighbour.
     * @param c The cost to get to the neighbour.
     */
    void addNeighbour(Node<T> *n, T cost) {
        this->edges.push_back(new Edge<T>(n, cost));
    }
    /**
     * Does a neighbour with the given name exist?
     * @param name The neighbour's name.
     * @return Whether or not the neighbour exists.
     */
    bool isNeighbour(char *name) {
        return this->getNeighbour(name) != (Node<T> *)NULL;
    }
    /**
     * Get the neighbour with the given name (if it exists).
     * @param name The neighbour's name.
     * @return The neighbour (if it exists).
     */
    Node<T> *getNeighbour(char *name) {
        for (Edge<T> *e : this->edges) {
            if (!strcmp(name, e->getTo()->getName())) {
                return e->getTo();
            }
        }
        return (Node<T> *)NULL;
    }
    /**
     * Delete the neighbouring node with the given name.
     * @param name The name of the target node.
     */
    void delNeighbour(char *name) {
        if (!this->isNeighbour(name))
            return;
        this->getNeighbour(name)->delNeighbour(name);
        for (int i = 0; i < this->edges.size(); i++) {
            if (!strcmp(this->edges[i]->getTo()->getName(), name)) {
                this->edges.erase(this->edges.begin() + i);
                break;
            }
        }
    }
};

template <class T>
struct Edge {
   private:
    Node<T> *to;
    T cost;

   public:
    /**
         * Create a new Edge with the given parameters.
         * @param to The to node.
         * @param cost How much it costs to get from "from" to "to".
         */
    Edge(Node<T> *to, T cost) {
        this->to = to;
        this->cost = cost;
    }
    /**
     * Get the edge's cost.
     * @return The edge's cost.
     */
    T getCost() {
        return this->cost;
    }
    /**
     * Get the edge's end node.
     * @return The end node.
     */
    Node<T> *getTo() {
        return this->to;
    }
};

template <class T>
struct Graph {
   private:
    std::vector<Node<T> *> nodes;

   public:
    /**
     * Get all nodes of the graph.
     * @return A vector containing all nodes of the graph.
     */
    std::vector<Node<T> *> getAllNodes() {
        return this->nodes;
    }
    /**
     * Adds a node to the graph.
     * @param n The new node.
     */
    void addNode(Node<T> *n) {
        this->nodes.push_back(n);
    }
    /**
     * Does a node with the given name exist?
     * @param name The target node's name.
     * @return Whether or not such a node exists.
     */
    bool isNode(char *name) {
        return this->getNode(name) != (Node<T> *)NULL;
    }
    /**
     * Gets the node with the given name (if it exists).
     * @param name The name of the target node.
     * @return The target node (if it exists).
     */
    Node<T> *getNode(char *name) {
        Node<T> *node = (Node<T> *)NULL;
        for (Node<T> *n : this->nodes) {
            if (!strcmp(name, n->getName())) {
                node = n;
                break;
            }
        }
        return node;
    }
    /**
     * Does the edge exist?
     * @param from The starting point.
     * @param to The endpoint.
     * @return Whether or not the edge exists.
     */
    bool isEdge(char *from, char *to) {
        if (!this->isNode(from) || !this->isNode(to))
            return false;
        return this->getNode(from)->isNeighbour(to);
    }
    /**
     * Adds an edge to the graph.
     * @param from The starting point.
     * @param to The endpoint.
     */
    void addEdge(char *from, char *to) {
        if (!this->isNode(to) || !this->isNode(from))
            return;
        this->getNode(from)->addNeighbour(this->getNode(to));
    }
    /**
     * Adds an edge to the graph.
     * @param from The starting point.
     * @param to The endpoint.
     * @param cost The cost.
     */
    void addEdge(char *from, char *to, T cost) {
        if (!this->isNode(to) || !this->isNode(from))
            return;
        this->getNode(from)->addNeighbour(this->getNode(to), cost);
    }
    /**
     * Gets all neighbours of the target node.
     * @param name The name of the target node.
     * @return A vector containing all neighbouring nodes.
     */
    std::vector<Node<T> *> getAllNeighbours(char *name) {
        if (!this->isNode(name))
            return {};
        return this->getNode(name)->getAllNeighbours();
    }
    /**
     * Deletes the node with the given name.
     * @param name The target node's name.
     */
    void delNode(char *name) {
        if (!this->isNode(name))
            return;
        for (int i = 0; i < this->nodes.size(); i++) {
            if (!strcmp(this->nodes[i]->getName(), name)) {
                for (Node<T> *n : this->nodes[i]->getAllNeighbours())
                    n->delNeighbour(name);
                this->nodes.erase(this->nodes.begin() + i);
                break;
            }
        }
    }
    /**
     * Deletes the edge spanning from one node to another.
     * @param from The starting point.
     * @param to The endpoint.
     */
    void delEdge(char *from, char *to) {
        if (isEdge(from, to)) {
            this->getNode(from)->delNeighbour(to);
            this->getNode(to)->delNeighbour(from);
        }
    }
    /**
     * Prints the graph to the console.
     */
    void print() {
        printf(" -- GRAPH -- \n");
        printf("%d Nodes\n", (int)this->getAllNodes().size());
        for (Node<int> *n : this->getAllNodes()) {
            // print the current node ...
            printf(" Node : %s\n", n->getName());
            // print its neighbouring nodes ...
            for (auto edge : n->getAllEdges()) {
                printf("  -> %s (%.2lf)\n", edge->getTo()->getName(), (double)edge->getCost());
            }
            if (n->getAllEdges().empty()) {
                printf("  <no-edges>\n");
            }
        }
        printf("\n");
    }
    /**
     * Prints the nodes and their visited-status.
     */
    void printVisited() {
        printf(" ");
        for (auto n : this->getAllNodes()) {
            printf("| %s (%d) |", n->getName(), (int)n->isVisited());
        }
        printf("\n");
    }
    /**
     * Conduct depth-first search.
     * @param c Current node.
     * @param t Target node.
     * @param p The path.
     */
    T dfs(Node<T> *c, Node<T> *t, std::vector<Node<T> *> *p, T co) {
        p->push_back(c);
        if (c->isVisited()) return co;
        c->visit();
        if (c == t) return co;
        for (auto e : c->getAllEdges()) {
            T dco = dfs(e->getTo(), t, p, co + e->getCost());
            if (p->back() == t) return dco;
            p->pop_back();
        }
    }
    /**
     * Conduct depth-first search.
     * @param c Current node.
     * @param t Target node.
     * @param p The path.
     */
    T pr_dfs(Node<T> *c, Node<T> *t, std::vector<Node<T> *> *p, T co) {
        p->push_back(c);
        if (c->isVisited()) return co;
        printf("%s ", c->getName());
        c->visit();
        if (c == t) return co;
        for (auto e : c->getAllEdges()) {
            T dco = pr_dfs(e->getTo(), t, p, co + e->getCost());
            if (p->back() == t) return dco;
            p->pop_back();
            printf("%s ", c->getName());
        }
    }
    /**
     * Conduct depth-first search.
     * @param start The starting node.
     * @param target The target node.
     * @return The path or empty if no path exists.
     */
    std::pair<T, std::vector<Node<T> *>> defise(Node<T> *start, Node<T> *target) {
        std::vector<Node<T> *> p;
        T co = dfs(start, target, &p, 0);
        if (p.back() != target) return {};
        return {co, p};
    }
    /**
     * Conduct depth-first search.
     * @param start The starting node.
     * @param target The target node.
     * @return The path or empty if no path exists.
     */
    std::pair<T, std::vector<Node<T> *>> pr_defise(Node<T> *start, Node<T> *target) {
        std::vector<Node<T> *> p;
        T co = pr_dfs(start, target, &p, 0);
        if (p.back() != target) return {};
        printf("\n");
        return {co, p};
    }
    /**
     * Conduct Dijkstra-Search.
     * @param start The starting node.
     * @param target The target node.
     * @return The cost and path.
     */
    std::pair<T, std::vector<Node<T> *>> dijkstra(Node<T> *start, Node<T> *target) {
        std::vector<Node<T> *> p;
        std::priority_queue<std::pair<T, std::pair<Node<T> *, Node<T> *>>> q;
        q.push({(T) 0, {(Node<T> *) NULL, start}});
        std::pair<T, std::pair<Node<T> *, Node<T> *>> cu;
        while (!q.empty()) {
            cu = q.top();
            q.pop();
            cu.second.second->setPrev(cu.second.first);
            if (cu.second.second == target) break;
            for (auto e : cu.second.second->getAllEdges()) {
                // cost has to be negative, since priority queue orders descending
                // so ... lower value is higher when negative 
                if (!e->getTo()->getPrev()) q.push({cu.first-e->getCost(), {cu.second.second, e->getTo()}});
            }
        }
        if (cu.second.second != target) return {-1, {}};
        while (cu.second.second != start) {
            p.insert(p.begin(), cu.second.second);
            cu.second.second = cu.second.second->getPrev();
        }
        p.insert(p.begin(), start);
        return {-cu.first, p};
    }
    /**
     * Conduct depth-first search.
     * @param start The starting node's name.
     * @param target The target node's name.
     * @return The path or empty if no path exists.
     */
    std::pair<T, std::vector<Node<T> *>> defise(char *start, char *target) {
        if (!this->isNode(start) || !this->isNode(target))
            return {-1, {}};
        return this->defise(this->getNode(start), this->getNode(target));
    }
    /**
     * Conduct depth-first search.
     * @param start The starting node's name.
     * @param target The target node's name.
     * @return The path or empty if no path exists.
     */
    std::pair<T, std::vector<Node<T> *>> pr_defise(char *start, char *target) {
        if (!this->isNode(start) || !this->isNode(target))
            return {-1, {}};
        return this->pr_defise(this->getNode(start), this->getNode(target));
    }
    /**
     * Conduct Dijkstra-Search.
     * @param start The starting node's name.
     * @param target The target node's name.
     * @return The cost and path.
     */
    std::pair<T, std::vector<Node<T> *>> dijkstra(char* start, char* target) {
        if (!this->isNode(start) || !this->isNode(target))
            return {-1, {}};
        return this->dijkstra(this->getNode(start), this->getNode(target));
    }
    /**
     * Undo a search.
     * @param start The starting node.
     */
    void unvisit(Node<T> *start) {
        if (!start->isVisited()) return;
        start->unvisit();
        start->setPrev((Node<T> *) NULL);
        for (auto e : start->getAllEdges()) {
            this->unvisit(e->getTo());
        }
    }
    /**
     * Undo a search.
     * @param start The starting node's name.
     */
    void unvisit(char *start) {
        if (!this->isNode(start))
            return;
        this->unvisit(this->getNode(start));
    }
};

/**
 * Load a graph from a file.
 * @param fp The file.
 */
Graph<int> *loadIntGraphFromFile(FILE *fp) {
    int N, n, co;
    // read the number of nodes from the file ...
    fscanf(fp, "%d", &N);
    Graph<int> *graph = (Graph<int> *)malloc(sizeof(Graph<int>));
    // iterate over node's edges ...
    for (int i = 0; i < N; i++) {
        char *tmp = (char *)malloc(sizeof(char) * MAX_NAME_LEN);
        fscanf(fp, "%s", tmp);
        Node<int> *ntmp = graph->getNode(tmp);
        if (ntmp == (Node<int> *)NULL) {
            ntmp = new Node<int>(tmp);
            graph->addNode(ntmp);
        }
        // read the number of edges from the file ...
        fscanf(fp, "%d", &n);
        // iterate over node's edges ...
        for (int j = 0; j < n; j++) {
            tmp = (char *)malloc(sizeof(char) * MAX_NAME_LEN);
            // read the target node & edge cost ...
            fscanf(fp, "%s %d", tmp, &co);
            Node<int> *nei = graph->getNode(tmp);
            if (nei == (Node<int> *)NULL) {
                nei = new Node<int>(tmp);
                graph->addNode(nei);
            }
            if (!ntmp->isNeighbour(nei->getName())) ntmp->addNeighbour(nei, co);
            if (!nei->isNeighbour(ntmp->getName())) nei->addNeighbour(ntmp, co);
        }
    }
    return graph;
}

#endif