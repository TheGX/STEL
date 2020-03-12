#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <time.h>

#define delta_t 0.001

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



//define se houve um arrival ou não
int arrivals(int lambda){

    double p = lambda*delta_t;
    double i = (double) (rand()+1)/RAND_MAX;

    if(i<p) return 1; // houve arrival
    else return 0;

} 
// Pequeno exemplo de utilização
int main(int argc, char const *argv[])
{
	lista  * lista_eventos;
	int tipo_ev; double tempo_ev;
	lista_eventos = NULL;
    int tempo_total=0;
    double lambda = atof(argv[1]);
    int n_samples = atoi(argv[2]);
    double u=0;
    int aux=0;
    int count=0;

    //histogram stuff    
    double delta = (0.2)*(1/lambda);
    double max_delta = 5 * (1/lambda);
    int histograma[25]={};
    int max=0;
    double total_count = 0;
    
    if(argc != 3){
        printf("Usage ./intervalos LAMBDA N_SAMPLES\n");
        return 0;
    } 
    
    printf("lambda = %lf\nn_samples = %d\n", lambda, n_samples);
    printf("\ndelta = %lf\nmax_delta = %lf\n", delta, max_delta);

    srand(time(0));


    for(int i= 0; i<n_samples ; i++) {

        if(lista_eventos != NULL){
            
            lista_eventos=remover(lista_eventos);
        }

        aux=0;
        count=0;

        while( aux == 0){ // enquanto nao houver chamada passa-se para o time-slot seguinte

            aux=arrivals(lambda);
            count++;
           
        }

        total_count=total_count+count*delta_t;

        for(int z = 0; z < 25; z++){
		    if(count*delta_t >= z*delta && count*delta_t < (z+1)*delta) {
			    histograma[z]++;
		    } if(z == 24 && count*delta_t >= (z+1)*delta )
			histograma[z]++;
		    if(histograma[z] > max) max = histograma[z]; 
	    }	

        lista_eventos=adicionar(lista_eventos, 0, delta_t*count);
        imprimir(lista_eventos);

    }

    printGraph(histograma,max);
    printf("Estimador de 1/lambda: %f\n",   total_count = total_count /(double) n_samples);
	/*lista_eventos = adicionar(lista_eventos, 1, 0.6);
	lista_eventos = adicionar(lista_eventos, 0, 0.4);
	lista_eventos = adicionar(lista_eventos, 1, 0.3);
	lista_eventos = adicionar(lista_eventos, 2, 0.5);
	lista_eventos = adicionar(lista_eventos, 1, 0.5);
	lista_eventos = adicionar(lista_eventos, 0, 0.2);
	lista_eventos = adicionar(lista_eventos, 1, 0.1);
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

	lista_eventos = adicionar(lista_eventos, 2, 0.5);
	lista_eventos = adicionar(lista_eventos, 1, 0.3);
	printf("\nLISTA ACTUAL\n");
	imprimir(lista_eventos);*/

    return 0;
}

