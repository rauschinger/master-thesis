import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

# Generate a random graph
G = nx.fast_gnp_random_graph(80, 0.3, None, False)

# Save the adjacency matrix as a 2D array
adj_matrix = nx.adjacency_matrix(G)
adj_array = adj_matrix.toarray()

# Convert adjacency matrix to the desired format
formatted_matrix = "{" + "},\n{".join([", ".join(map(str, row)) for row in adj_array]) + "}"

# Save the formatted adjacency matrix to a file
with open("fast_gnp_random-80x80.txt", "w") as file:
    file.write(formatted_matrix)

# Draw the graph
nx.draw(G)
plt.show()