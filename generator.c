#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h> 

void main(){
	int n = 10;
	clock_t start, end;
 	double cpu_time_used;

 	int t;
 	printf ("Ennter the number of threads: ");
	scanf("%d", &t);
	omp_set_num_threads(t);

	// int p;
	// p = omp_get_num_procs();

	//taking input from user
	printf("enter number of random numbers to generate: ");
	scanf("%d",&n);

	start = clock();

	FILE *fp;
	fp = fopen("random.txt","w+");

	//generating a random seed, so that each time different random numbers are generated
	srand(time(0));

	//generating and printing n random numbers between 0 to 999, this is done so that we can analyse result easily
	//no loop level dependency here.. can run parallely
	#pragma omp parallel for
	for(int i=0;i<n;i++){
		// t = omp_get_num_threads();
		// printf("in thread no: %d\n", omp_get_thread_num());
		#pragma omp critical
		{
			fprintf(fp, "%d\n", rand()%10000);
		}
	}
	fclose(fp);

	printf("Done!\n");
	
	end = clock();
 	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	// printf("Prcs available = %d : Number of Threads = %d : CPU time = %f\n", p, t, cpu_time_used);	
	// printf("Prcs available = %d : CPU time = %f\n", p, cpu_time_used);	
	printf("Time taken = %f\n",cpu_time_used);
}