import numpy as np
import networkx as nx
import matplotlib.pyplot as plt


def is_symmetric(matrix):
    return (matrix == matrix.T).all()

# Generate a random graph
G = nx.fast_gnp_random_graph(20, 0.3, None, True)

# Save the adjacency matrix as a 2D array
adj_matrix = nx.adjacency_matrix(G)
adj_array = adj_matrix.toarray()

# Convert adjacency matrix to the desired format
formatted_matrix = "{" + "},\n{".join([", ".join(map(str, row)) for row in adj_array]) + "}"

# Compute the transposed adjacency matrix
transposed_matrix = adj_array.T

# Convert transposed adjacency matrix to the desired format
formatted_transposed_matrix = "{" + "},\n{".join([", ".join(map(str, row)) for row in transposed_matrix]) + "}"

# Construct the combined matrix with zeros filling the remaining parts
combined_matrix = np.zeros((40, 40), dtype=int)

# Fill lower left part with the original adjacency matrix
combined_matrix[20:40, 0:20] = adj_array

# Fill upper right part with the transposed adjacency matrix
combined_matrix[0:20, 20:40] = transposed_matrix

# Check if the adjacency matrix is symmetric
if is_symmetric(np.array(combined_matrix)):
   print("Adjacency Matrix is symmetric.")
else:
   print("Adjacency Matrix is not symmetric.")

# Convert the combined matrix to the desired format
formatted_combined_matrix = "{" + "},\n{".join([", ".join(map(str, row)) for row in combined_matrix]) + "}"

# Save the formatted adjacency matrices to a file
with open("bipartite-fast_gnp_random-40x40.txt", "w") as file:
    file.write(formatted_combined_matrix)

# Draw the graph
nx.draw(G)
plt.show()