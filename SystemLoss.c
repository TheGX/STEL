#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"prototipo.c"

#define	lambda 200.0f
#define	dm 0.008f
#define	ARRIVAL 0
#define	DEPARTURE 1

int main(int argc, char const *argv[]) {
    
    if(argc != 3){
        printf("Usage ./intervalos N_Channels N_SAMPLES\n");
        return 0;
    } 
    int n_channels = atoi(argv[1]);
    int n_samples = atoi(argv[2]);
    lista  *lista_eventos = NULL;
    double delta = (0.2)*(1/lambda);
    double max_delta = 5 * (1/lambda);
	
    srand(time(0));
    int size = (max_delta/delta);
    int *histograma = (int *)malloc(size*sizeof(int));
    int max=0, bussy=0, blocked=0;
    double total_c = 0, d=0, c = 0, current_time = 0; 
	
	int i=0;
	while(i < n_samples) {
    	if(i == 0) lista_eventos = adicionar(lista_eventos, ARRIVAL, 0);

		if (lista_eventos->tipo == 1){
			bussy--;
		}else{
			i++;
			double u = ((double) rand()+1)/RAND_MAX;
    		c = -(1/lambda)*log(u);
			total_c += c;
			lista_eventos = adicionar(lista_eventos, ARRIVAL, lista_eventos->tempo + c);
			if(bussy < n_channels) { //AVAILABLE RESOURCES
				bussy++;
				current_time = lista_eventos->tempo;
				u = ((double) rand()+1)/RAND_MAX;
				d = -dm*log(u);
				lista_eventos = adicionar(lista_eventos, DEPARTURE, current_time + d);
			} else blocked++;
		}
		lista_eventos = remover(lista_eventos);
		//TODO: REMOVE THIS
		for(int z = 0; z < size; z++){
			if(c >= z*delta && c < (z+1)*delta) {
				histograma[z]++;
			} if(z == 24 && c >= (z+1)*delta )
				histograma[z]++;
			if(histograma[z] > max)
				max = histograma[z];
		}	
    }	
    total_c = total_c /(double) n_samples;
    printf("Estimador = %f\n", total_c);
    //printGraph(histograma, max);
    printf("lambda = %lf\nn_samples = %d\ndelta = %lf\nmax_delta = %lf\nHistogram size =%d\n", lambda, n_samples, delta, max_delta, size);
    printf("Blocked: %lf%%\n",(double)blocked/n_samples *100);
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



