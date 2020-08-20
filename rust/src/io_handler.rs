use crate::structures::*;
use serde::{Deserialize, Serialize};
use std::include_str;
use std::rc::Rc;

#[derive(Serialize, Deserialize)]
struct IONode {
    node: &'static str,
    neighbours: Vec<(&'static str, i32)>,
}

const CONTENT: &'static str = include_str!("../res/input.json");

pub fn read_input() -> (Graph<&'static str>, Vec<NodeRef<&'static str>>) {
    let mut graph: Graph<&'static str> = Graph::new();
    let mut all_nodes = vec![];

    let content: Vec<IONode> = serde_json::from_str(CONTENT).unwrap();

    content.into_iter().for_each(|io_node| {
        // destructure the input
        let IONode { node, neighbours } = io_node;

        // create new node
        let node_from: NodeRef<&'static str> = Node::new_ref(node);

        //add the node to all_nodes
        all_nodes.push(Rc::clone(&node_from));

        // for each neighbour
        neighbours.into_iter().for_each(|(neighbour, cost)| {
            // Search for current name in all_nodes
            let contained_key: Option<&NodeRef<&'static str>> = all_nodes
                .iter()
                .find(|value: &&NodeRef<&'static str>| value.data == neighbour);

            match contained_key {
                // Node already exists --> just clone it's reference
                Some(prev_occ) => {
                    graph.add_adjacency(Rc::clone(&node_from), Rc::clone(prev_occ), cost);
                }
                // Node doesn't exist --> create a new node with the given name
                None => {
                    // create new neighbour-node
                    let node_to: NodeRef<&'static str> = Node::new_ref(neighbour);
                    // and add it to the graph
                    graph.add_adjacency(Rc::clone(&node_from), node_to, cost);
                }
            };
        });
    });

    (graph, all_nodes)
}
