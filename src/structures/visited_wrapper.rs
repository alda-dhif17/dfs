use super::NodeRef;
use std::cell::UnsafeCell;
use std::collections::HashSet;
use std::hash::Hash;

#[derive(Debug)]
pub struct VisitedWrapper<T>
where
    T: Hash + Eq,
{
    /// This is wrapped within an UnsafeCell
    /// in order to not require a mutable reference
    /// to self to alter it (thus avoiding cloning the neighbours)
    visited: UnsafeCell<HashSet<NodeRef<T>>>,
}

impl<T> VisitedWrapper<T>
where
    T: Hash + Eq,
{
    /// Used to instantiate the struct
    pub fn new() -> Self {
        Self {
            visited: UnsafeCell::new(HashSet::new()),
        }
    }

    /// Returns mutable reference to contained data
    pub fn get_visited_mut(&self) -> &mut HashSet<NodeRef<T>> {
        unsafe { &mut *self.visited.get() }
    }

    /// Returns immutable reference to contained data
    pub fn get_visited(&self) -> &HashSet<NodeRef<T>> {
        unsafe { &*self.visited.get() }
    }
}
