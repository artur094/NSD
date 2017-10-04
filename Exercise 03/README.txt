Exercise 3 - Node Degree

The proposed solution is able to find the degree of each node without storing all the graph into the memory.
It works just with an array storing the degree of every node.

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The program has been tested with 3 different graphs and all the results obtained are the same as the one obtained from KONECT.

The graphs used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges
- Friendster with 68,349,466 nodes and 2,586,147,869 edges


Drawbacks:
1 - It counts several times the duplicated links, increasing the real degree of some nodes.

Requirements:
1 - there must be no copies of the same link in the file.

Results:
1 - With 1,224 nodes and 19,025 edges it required less than 0 second (Blogs)
2 - With 382,219 nodes and 33,115,812 edges it required 46 seconds (Actor Collaborations)
3 - With 68,349,466 nodes and 2,586,147,869 edges it returns wrong number of nodes (2,906,384 nodes), anyway it required 88 seconds (Friendster)

All the results are obtained from a Macbook Pro mid 2014 with I7 (4 cores) 2,2GHz and 16GB of RAM (1600MHz DDR3)