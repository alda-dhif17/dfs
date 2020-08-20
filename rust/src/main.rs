mod io_handler;
mod structures;
use crate::io_handler::read_input;
use std::rc::Rc;

fn main() {
    let (graph, nodes) = read_input();
    println!("{:?}", graph);

    let result: i32 = graph.depth_search(Rc::clone(&nodes[0]), Rc::clone(&nodes[5]));
    println!("The result is: {}!", result);
}
