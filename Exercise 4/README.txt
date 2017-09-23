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