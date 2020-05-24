use std::hash::Hash;

use std::rc::Rc;

pub use super::adjacency_matrix::AdjacencyMatrix;
pub use super::graph::Graph;

pub type NodeRef<T> = Rc<Node<T>>;

#[derive(Hash, Eq, PartialEq, Debug)]
pub struct Node<T>
where
    T: Hash + Eq,
{
    pub data: T,
}

impl<T> Node<T>
where
    T: Hash + Eq,
{
    fn new(data: T) -> Self {
        Self { data }
    }

    pub fn new_ref(data: T) -> Rc<Self> {
        Rc::new(Self::new(data))
    }
}
