Exercise 4 - Cleaning Data
The proposed solution load the whole graph into an adjacency list with a contiguous array for the neighbours, then it writes back the whole graph into a new file (output.txt) without self loop and duplicates.

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The programme has been tested with 2 different graphs and results are the same by the one provided on KONECT.
The graphs that has been used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges

Drawback:
1 - It requires to count the number of nodes, initialize the arrays, load all data and then write it again. This is not the best way to clean data.

Results:
1 - With 1,224 nodes and 19,025 edges it required less than 1 second (Blogs)
2 - With 382,219 nodes and 33,115,812 edges it required 136 seconds (Actor Collaborations)

All the results are obtained from a Macbook Pro mid 2014 with I7 (4 cores) 2,2GHz and 16GB of RAM (1600MHz DDR3)

Below there is the README of the older version (exercise4_matrix_OLD.cpp) where I've used a matrix to load data. Sadly, the amount of memory required is really a lot and it is explained in the Exercise 6

--------------------------------------------------- OLD VERSION WITH MATRIX ----------------------------------------------

Exercise 4 - Cleaning Data
The proposed solution is able to clean data by putting everything into a matrix and then write it into a new file. In this way, all copies and self loops will be removed

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The programme has been tested with 2 different graphs and results are the same by the one provided on KONECT.
The graphs that has been used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges

Drawback:
1 - It requires to count the number of nodes, initialize a matrix, load all data and then write it again. The complexity is O(m + m + n^2) (read the file to get number nodes ( O(m) ) + fill matrix ( O(m) ) and then iterate for the whole matrix ( O(n^2) )

Results:
1 - With 1,224 nodes and 19,025 edges it required less than 0 second (Blogs)
2 - With 382,219 nodes and 33,115,812 edges it required 38 seconds (Actor Collaborations) KILLED

All the results are obtained from a Macbook Pro mid 2014 with I7 (4 cores) 2,2GHz and 16GB of RAM (1600MHz DDR3)