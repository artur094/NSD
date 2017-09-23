Exercise 2 - Size of a graph
The proposed solution is able to find the number of nodes by taking the minimum and maximum ID in the file.
In this way, it works with both files starting from ID = 0 or ID = 1 (or even different).
Then this solution consider every node between the minimum ID and maximum ID that are not in the file as a single component not connected with other nodes.
Concerning with edges, it count how many edges are in the file (how many rows).

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The programme has been tested with 3 different graphs and results are the same by the one provided on KONECT.
The graphs that has been used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges
- Friendster with 68,349,466 nodes and 2,586,147,869 edges


Drawback:
1 - any node without edges with ID less than the minimum found on the file is not considered (otherwise it would be ambiguous to understand the ID of the first node)
2 - since there is the limitation that it is not possible to load the graph onto the memory,
    then it is difficult and time wasting to search for possible copies of the same link, therefore the programme will also count duplicates.
    Therefore, there must be no duplicates in the graph file.

Requirements:
1 - there must be no copies of the same link in the file, otherwise the number of edges will be higher

Results:
1 - With 1,224 nodes and 19,025 edges it required less than 1 second (Blogs)
2 - With 382,219 nodes and 33,115,812 edges it required 17 seconds (Actor Collaborations)
3 - With 68,349,466 nodes and 2,586,147,869 edges it returns wrong number of nodes (2,906,384 nodes), anyway it required 8 seconds (Friendster)

All the results are obtained from a Macbook Pro mid 2014 with I7 (4 cores) 2,2GHz and 16GB of RAM (1600MHz DDR3)
