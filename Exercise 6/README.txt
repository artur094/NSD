Exercise 6 - Scalability
These are the results obtained with the several data structures:

LIST OF EDGES
    Results:
    1 - With 1,224 nodes and 19,025 edges, it required less than 0 second (Blogs)
    2 - With 382,219 nodes and 33,115,812 edges, it required 18 seconds (Actor Collaborations)

MATRIX
    Results:
    1 - With 1,224 nodes and 19,025 edges, it required less than 0 second (Blogs)
    2 - With 382,219 nodes and 33,115,812 edges, KILLED 9 (Actor Collaborations)

ADJACENCY LIST (not contiguous)
    Results:
    1 - With 1,224 nodes and 19,025 edges it required less than 0 second (Blogs)
    2 - With 382,219 nodes and 33,115,812 edges it required 137 seconds (Actor Collaborations)

ADJACENCY LIST (contiguous)
    Results:
    1 - With 1,224 nodes and 19,025 edges it required less than 0 second (Blogs)
    2 - With 382,219 nodes and 33,115,812 edges it required 110 seconds (Actor Collaborations)

List of edges is the fastest, since it requires only 18 seconds to store all 33M of edges. However, it is not optimal for doing a Breadth First Search (BFS) or Depth First Search (DFS) or any other type of work on a graph. 

It is really fast, since it just require to store the data of the file in the same way on the memory. Some improvements can be done, such as keeping a sorted array, with a duplicate for each edge (e.g. pair 1 4, it is better to also have the pair 4 1 in the array, so if I want to do a BFS or DFS you already know the neighbors without searching every time in the graph.

The Matrix is not scalable, since it has a lot of wasted space (where we don't have edges). Indeed, it is the only data structures which is killed by the kernel with code 9. All the other data structures worked fine.

If calculating the theoretical RAM memory size for a 450MB graph (Actor Collaborations - 382,219 nodes), the matrix would require:
    8 Bytes (long type) * 382219^2 (#nodes) = 1.168.730.911.688 Bytes
    1.168.730.911.688 Bytes / 1024^3 = ~1089GB
Therefore, matrix can handle only small graphs.

With the Adjacency list (not contiguous), there is no waste of space like the matrix, therefore it can handle more space. In fact, it loaded the Actor Collaborations graph without problems.

On the other hand, it has some overhead due to the array of pointers (to get the array of neighbors). The space used this time should be:
    4 Bytes (pointers) * 382,219 (#nodes) = 3.057.752 Bytes = ~2,9MB
Since an edge must be added twice (the total number of edges is doubled), then:
    8 Bytes (long) * 2 * 33,115,812 (#edges) = 529.852.992 Bytes = ~505MB

Therefore, this solution is more scalable and easier to use for BFS or DFS, since to access to the neighbors of a node requires O(1)

Finally, with the adjacency list (contiguous), instead of having N different arrays (one array per node) there is just one big contiguous array, and another array which points to the first neighbor of a specific node. 

This version is faster than the previous one, as it requires more than half a minute less to load all the structure. The reason of this improvement might be the different management of the data structures when compared to the vectors used for the previous.