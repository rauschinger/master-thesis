import networkx as nx
import matplotlib.pyplot as plt
import subprocess

def parse_output(output):
    lines = output.split('\n')
    
    # Parse adjacency matrix
    adj_matrix = [list(map(int, line.split())) for line in lines[:7]]

    # Parse node colors
    node_colors = list(map(int, lines[8].split()))

    return adj_matrix, node_colors

def plot_colored_graph(adj_matrix, node_colors, pos=None):
    G = nx.Graph()

    # Add nodes with labels v_1 to v_7
    for i in range(1, 8):
        G.add_node(i, label=f'$v_{i}$')

    # Add edges based on adjacency matrix
    for i in range(len(adj_matrix)):
        for j in range(len(adj_matrix[i])):
            if adj_matrix[i][j] == 1:
                G.add_edge(i + 1, j + 1)

    # Define custom colors
    colors = ['Cyan', 'yellow', 'green', 'orange', 'red']  # Add more colors as needed

    # Map node colors to actual colors
    node_colors = [colors[color_index] for color_index in node_colors]

    # Plot the graph with node colors
    if pos is None:
        pos = nx.circular_layout(G)  # Default layout if pos is not provided

    plt.title("Graph A")

    nx.draw(G, pos, with_labels=True, labels=nx.get_node_attributes(G, 'label'),
            node_color=node_colors, font_color='black', font_weight='bold', node_size=1200, font_size=20)

    plt.show()

def main():
    # Run the C program and get the output
    result = subprocess.run(["./graphblas_example_v4"], capture_output=True, text=True)
    output = result.stdout

    # Parse the output
    adj_matrix, node_colors = parse_output(output)

    # Example: Specify custom node positions (replace with your own positions)
    custom_positions = {1: (0, 2), 2: (0, 1), 3: (0, 0), 4: (1, 2), 5: (1, 1), 6: (1, 0), 7: (2, 1)}

    # Plot the colored graph with custom node positions
    plot_colored_graph(adj_matrix, node_colors, pos=custom_positions)

if __name__ == "__main__":
    main()
