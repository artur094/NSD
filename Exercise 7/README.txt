Exercise 7 - Number of Isolated Nodes
The proposed solution load the file more times: the first to get the number of nodes and edges, then the second to compute the degree of each node.
Then, it iterates with the array to compute all required data.

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The programme has been tested with 2 different graphs and results are the same by the one provided on KONECT.
The graphs that has been used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges

Results
1 - With 1,224 nodes and 19,025 edges it required less than 0 second (Blogs)
2 - With 382,219 nodes and 33,115,812 edges it required 37 seconds (Actor Collaborations)

All the results are obtained from a Macbook Pro mid 2014 with I7 (4 cores) 2,2GHz and 16GB of RAM (1600MHz DDR3)