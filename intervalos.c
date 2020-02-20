#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Definição da estrutura da lista
typedef struct{
	int tipo;
	double tempo;
	struct lista * proximo;
} lista;

int main(int argc, char const *argv[]) {
    
    if(argc != 3){
        printf("Usage ./intervalos LAMBDA N_SAMPLES\n");
        return 0;
    }
    double lambda = atoi(argv[1]);
    int n_samples = atoi(argv[2]);

    srand ( time ( NULL));
    double long u = (double) rand()/RAND_MAX;
    
    // double c = log(u);
    double c = -(1-lambda)*log(u);
    // printf("lambda = %d\n n_samples = %d\n u = %Lf\n c = %Lf\n", lambda, n_samples, u, c);
    printf("lambda = %lf\n n_samples = %d\n u = %Lf\n c = %lf\n", lambda, n_samples, u, c);


    return 1;
}

