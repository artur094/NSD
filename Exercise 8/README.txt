Exercise 8 - Degree Distribution
The proposed program uses the same methods as in exercise 3. 
In this case, instead of writing on the standard output, it writes the distribution on the file stream.

It is also possible to specify the file name as argument of the main:
    ./a.out <file_name>

The program has been tested with 2 different graphs and all the results obtained are the same as the one obtained from KONECT.

The graphs used are:
- Blogs with 1,224 nodes and 19,025 edges
- Actor Collaborations with 382,219 nodes and 33,115,812 edges


Results
1 - With 1,224 nodes and 19,025 edges it required less than 0 second (Blogs)
2 - With 382,219 nodes and 33,115,812 edges it required 36 seconds (Actor Collaborations)

All the results are obtained from a Macbook Pro mid 2014 with I7 (4 cores) 2,2GHz and 16GB of RAM (1600MHz DDR3)