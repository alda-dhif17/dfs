use super::*;
use std::collections::HashMap;
use std::hash::Hash;
use std::rc::Rc;

#[derive(Debug)]
pub struct AdjacencyMatrix<T>
where
    T: Hash + Eq,
{
    matrix: HashMap<Rc<Node<T>>, Vec<(Rc<Node<T>>, i32)>>,
}

impl<T> AdjacencyMatrix<T>
where
    T: Hash + Eq,
{
    pub fn new() -> Self {
        Self {
            matrix: HashMap::new(),
        }
    }

    /// Used to add a new adjacency to the matrix. Automatically inserts key
    /// if it hasn't been present yet
    pub fn add_adjacency(&mut self, from: Rc<Node<T>>, to: Rc<Node<T>>, cost: i32) {
        match self.matrix.get_mut(&from) {
            // Already present --> just append it!
            Some(prev_elements) => {
                prev_elements.push((to, cost));
            }
            // Not present yet --> insert new key + list containing the new neighbour
            None => {
                self.matrix.insert(from, vec![(to, cost)]);
            }
        }
    }

    /// To check whether a node is contained (as a key in the hashmap)
    pub fn contains_node(&self, node: &NodeRef<T>) -> bool {
        self.matrix.contains_key(node)
    }

    /// Returns all neighbours for a given node, or none if it doesn't exist
    pub fn get_neighbours_for_node(&self, node: &NodeRef<T>) -> Option<&Vec<(NodeRef<T>, i32)>> {
        self.matrix.get(node)
    }
}
