import subprocess
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

def run_c_program():
    # Run the compiled C program and capture its output
    result = subprocess.run(["./graphblas_example_v5"], capture_output=True, text=True)
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


def plot_colored_graph(matrix, coloring):
    # Create a graph from the adjacency matrix
    G = nx.Graph(np.array(matrix))

    # Create a mapping from node indices to node names
    node_names = {i: f'$v_{{{i+1}}}$' for i in range(len(G.nodes()))}

    # Create a colormap for node colors
    cmap = plt.cm.rainbow

    # Create a figure and axis for the plot
    fig, ax = plt.subplots()

    # Plot the graph with colored nodes
    nx.draw(G, labels=node_names, node_size=1200,
            node_color=coloring, cmap=cmap, font_color='black', font_size=20, ax=ax)

    plt.title("Graph B")

    # Display the plot
    plt.show()

if __name__ == "__main__":
    # Run the compiled C program and capture the output
    c_program_output = run_c_program()

    # Parse the output to get the adjacency matrix and coloring vector
    adjacency_matrix, coloring = parse_output(c_program_output)

    # Plot the graph with coloring using NetworkX
    plot_colored_graph(adjacency_matrix, coloring)