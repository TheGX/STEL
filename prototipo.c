#include<stdio.h>
#include<stdlib.h>

#define	lambda 0.022f
#define	dm 120.0f
#define	ARRIVAL 0
#define	DEPARTURE 1
#define	SPECIFIC 0
#define	GENERAL 1
#define st_desv 20
#define mean 60

typedef struct {
  double *array;
  size_t used;
  size_t size;
} Array;

// Definição da estrutura da lista
typedef struct{
	int tipo;
	int area;
	double suffered_delay;
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
lista * adicionar (lista * apontador, int n_tipo, int area, double suffered_delay, double n_tempo)
{
	lista * lap = apontador;
	lista * ap_aux, * ap_next;
	if(apontador == NULL)
	{
		apontador = (lista *) malloc(sizeof (lista));
		apontador -> proximo = NULL;
		apontador -> tipo = n_tipo;
		apontador -> area = area;
		apontador -> suffered_delay = suffered_delay;
		apontador -> tempo = n_tempo;
		return apontador;
	}
	else
	{
		if (apontador->tempo > n_tempo) {
	        ap_aux = (lista *) malloc(sizeof (lista));
	        ap_aux -> tipo = n_tipo;
	        ap_aux -> area = area;
			ap_aux -> suffered_delay = suffered_delay;
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
		apontador -> area = area;
		apontador -> suffered_delay = suffered_delay;
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
			printf("Tipo=%d\tArea=%d\tTempo=%lf\n", apontador -> tipo, apontador -> area, apontador -> tempo);
			apontador = (lista *)apontador -> proximo;
		}
	}
}

// /*return random number between zero and one */
double generate_random() {
	return ((double) rand()+1)/RAND_MAX;
}

// /*returns time between calls*/
double time_between_calls(){
	double u = generate_random(), r;
	return r = -(1/lambda)*log(u);
}

// /*returns if a call is general or general+specific*/
int get_call_area(){

	double p = generate_random();
	int x;
	if (p <= 0.3) x = GENERAL; 
	else x = SPECIFIC;
	return x;
}

// /*returns duration of the call in the area specific chhannel*/
double duration_of_call_general(int area){
	double r;

	if(area == GENERAL) {
		//exponential avg 120, min 60 and max 300
		double u =generate_random();
		r =(double) 60 -dm*log(u);
		if( r > (double) 300) r = (double) 300;		
	} else {
		//gaus avg 60, std 20, min 30, max 120
		double u2=generate_random(), u=generate_random();
		double teta = 2*M_PI*u;
		r = (sqrt(-2*log(u2))*cos(teta));

		r = 30+ ((r*st_desv) + mean);
		if( r > (double) 120) r = (double) 120;
	}

	return r;
}

// /*returns duration of the call in the area specific chhannel*/
double duration_of_call_specific(){
	double u = generate_random(), r, d =150.0;

	return (double) (r = 60 -d*log(u));
}

// /*returns running average delay*/
double running_average(int n, double current_sample, double previous_avg){
		return (previous_avg*(n-1) + current_sample)/n;
}

double media(double* values, int n)
{
    int i;
    double s = 0;
 
    for ( i = 0; i < n; i++ )
        s += values[i];
    return s / n;
}
double stddev(double* values, int n, double average)
{
    int i;
    double s = 0;
 
    for ( i = 0; i < n; i++ )
        s += (values[i] - average) * (values[i] - average);
    return sqrt(s / (n - 1));
}
void initArray(Array *a, size_t initialSize) {
  a->array = (double *)malloc(initialSize * sizeof(double));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Array *a, int element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (double *)realloc(a->array, a->size * sizeof(double));
  }
  a->array[a->used++] = element;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}