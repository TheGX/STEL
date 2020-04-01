#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"prototipo.c"

#define	lambda 200.0f
#define	dm 0.008f
#define	ARRIVAL 0
#define	DEPARTURE 1

double generate_event(int type);
int main(int argc, char const *argv[]) {
    
    if(argc != 3){
        printf("Usage ./intervalos N_Channels N_SAMPLES\n");
        return 0;
    } 
    int n_channels = atoi(argv[1]);
    int n_samples = atoi(argv[2]);
    lista  *lista_eventos = NULL;
    lista  *queue = NULL;
    double delta = (0.2)*(1/lambda);
    double max_delta = 5 * (1/lambda);
	
    srand(time(0));
    int size = (max_delta/delta);
    int bussy=0, delayed=0;
    double total_c = 0, u=0, d=0, c = 0, total_delay=0; 
	
    int i=0;
	while(i < n_samples) {
    		if(i == 0) lista_eventos = adicionar(lista_eventos, ARRIVAL, 0);

		if (lista_eventos->tipo == DEPARTURE){
			bussy--;
			if(queue != NULL) { //RESOURCES WERE FREED AND THERES ELEMENTS IN THE QUEUE 
				total_delay += (lista_eventos->tempo - queue->tempo);
				bussy++;
				d = generate_event(DEPARTURE);
				lista_eventos = adicionar(lista_eventos, DEPARTURE, lista_eventos->tempo + d);
				queue = remover(queue);
			}
		}else{
			i++;
			c = generate_event(ARRIVAL);
			total_c += c;
			lista_eventos = adicionar(lista_eventos, ARRIVAL, lista_eventos->tempo + c); //ADD THE NEXT ARRIVAL
			if(bussy < n_channels) { //AVAILABLE RESOURCES
				bussy++;
				d = generate_event(DEPARTURE);
				lista_eventos = adicionar(lista_eventos, DEPARTURE,  lista_eventos->tempo + d);
			} else {
				delayed++;
				queue = adicionar(queue, ARRIVAL, lista_eventos->tempo);	//ADD DELAYED ELEMENT TO QUEUE WITH TIME OF ARRIVAL
			}
		}
		lista_eventos = remover(lista_eventos);
    	}	
    total_c = total_c /(double) n_samples;
    printf("Estimador = %f\n", total_c);
    printf("lambda = %lf\nn_samples = %d\ndelta = %lf\nmax_delta = %lf\nHistogram size =%d\n", lambda, n_samples, delta, max_delta, size);
    printf("Avg Packets Delayed: 		%lf%%\n",(double)delayed/n_samples *100);
    printf("Avg Delayed Time: 	 	%lf\n",(double)total_delay/n_samples);

    return 1;
}

double generate_event(int type){
	double u = ((double) rand()+1)/RAND_MAX, r;

	if(type == ARRIVAL) r = -(1/lambda)*log(u);
	else r = -dm*log(u);

	return r;
}
