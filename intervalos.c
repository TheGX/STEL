#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


void printGraph(int histograma[25], int max);
int main(int argc, char const *argv[]) {
    
    if(argc != 3){
        printf("Usage ./intervalos LAMBDA N_SAMPLES\n");
        return 0;
    } 
    double lambda = atof(argv[1]);
    int n_samples = atoi(argv[2]);

    printf("lambda = %lf\nn_samples = %d\n", lambda, n_samples);
    double long u = (double) rand()/RAND_MAX;
    
    double delta = (0.2)*(1/lambda);
    double max_delta = 5 * (1/lambda);
    int histograma[25]={};
    int max=0;
    double total_c = 0;
    
    printf("\ndelta = %lf\nmax_delta = %lf\n", delta, max_delta);
    srand(time(0));
    for(int i= 0; i<n_samples ; i++) {
    	u = ((double) rand()+1)/RAND_MAX;
    	double c = -(1/lambda)*log(u);
	total_c += c ;
	for(int z = 0; z < 25; z++){
		if(c >= z*delta && c < (z+1)*delta) {
			histograma[z]++;
		} if(z == 24 && c >= (z+1)*delta )
			histograma[z]++;
		if(histograma[z] > max)
			max = histograma[z]; 
	}	
    }	
    total_c = total_c /(double) n_samples;
    printGraph(histograma, max);
    printf("Estimador = %f\n", total_c);
    return 1;
}
	

void printGraph(int histograma[25], int max){
    for(int height = max; height > 0; height--){
	printf("%02d ", height);
    	for(int z = 0; z < 25; z++){
		if(histograma[z] >= height)
			printf(" = ");
		else printf("   ");
   	}
	printf("\n");
    }

    printf("x: ");	
    for(int z = 0; z < 25; z++){
	printf("%02d ", z);
    }
    printf("\n");

}

