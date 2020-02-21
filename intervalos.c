#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int main(int argc, char const *argv[]) {
    
    if(argc != 3){
        printf("Usage ./intervalos LAMBDA N_SAMPLES\n");
        return 0;
    } 
    double lambda = atof(argv[1]);
    int n_samples = atoi(argv[2]);

    printf("lambda = %lf\n n_samples = %d\n", lambda, n_samples);
    srand(time(0));
    double long u = (double) rand()/RAND_MAX;
    
    double c = -(1/lambda)*log(u);
	
    double delta = (0.2)*(1/lambda);
    double max_delta = 5 * (1/lambda);
    int histograma[25]={};
	printf("delta = %lf\nmax_delta = %lf\n", delta, max_delta);
    for(int i= 0; i<n_samples ; i++) {
    	u = (double) rand()/RAND_MAX;
    	double c = -(1/lambda)*log(u);
	for(int z = 0; z < 25; z++){
			//printf("z = %d, z*delta = %d \n", z ,z*delta);
		if(c >= z*delta && c < (z+1)*delta) {
			histograma[z]++;
		}
	}	
    }
	for(int z = 0; z < 25; z++){
   	 printf("histograma[%d] = %d \n", z, histograma[z]);
	}

    return 1;
}

