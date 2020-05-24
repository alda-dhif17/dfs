use super::*;
use std::hash::Hash;
use std::rc::Rc;

#[derive(Debug)]
pub struct Graph<T>
where
    T: Hash + Eq,
{
    matrix: AdjacencyMatrix<T>,
    visited: VisitedWrapper<T>,
}

impl<T> Graph<T>
where
    T: Hash + Eq + std::fmt::Debug,
{
    pub fn new() -> Self {
        Self {
            matrix: AdjacencyMatrix::new(),
            visited: VisitedWrapper::new(),
        }
    }

    /// This is just a wrapper around the contained matrix in order to
    /// not expose it to the user
    pub fn add_adjacency(&mut self, from: Rc<Node<T>>, to: Rc<Node<T>>, cost: i32) {
        self.matrix.add_adjacency(from, to, cost);
    }

    /// the public API used to execute a depth-search
    pub fn depth_search(&self, from: NodeRef<T>, to: NodeRef<T>) -> i32 {
        let contained = self.matrix.contains_node(&from);
        if contained {
            self.dfs(from, to, 0)
        } else {
            -1
        }
    }

    /// Checks whether a node has been visited yet
    fn visited(&self, node: &NodeRef<T>) -> bool {
        self.visited.get_visited().contains(node)
    }

    /// "Visits" a node by putting it in the visited HashSet
    fn visit(&self, node: NodeRef<T>) {
        self.visited.get_visited_mut().insert(node);
    }

    /// The actual DFS-implementation
    fn dfs(&self, from: NodeRef<T>, to: NodeRef<T>, mut sum: i32) -> i32 {
        // Node has already been visited --> Skip it this time 'round & don't add the cost to the
        // sum
        if self.visited(&from) || *from == *to {
            return sum;
        }

        // Visit node
        self.visit(Rc::clone(&from));

        // Get its neighbours
        let current_neighbours: &Vec<(NodeRef<T>, i32)> =
            self.matrix.get_neighbours_for_node(&from).unwrap();

        // Loop through all neighbours
        for (neighbour, cost) in current_neighbours {
            // And call dfs on them!
            sum = cost + self.dfs(Rc::clone(&neighbour), Rc::clone(&to), sum);
        }

        sum
    }
}
