Exercise 2 - Size of a graph

The proposed solution is able to find the number of nodes by taking the minimum and maximum ID in the file.
In this way, it works with any file with IDs starting from 0, 1, or any other positive entire number.

This solution considers every node between the minimum ID and maximum ID that are linked to other nodes within the file. 
Regarding the edges, it counts them by obtaining the number of rows in the file.

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The program has been tested with 3 different graphs and all the results obtained are the same as the one obtained from KONECT.

The graphs used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges
- Friendster with 68,349,466 nodes and 2,586,147,869 edges


Drawbacks:
1 - any node without edges with smaller ID than the minimum found in the file is not considered.
2 - since it is not possible to store the graph the memory, the program does not discard duplicated links. 
Therefore, to properly count there must be no duplicates in the graph file.

Requirements:
1 - the file must contain ONLY non-duplicated links.

Results:
1 - With 1,224 nodes and 19,025 edges it required less than 1 second (Blogs)
2 - With 382,219 nodes and 33,115,812 edges it required 17 seconds (Actor Collaborations)
3 - With 68,349,466 nodes and 2,586,147,869 edges it returns wrong number of nodes (2,906,384 nodes), anyway it required 8 seconds (Friendster)

All the results are obtained from a Macbook Pro mid 2014 with I7 (4 cores) 2,2GHz and 16GB of RAM (1600MHz DDR3)
