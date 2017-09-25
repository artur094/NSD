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

List of edges is the fastest, since it requires only 18 seconds to store all 33M of edges. But, it is unusable for doing a Breadth First Search (BFS) or Depth First Search (DFS) or any other type of
work on a graph. It is really fast, since it just require to store the data of the file in the same way on the memory. Some improvements can be done, for example keeping a sorted array, with a duplicate
for each edge (e.g. pair 1 4, it is better to also have the pair 4 1 in the array, so if I want to do a BFS or DFS I already know the neighbours, without searching everytime in the graph.

The Matrix is not scalable, since it has a lot of wasted space (where we don't use edges). Indeed, it is the only data structures which is killed by the kernel with code 9. All the other data structures work fine.
Moreover, just think for a 450MB graph (Actor Collaborations - 382,219 nodes), the matrix would require:
    8 Bytes (long type) * 382219^2 (#nodes) = 1.168.730.911.688 Bytes
    1.168.730.911.688 Bytes / 1024^3 = ~1089GB
Therefore, matrix can handle only small graphs, therefore not scalable.

With the Adjacency list (not contiguous), there is no waste of space like the matrix, therefore it can handle more space. Indeed, it succeeded in loading the Actor Collaborations graph without problems.
It just have some overhead due to the array of pointers (to get the array of neighbours).
The space used this time should be:
    4 Bytes (pointers) * 382,219 (#nodes) = 3.057.752 Bytes = ~2,9MB
Since an edge must be added twice, the total number of edges is doubled, then:
    8 Bytes (long) * 2 * 33,115,812 (#edges) = 529.852.992 Bytes = ~505MB
Therefore, this solution is more scalable and easier to use for BFS or DFS, since to access to the neighbours of a node requires O(1)

With the last version, adjacency list (contiguous), instead of having N different arrays (one array for node), there is just one big contiguous array, and another array which points to the first neighbour
of a specific node. This version is faster than the previous one, indeed it required more than half a minute less to load all the structure.
The reason of this improvement may be are that first of all, the array is contiguous (not sure), then also for a different management of the data structures (in the previous one I've used vectors).




