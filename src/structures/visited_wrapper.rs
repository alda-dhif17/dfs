use super::NodeRef;
use std::cell::{Ref, RefCell, RefMut};
use std::collections::HashSet;
use std::hash::Hash;

#[derive(Debug)]
pub struct VisitedWrapper<T>
where
    T: Hash + Eq,
{
    /// Would panick in case I messed something up :)
    visited: RefCell<HashSet<NodeRef<T>>>,
}

impl<T> VisitedWrapper<T>
where
    T: Hash + Eq,
{
    /// Used to instantiate the struct
    pub fn new() -> Self {
        Self {
            visited: RefCell::new(HashSet::new()),
        }
    }

    /// Returns mutable reference to contained data
    pub fn get_visited_mut(&self) -> RefMut<HashSet<NodeRef<T>>> {
        self.visited.borrow_mut()
    }

    /// Returns immutable reference to contained data
    pub fn get_visited(&self) -> Ref<HashSet<NodeRef<T>>> {
        self.visited.borrow()
    }
}
