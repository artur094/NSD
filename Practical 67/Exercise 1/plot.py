import networkx as nx
import numpy as np
import matplotlib.pyplot as plt

#graph_file = open("output.dat", "r")
graph_file = open("../../graph01.txt", "r")
graph = []

for line in graph_file:
    x = line.split(' ')[0]
    y = line.split(' ')[1]
    graph.append({x, y})


G = nx.Graph()
G.add_edges_from(graph)


nx.draw(G, with_labels=True, font_weight='bold')
plt.show()