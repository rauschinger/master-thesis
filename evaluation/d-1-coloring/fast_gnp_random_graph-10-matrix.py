import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

# Generate a Erdős-Rényi graph
G = nx.fast_gnp_random_graph(10, 0.3, None, False)

# Save the adjacency matrix as a 2D array
adj_matrix = nx.adjacency_matrix(G)
adj_array = adj_matrix.toarray()

# Convert the adjacency matrix to the desired format
formatted_matrix = "{" + "},\n{".join([", ".join(map(str, row)) for row in adj_array]) + "}"

# Save the formatted adjacency matrix to a .txt file
with open("fast_gnp_random-10x10.txt", "w") as file:
    file.write(formatted_matrix)

# Draw the graph
nx.draw(G)
plt.show()
