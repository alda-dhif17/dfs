use super::*;
use std::cell::UnsafeCell;
use std::collections::HashSet;
use std::hash::Hash;
use std::rc::Rc;

#[derive(Debug)]
pub struct Graph<T>
where
    T: Hash + Eq,
{
    matrix: AdjacencyMatrix<T>,

    /// This is wrapped within an UnsafeCell
    /// in order to not require a mutable reference
    /// to self to alter it (thus avoiding cloning the neighbours)
    visited: UnsafeCell<HashSet<Rc<Node<T>>>>,
}

impl<T> Graph<T>
where
    T: Hash + Eq + std::fmt::Debug,
{
    pub fn new() -> Self {
        Self {
            matrix: AdjacencyMatrix::new(),
            visited: UnsafeCell::new(HashSet::new()),
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
        self.get_visited().contains(node)
    }

    /// "Visits" a node by putting it in the visited HashSet
    fn visit(&self, node: NodeRef<T>) {
        self.get_visited().insert(node);
    }

    /// Used to keep the code DRY
    fn get_visited(&self) -> &mut HashSet<NodeRef<T>> {
        unsafe { &mut *self.visited.get() }
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
