def parse_connections(file_path):
    connections = {}
    
    with open(file_path, 'r') as file:
        for line in file:
            node1, node2 = line.strip().split('-')
            if node1 not in connections:
                connections[node1] = set()
            if node2 not in connections:
                connections[node2] = set()
            # Since the connection is bidirectional, add both ways
            connections[node1].add(node2)
            connections[node2].add(node1)
    
    return connections

def is_path_possible(path, connections):
    nodes = path.split()
    for i in range(len(nodes) - 1):
        if nodes[i+1] not in connections.get(nodes[i], set()):
            return False
    return True

def verify_paths(connections_file, paths_file):
    # Parse the connections
    connections = parse_connections(connections_file)
    
    # Verify each path
    with open(paths_file, 'r') as file:
        for line in file:
            path = line.strip()
            if is_path_possible(path, connections):
                print(f"Path '{path}' is possible.")
            else:
                print(f"Path '{path}' is NOT possible.")

# Example usage:
verify_paths('connections.txt', 'paths.txt')