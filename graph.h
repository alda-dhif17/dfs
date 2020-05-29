#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <vector>

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
        this->edges.push_back(new Edge<T>(this, n, 1));
    }
    /**
     * Adds a neighbouring node.
     * @param n The new neighbour.
     * @param c The cost to get to the neighbour.
     */
    void addNeighbour(Node<T> *n, T cost) {
        this->edges.push_back(new Edge<T>(this, n, cost));
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
    Node<T> *from;
    Node<T> *to;
    T cost;

   public:
    /**
         * Create a new Edge with the given parameters.
         * @param from The from node.
         * @param to The to node.
         * @param cost How much it costs to get from "from" to "to".
         */
    Edge(Node<T> *from, Node<T> *to, T cost) {
        this->from = from;
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
    Node<T> *getFrom() {
        return this->from;
    }
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
    void dfs(Node<T> *c, Node<T> *t, std::vector<Node<T> *> *p) {
        p->push_back(c);
        if (c->isVisited()) return;
        c->visit();
        if (c == t) return;
        for (auto e : c->getAllEdges()) {
            dfs(e->getTo(), t, p);
            if (p->back() == t) return;
            p->pop_back();
        }
    }
    /**
     * Conduct depth-first search.
     * @param start The starting node.
     * @param target The target node.
     * @return The path or empty if no path exists.
     */
    std::vector<Node<T> *> defise(Node<T> *start, Node<T> *target) {
        std::vector<Node<T> *> p;
        dfs(start, target, &p);
        if (p.back() != target) return {};
        return p;
    }
    /**
     * @param start The starting node's name.
     * @param target The target node's name.
     * @return The path or empty if no path exists.
     */
    std::vector<Node<T> *> defise(char *start, char *target) {
        if (!this->isNode(start) || !this->isNode(target))
            return {};
        return this->defise(this->getNode(start), this->getNode(target));
    }
    /**
     * Undo a search.
     * @param start The starting node.
     */
    void unvisit(Node<T>* start) {
        if (!start->isVisited()) return;
        start->unvisit();
        for (auto e: start->getAllEdges()) {
            this->unvisit(e->getTo());
        }
    }
    /**
     * Undo a search.
     * @param start The starting node's name.
     */
    void unvisit(char* start) {
        if (!this->isNode(start))
            return;
        this->unvisit(this->getNode(start));
    }
};

#endif