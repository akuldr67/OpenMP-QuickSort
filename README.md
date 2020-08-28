# OpenMP-QuickSort
To Sort given numbers by quick sort and parallelizing it by OpenMP.
generator.c is used to generate N random numbers and write them in file random.txt. N is asked from user. 
Code is mainly parallelized in sort.c. Just for the sake of similarity, generator.c is also converted to a OpenMP code.   
To compile:
```
gcc -fopenmp -o generator generator.c
```
To run:
```
./generator
```
When the code runs, it asks for number of threads (n). 
sort.c reads the data from random.txt and sorts it and writes back the sorted data in sorted.txt. Best results will come when n = 3 or 4 is given while running sort.c  
To compile:
```
gcc -fopenmp -o sort sort.c
```
To run
```
./sort
```
