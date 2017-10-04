Exercise 4 - Cleaning Data
The proposed solution loads the entire graph into an adjacency list with a contiguous array for the neighbors. 
Then, it writes back the graph into a new file (output.txt) without self loop and duplicates.

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The program has been tested with 2 different graphs and all the results obtained are the same as the one obtained from KONECT.

The graphs used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges

Drawbacks:
1 - It requires to count the number of nodes, initialize the arrays, load all data and then write it again. This is not the optimal way to clean data.

Results:
1 - With 1,224 nodes and 19,025 edges it required less than 1 second (Blogs)
2 - With 382,219 nodes and 33,115,812 edges it required 136 seconds (Actor Collaborations)

All the results are obtained from a Macbook Pro mid 2014 with I7 (4 cores) 2,2GHz and 16GB of RAM (1600MHz DDR3)


Below you can find the README from an older version of this exercise (exercise4_matrix_OLD.cpp) where a matrix is to load data. 
Sadly, it required a big amount of memory as explained later (in the Exercise 6).

--------------------------------------------------- OLD VERSION WITH MATRIX ----------------------------------------------

Exercise 4 - Cleaning Data
The proposed solution is able to clean the file by storing the data into a matrix and then write it into a new file. 
In this way, all copies and self loops will be removed.

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The program has been tested with 2 different graphs and all the results obtained are the same as the one obtained from KONECT.

The graphs used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges

Drawbacks:
1 - It requires to count the number of nodes, initialize a matrix, load all data and then write it again. The complexity is O(m + m + n^2) (read the file to get number nodes ( O(m) ) + fill matrix ( O(m) ) and then iterate for the whole matrix ( O(n^2) )

Results:
1 - With 1,224 nodes and 19,025 edges it required less than 0 second (Blogs)
2 - With 382,219 nodes and 33,115,812 edges it required 38 seconds (Actor Collaborations) KILLED

All the results are obtained from a Macbook Pro mid 2014 with I7 (4 cores) 2,2GHz and 16GB of RAM (1600MHz DDR3)