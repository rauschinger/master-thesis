# python wrapper for d1 coloring
# plots the colored bipartit graph


import subprocess
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

def run_c_program():
    # Run the compiled C program and capture its output
    result = subprocess.run(["./graphblas_example_v2"], capture_output=True, text=True)
    return result.stdout

def is_symmetric(matrix):
    return (matrix == matrix.T).all()

def parse_output(output):
    # Split the output into lines
    lines = output.strip().split('\n')

    # Parse the adjacency matrix, skipping empty lines
    matrix_lines = [list(map(int, line.split())) for line in lines[:-1] if line.strip()]

    # Check if the adjacency matrix is symmetric
    if is_symmetric(np.array(matrix_lines)):
        print("Adjacency Matrix is symmetric.")
    else:
        print("Adjacency Matrix is not symmetric.")

    # Print the adjacency matrix for debugging
    print("Adjacency Matrix:")
    for row in matrix_lines:
        print(row)

    # Parse the coloring vector and convert it to a 1D NumPy array
    coloring_line = lines[-1]
    coloring = np.ravel(np.array(list(map(int, coloring_line.split()))))

    # Print the shape of the coloring array for debugging
    print("Coloring Vector:")
    for color in coloring:
        print(color)

    return np.array(matrix_lines), coloring


def plot_bipartite_graph(matrix, coloring):
    # Create a bipartite graph from the adjacency matrix
    G = nx.Graph(np.array(matrix))

    # Get the sets of nodes based on bipartite structure
    left_nodes, right_nodes = nx.bipartite.sets(G)

    # Create a bipartite layout
    pos = nx.bipartite_layout(G, left_nodes)

    # Create a figure and axis for the plot
    fig, ax = plt.subplots()

    # Plot the bipartite graph with colored nodes
    nx.draw(G, pos, with_labels=True, font_weight='bold', node_size=700,
            node_color=coloring, cmap=plt.cm.rainbow, font_color='black', font_size=8, ax=ax)

    plt.title("Bipartite Graph")

    # Display the plot
    plt.show()



if __name__ == "__main__":
    # Step 1: Run the compiled C program and capture the output
    c_program_output = run_c_program()

    # Step 2: Parse the output to get the adjacency matrix and coloring vector
    adjacency_matrix, coloring = parse_output(c_program_output)

    # Step 3: Plot the bipartite graph with coloring using NetworkX
    plot_bipartite_graph(adjacency_matrix, coloring)
