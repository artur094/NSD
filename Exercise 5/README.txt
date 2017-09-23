Exercise 4 - Three Graph Data Structures
There are 4 proposed solution:
- List of Edges, called "exercise5_listedges.cpp"
- Adjacency Matrix, called "exercise5_adjacencymatrix.cpp"
- Adjacency List done with an array of arrays, called "exercise5_adjacencylist.cpp"
- Adjacency List done with a contiguous array, called "exercise5_adjacencylist2.cpp"

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The programme has been tested with 2 different graphs and results are the same by the one provided on KONECT.
The graphs that has been used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges

Drawback:
1 - It requires to count the number of nodes, initialize a matrix, load all data and then write it again. The complexity is O(m + m + n^2) (read the file to get number nodes ( O(m) ) + fill matrix ( O(m) ) and then iterate for the whole matrix ( O(n^2) )

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
    2 - With 382,219 nodes and 33,115,812 edges it required 128 seconds (Actor Collaborations)

ADJACENCY LIST (contiguous)
    Results:
    1 - With 1,224 nodes and 19,025 edges it required less than 0 second (Blogs)
    2 - With 382,219 nodes and 33,115,812 edges it required 101 seconds (Actor Collaborations)