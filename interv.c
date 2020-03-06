#include<stdio.h>
#include<stdlib.h>

// Definição da estrutura da lista
typedef struct{
	int tipo;
	double tempo;
	struct lista * proximo;
} lista;


// Função que remove o primeiro elemento da lista
lista * remover (lista * apontador)
{
	lista * lap = (lista *)apontador -> proximo;
	free(apontador);
	return lap;
}

// Função que adiciona novo elemento à lista, ordenando a mesma por tempo
lista * adicionar (lista * apontador, int n_tipo, double n_tempo)
{
	lista * lap = apontador;
	lista * ap_aux, * ap_next;
	if(apontador == NULL)
	{
		apontador = (lista *) malloc(sizeof (lista));
		apontador -> proximo = NULL;
		apontador -> tipo = n_tipo;
		apontador -> tempo = n_tempo;
		return apontador;
	}
	else
	{
		if (apontador->tempo > n_tempo) {
	        ap_aux = (lista *) malloc(sizeof (lista));
	        ap_aux -> tipo = n_tipo;
            ap_aux -> tempo = n_tempo;
            ap_aux -> proximo = (struct lista *) apontador;
            return ap_aux;
	    }

		ap_next = (lista *)apontador -> proximo;
		while(apontador != NULL)
		{
			if((ap_next == NULL) || ((ap_next -> tempo) > n_tempo))
				break;
			apontador = (lista *)apontador -> proximo;
			ap_next = (lista *)apontador -> proximo;
		}
		ap_aux = (lista *)apontador -> proximo;
		apontador -> proximo = (struct lista *) malloc(sizeof (lista));
		apontador = (lista *)apontador -> proximo;
		if(ap_aux != NULL)
			apontador -> proximo = (struct lista *)ap_aux;
		else
			apontador -> proximo = NULL;
		apontador -> tipo = n_tipo;
		apontador -> tempo = n_tempo;
		return lap;
	}
}

// Função que imprime no ecra todos os elementos da lista
void imprimir (lista * apontador)
{
	if(apontador == NULL)
		printf("Lista vazia!\n");
	else
	{
		while(apontador != NULL)
		{
			printf("Tipo=%d\tTempo=%lf\n", apontador -> tipo, apontador -> tempo);
			apontador = (lista *)apontador -> proximo;
		}
	}
}

int main(int argc, char const *argv[]) {
    
    if(argc != 3){
        printf("Usage ./intervalos LAMBDA N_SAMPLES\n");
        return 0;
    } 
    double lambda = atof(argv[1]);
    int n_samples = atoi(argv[2]);
    lista  * lista_eventos;
    int tipo_ev; 
    double tempo_ev;

    printf("lambda = %lf\nn_samples = %d\n", lambda, n_samples);
    double long u = (double) rand()/RAND_MAX;
    
    double delta = (0.2)*(1/lambda);
    double max_delta = 5 * (1/lambda);

    int max=0;
    double total_c = 0;
    
    printf("\ndelta = %lf\nmax_delta = %lf\n", delta, max_delta);
    srand(time(0));
    for(int i= 0; i<n_samples ; i++) {
    	u = ((double) rand()+1)/RAND_MAX;
    	double c = -(1/lambda)*log(u);
	adicionar(lista_eventos, 0, c);

    }	
    total_c = total_c /(double) n_samples;
    printf("Estimador = %f\n", total_c);
    return 1;
	
	printf("\nLISTA ACTUAL\n");
	imprimir(lista_eventos);

	if ( lista_eventos != NULL)
	{
		tipo_ev = lista_eventos -> tipo;
		tempo_ev = lista_eventos -> tempo;
		lista_eventos = remover(lista_eventos);
		printf("\nEVENTO REMOVIDO\n");
		printf("Tipo=%d\tTempo=%lf\n", tipo_ev, tempo_ev);
	}
	printf("\nLISTA ACTUAL\n");
	imprimir(lista_eventos);

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



