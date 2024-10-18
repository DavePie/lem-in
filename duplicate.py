def load_paths(file_path):
    """
    Load paths from the given file, where each path is on a new line.
    """
    with open(file_path, 'r') as file:
        paths = file.read().splitlines()
    return paths

def extract_nodes_from_path(path):
    """
    Extract individual nodes from a path string of the format 'Node1 -> Node2 -> Node3'.
    """
    return path.split(' -> ')

def find_common_nodes(paths):
    """
    Find all common nodes across the paths. Returns a set of common nodes.
    """
    seen_nodes = set()
    common_nodes = set()
    
    for path in paths:
        nodes = extract_nodes_from_path(path)
        
        for node in nodes:
            if node in seen_nodes:
                common_nodes.add(node)
            else:
                seen_nodes.add(node)
    
    return common_nodes

def main():
    file_path = 'paths2'  # Replace with your file path
    paths = load_paths(file_path)
    
    common_nodes = find_common_nodes(paths)
    
    if common_nodes:
        print("Common nodes found:", ", ".join(common_nodes))
    else:
        print("No common nodes found across all paths.")

if __name__ == "__main__":
    main()