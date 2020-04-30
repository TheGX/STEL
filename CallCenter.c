#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"prototipo.c"
#include"printGraph.c"

#define	lambda 0.022f
#define	dm 0.008f
#define	ARRIVAL 0
#define	DEPARTURE 1

double generate_event(int type);
int * histogram(double data, int size, int * histograma, double delta);
int main(int argc, char const *argv[]) {
    
    if(argc != 4){
        printf("Usage ./FiniteQueue N_Channels N_SAMPLES General_Queue_Lenght\n");
        return 0;
    } 
    int general_channels = 	atoi(argv[1]);
    int n_samples = 	atoi(argv[2]);
    int capacity= 	atoi(argv[3]);
    int lenght=0, blocked=0;
    lista  *system_arrival = NULL; 			
    lista  *specific = NULL; 		
    lista  *specific_queue = NULL; 			//INFINITE QUEUE
    lista  *general_queue = NULL;			//FINITE QUEUE
    double delta = (0.2)*(1/lambda);
    double max_delta = 5 * (1/lambda);
	
    srand(time(0));
    int size = (max_delta/delta);
    int bussy=0, delayed=0;
    double total_c = 0, d=0, c = 0, total_delay=0; 
    int *histograma = (int *)malloc(size*sizeof(int));
    double delay=0; 
    
    int i=0; 
    while(i < n_samples) {
    	if(i == 0) system_arrival = adicionar(arrival, ARRIVAL, 0);

	if (system_arrival->tipo == DEPARTURE){
		bussy--;
		// IF(SYSTEM_ARRIVAL->SPECIFC != NULL)
		// 	SEND TO SPECIFIC
		//		CHECK IF RESOURCERS -> ADD TO specific 
		//		IF NOT -> add to specific_queue
		// ELSE FINISH
		if(general_queue != NULL) { 			//RESOURCES WERE FREED AND THERES ELEMENTS IN THE QUEUE 
			delay = (system_arrival->tempo - general_queue->tempo);
			total_delay += delay;
			histograma = histogram(delay, size, histograma, delta);
			bussy++;
			//TODO SPECIFIC OR NOT
			//TODO GENERATE RETURNS DEPENDS ON SPECIFIC OR NOT -> ADD VARIABLE TO STRUCT TO REPRESENT SPECIFIC
			d = generate_event(DEPARTURE);		//GENEATE PROCESSING TIME
			system_arrival = adicionar(system_arrival, DEPARTURE, system_arrival->tempo + d);
			general_queue = remover(general_queue);
			lenght--;
		}
	}else{
		i++;
		c = generate_event(ARRIVAL);
		system_arrival = adicionar(arrival, ARRIVAL, arrival->tempo + c); 			//ADD NEXT ARRIVAL
		total_c += c;
		if(bussy < general_channels) { 								//RESOURCES AVAILABLE
			bussy++;
			//TODO SPECIFIC OR NOT
			//TODO GENERATE RETURNS DEPENDS ON SPECIFIC OR NOT -> ADD VARIABLE TO STRUCT TO REPRESENT SPECIFIC
			d = generate_event(DEPARTURE);
			system_arrival = adicionar(system_arrival, DEPARTURE,  system_arrival->tempo + d);
		} else if(lenght < capacity){								//CHECK IF QUEUE IS FULL
			delayed++;
			lenght++;
			general_queue = adicionar(general_queue, ARRIVAL, system_arrival->tempo);			//ADD DELAYED ELEMENT TO QUEUE WITH TIME OF ARRIVAL
		} else blocked++;
	}
	system_arrival = remover(system_arrival);
    }	
    total_c = total_c /(double) n_samples;
    printGraph(histograma, size);
    printf("Avg Packets Blocked: 		%lf%%\n",(double)blocked/n_samples *100);
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
