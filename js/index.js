const fs = require("fs");

/**
 *
 * Ein WunderWERK von:
 *
 * Adrian Berner, Timo Donnelly, Denis Imeri, Elias Wilfinger
 *
 */

/**
 * Klasse zum managen von den Knoten
 */
class Graph {
  /**
   * @returns {Knoten[]} die Knoten
   */
  get nodes() {
    return this._nodes;
  }

  set nodes(nodes) {
    this._nodes = nodes;
  }

  constructor() {
    const nodeMap = {};
    const nodes = [];
    const input = fs.readFileSync("in.txt", "utf8").split("\r\n");

    // go through every row of the list
    // check if this node has already been a neighbor

    for (let nodeRow of input) {
      let edges = [];
      let neighbors = [];
      const split = nodeRow.split(" ");
      let node = nodeMap[split[0]];
      if (!node) {
        node = new Knoten(split[0], []);
        nodes.push(node);
      }

      // go through every neighbor
      for (let neighborName of split.slice(1)) {
        // check if the neighbor was already found somewhere else (already exists in the nodes)
        let neighbor = nodeMap[neighborName];
        if (!neighbor) {
          // if it does not exist, create it and add it to the nodes
          neighbor = new Knoten(neighborName, []);
          nodes.push(neighbor);
        }
        edges.push(new Edge(node, neighbor, 0));
        neighbors.push(neighbor);
        nodeMap[neighborName] = neighbor;
      }

      // set the neighbors and copy the array (because of references)
      node.edges = [...edges];
      node.neighbors = [...neighbors];
      nodeMap[split[0]] = node;
    }
    this._nodes = nodes;
  }

  /**
   * gibt alle knoten in string repräsentation zurück
   */
  getAlleKnoten() {
    let knoten = [];

    for (let n of this.nodes) {
      let str = `${n.name}: `;
      if (n.neighbors.length > 0) {
        for (let x of n.neighbors) {
          str += x.name + ", ";
        }
      } else {
        str += " ";
      }
      knoten.push(str.substr(0, str.length - 2));
    }

    return knoten;
  }

  /**
   * gibt die Nachbarn für einen Knoten zurück
   * @param {string} knotenName name des Knoten
   */
  getAlleNachbarn(knotenName) {
    let list = [];
    for (let n of this.nodes)
      if (n.name == knotenName) for (let x of n.neighbors) list.push(x.name);

    return list;
  }

  /**
   * fügt einen Knoten hinzu
   * @param {string} knotenName name vom Knoten
   */
  addKnoten(knotenName) {
    this.nodes.push(new Knoten(knotenName, []));
  }

  /**
   * löscht einen Knoten und löscht alle Kanten, die auf diesen Knoten zeigen
   * @param {string} knotenName name vom Knoten
   */
  delKnoten(knotenName) {
    for (let i in this.nodes) {
      if (this.nodes[i].name == knotenName) {
        this.nodes.splice(i, 1);
        // löscht knoten

        // löscht Kanten, die AUF diesen Knoten zeigen
        for (let n of this.nodes) {
          for (let neighbor of n.neighbors) {
            if (neighbor.name == knotenName) {
              this.delKante(n.name, knotenName);
            }
          }
        }
        return;
      }
    }
  }

  /**
   * fügt eine Kante hinzu
   * @param {string} startName der Name des Anfangsknoten
   * @param {string} endName der Name des Endknoten
   */
  addKante(startName, endName) {
    for (let node of this.nodes) {
      if (node.name == startName) {
        for (let end of this.nodes) {
          if (end.name == endName) {
            node.neighbors.push(end);
            return;
          }
        }
      }
    }
  }

  /**
   * entfernt eine kante
   * @param {string} startName der Name vom Anfangsknoten
   * @param {string} endName der Name vom Endknoten
   */
  delKante(startName, endName) {
    for (let node of this.nodes) {
      if (node.name == startName) {
        for (let i in node.neighbors) {
          if (node.neighbors[i].name == endName) {
            node.neighbors.splice(i, 1);
            return;
          }
        }
      }
    }
  }

  /**
   * schaut ob diese Kante existiert
   * @param {string} startName der Name des Anfangsknoten
   * @param {string} endName der Name des Endknoten
   */
  existiertKante(startName, endName) {
    for (let node of this.nodes)
      if (node.name == startName) {
        for (let n of node.neighbors) if (n.name == endName) return true;
      }

    return false;
  }

