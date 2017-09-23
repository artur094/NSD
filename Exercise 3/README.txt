Exercise 3 - Node Degree
The proposed solution is able to find the degree of each node without storing all the graph into the memory, just an array to count the degree of each node.

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The programme has been tested with 3 different graphs and results are the same by the one provided on KONECT.
The graphs that has been used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges
- Friendster with 68,349,466 nodes and 2,586,147,869 edges


Drawback:
1 - It count the copies of a link, increasing the degree of some nodes.

Requirements:
1 - there must be no copies of the same link in the file, otherwise the ndegree will be higher

Results:
1 - With 1,224 nodes and 19,025 edges it required less than 0 second (Blogs)
2 - With 382,219 nodes and 33,115,812 edges it required 46 seconds (Actor Collaborations)
3 - With 68,349,466 nodes and 2,586,147,869 edges it returns wrong number of nodes (2,906,384 nodes), anyway it required 88 seconds (Friendster)

All the results are obtained from a Macbook Pro mid 2014 with I7 (4 cores) 2,2GHz and 16GB of RAM (1600MHz DDR3)