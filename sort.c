#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <omp.h>

void swap(long* a, long* b) 
{ 
    long t = *a; 
    *a = *b; 
    *b = t; 
} 
  
 //always last element of the given array is picked as a pivot
 //after calling partition, elements before the returned index is less than pivot, and after are greater than pivot 
long partition (long arr[], long low, long high) 
{ 
    long pivot = arr[high];    // pivot 
    long i = (low - 1);  // Index of smaller element 
  
    for (long j = low; j <= high- 1; j++) 
    { 
    	//intra loop dependency here
        if (arr[j] < pivot) // If current element is smaller than the pivot 
        { 
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 
  
void quickSort(long arr[], long low, long high) 
{ 
    if (low < high) 
    {      
       long pi = partition(arr, low, high); // pi is partitioning index, arr[p] is now at right place

       quickSort(arr, low, pi - 1); 
       quickSort(arr, pi + 1, high); 

       // #pragma omp parallel sections if(high-low>10)
     //   #pragma omp parallel sections
     //   {
     //   	#pragma omp section
	    //     quickSort(arr, low, pi - 1); 
     //    #pragma omp section
     //    	quickSort(arr, pi + 1, high); 
    	// }
    } 
} 

void main(int argc, char *argv[]){
	long n = 0;
	char c;
	FILE *fp;

	clock_t start, end;
 	double cpu_time_used;


	fp = fopen("random.txt","r");
	//counting number of lines present (and hence no of numbers)
	if(fp!=NULL){
		for (c = getc(fp); c != EOF; c = getc(fp)){
        	if (c == '\n')
            	n = n + 1; 
    	}
	}
	fclose(fp);


	long array[n+1];
	fp = fopen("random.txt","r");

	//scanning numbers and storing them in a array
	if(fp!=NULL){
		for(long i=0;i<n;i++){
			fscanf(fp,"%ld",&array[i]);
		}
	}
	fclose(fp);

	int t;
 	printf ("Ennter the number of threads: ");
	scanf("%d", &t);
	omp_set_num_threads(t);

	start = clock();

	//printing inital unsorted array
	// printf("Initial array: \n");
	// for(long i =0;i<n;i++){
	// 	printf("%ld ", array[i]);
	// }
	// printf("\n\n");

	long local_n = n/t;
	int myid;
	long my_first[t],my_last[t];


	#pragma omp parallel private(myid)
	{
		myid = omp_get_thread_num();
		my_first[myid] = myid * local_n;
		my_last[myid] = my_first[myid]+local_n-1;
		quickSort(array,my_first[myid],my_last[myid]);
	}

	long remaining_first = local_n*t;
	long remaining_length = n-remaining_first;

	quickSort(array,remaining_first,n-1);

	long array2[n];
	long iterators[t+1];

	for(int i=0;i<t;i++)
		iterators[i]=my_first[i];
	iterators[t] = remaining_first;

	long count = 0;
	while(count<n){
		long minVal = 99999999,procNo = 0;
		for(int i=0;i<t;i++){
			if(iterators[i]<=my_last[i] && minVal>=array[iterators[i]]){
				minVal = array[iterators[i]];
				procNo = i;
			}
		}
		if(iterators[t]<n && array[iterators[t]]<minVal){
			array2[count] = array[iterators[t]];
			iterators[t]++;
		}
		else{
			array2[count] = minVal;
			iterators[procNo]++;
		}
		count++;
	}

	//printing the sorted array
	// printf("Sorted array: \n");
	// for(long i =0;i<n;i++){
	// 	printf("%ld ", array2[i]);
	// }
	// printf("\n\n");

	//writing the sorted array in a file named sorted.txt
	fp = fopen("sorted.txt","w+");
	for(long i=0;i<n;i++){
		fprintf(fp, "%ld\n", array2[i]);
	}
	fclose(fp);

	printf("done\n");

	end = clock();
 	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Time taken = %f\n",cpu_time_used);
}
