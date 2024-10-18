def load_paths(file_path):
    """
    Load paths from the given file, where each path is on a new line.
    """
    with open(file_path, 'r') as file:
        paths = file.read().splitlines()
    return paths

def load_edges(file_path):
    """
    Load edges from a file, where each edge is on a new line in the format 'Node1-Node2'.
    """
    with open(file_path, 'r') as file:
        edges = file.read().splitlines()

    # Create a set of bidirectional edges
    edge_set = set()
    for edge in edges:
        node1, node2 = edge.split('-')
        edge_set.add((node1, node2))
        edge_set.add((node2, node1))  # Bidirectional, so we add both directions
    
    return edge_set

def extract_nodes_from_path(path):
    """
    Extract individual nodes from a path string of the format 'Node1 -> Node2 -> Node3'.
    """
    return path.split(' -> ')

def verify_paths_against_edges(paths, edge_set):
    """
    Verify that each path uses valid edges from the edge_set.
    Prints invalid connections if found.
    """
    for path in paths:
        nodes = extract_nodes_from_path(path)
        for i in range(len(nodes) - 1):
            node1 = nodes[i]
            node2 = nodes[i + 1]
            if (node1, node2) not in edge_set:
                print("Invalid edge")
                return False
    
    return True

def main():
    path_file = 'paths'  # Replace with your path file
    edge_file = 'edges'  # Replace with your edge file

    paths = load_paths(path_file)
    edge_set = load_edges(edge_file)

    if verify_paths_against_edges(paths, edge_set):
        print("All paths use valid edges.")
    else:
        print("Some paths use invalid edges.")

if __name__ == "__main__":
    main()