  /**
   * schaut ob es diesen Knoten gibt
   * @param {string} kantenName Name des Knoten
   */
  existiertKnoten(kantenName) {
    for (let n of this.nodes) if (n.name == kantenName) return true;

    return false;
  }

  /**
   * actually searches
   * @param {Node} currentNode
   * @param {Node} targetNode
   * @param {Node[]} path the path until now
   */
  tiefensuche(currentNode, targetNode, path) {
    path.push(currentNode);
    if (currentNode.visited) {
      return;
    }
    // sets visited to true
    currentNode.visited = true;

    // if it's already the target
    if (currentNode == targetNode) {
      return;
    }

    // go through its edges
    for (let edge of currentNode.edges) {
      this.tiefensuche(edge.to, targetNode, path);
      if (path[path.length - 1] == targetNode) {
        return;
      }
      // remove the last element
      path.splice(path.length - 1, 1);
    }
  }

  /**
   * starts the Tiefensuche
   * @param {Node} start
   * @param {Node} end
   */
  startTiefenSuche(start, end) {
    const path = [];
    this.tiefensuche(start, end, path);
    if (path[path.length - 1] != end) {
      // return an empty array because no path was found
      return [];
    }

    // otherwise return the path
    return path;
  }

  /**
   * holt sich einen Knoten mit einem Namen
   * @param {string} name der Name nach dem gesucht werden soll
   */
  getKnoten(name) {
    for (let node of this.nodes) {
      if (node.name == name) {
        return node;
      }
    }
    return undefined;
  }
}

/**
 * Datenklasse für den Knoten
 */
class Knoten {
  /**
   * @return {Edge[]}
   */
  get edges() {
    return this._edges;
  }

  /**
   * @param {Edge[]} edges die Edges
   */
  set edges(edges) {
    this._edges = edges;
  }
  /**
   * Der Konstruktor für die Knoten Klasse
   * @param {string} name der Name vom Knoten
   */
  constructor(name) {
    this.name = name;
    this.visited = false;
    this._edges = [];
  }
}

class Edge {
  /**
   * @param {Knoten} from von wo es geht
   * @param {Knoten} to wohin es geht
   * @param {number} cost wie viel die Verbindung kostet
   */
  constructor(from, to, cost) {
    this.from = from;
    this.to = to;
    this.cost = cost;
  }
}

/**
 * eine wunderbare Funktion, die alle Knoten ausgibt
 * @param {string[]} knoten liste die die verbindungen repräsentiert
 */
function output(knoten) {
  knoten.forEach((knot) => {
    console.log(knot);
  });
}

/**
 * in diese hübschen funktion werden alle knoten namen ausgegeben vom path
 * @param {Knoten[]} path knoten zum ausgeben
 */
function outputPath(path) {
  path.forEach((knot) => {
    console.log(knot.name);
  });
}

const graph = new Graph();
console.log("Anfangswerte");
const knoten = graph.getAlleKnoten();
output(knoten);

console.log();
console.log("TIEFENSUCHE");

let start = graph.getKnoten("D");
let end = graph.getKnoten("C");
let path = graph.startTiefenSuche(start, end);
if (path.length > 0) {
  console.log("es gab einen richtigen weg! JUHU!");
  outputPath(path);
} else {
  console.log("es gab keinen WEG! !JUHU");
}

console.log();
console.log("Tiefensuche von einem nicht möglichen weg");
start = graph.getKnoten("D");
end = graph.getKnoten("E");
path = graph.startTiefenSuche(start, end);
if (path.length > 0) {
  console.log("es gab einen richtigen weg! JUHU!");
  outputPath(path);
} else {
  console.log("es gab keinen WEG! !JUHU");
}

// console.log("Lösche Kante B -> C");
// graph.delKante("B", "C");
// output(graph.getAlleKnoten());

// console.log("Lösche Kante C -> B");
// graph.delKante("C", "B");
// output(graph.getAlleKnoten());

// console.log("Lösche Kante C -> C");
// graph.addKante("C", "C");
// output(graph.getAlleKnoten());

// console.log("Lösche Knoten B");
// graph.delKnoten("B");
// output(graph.getAlleKnoten());

// console.log("Lösche Knoten C");
// graph.delKnoten("C");
// output(graph.getAlleKnoten());
