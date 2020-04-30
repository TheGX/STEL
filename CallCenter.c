#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"prototipo.c"
#include"printGraph.c"

#define	lambda 0.022f
#define	ARRIVAL 0
#define	DEPARTURE 1
#define st_desv 20
#define medium 60

/*return random number between zero and one */
double generate_random();

/*returns duration of the call on the general channel according to it's type*/
double duration_of_call_general(int call_type);

/*returns duration of the call in the area specific chhannel*/
double duration_of_call_area_specific();

/*returns time between calls*/
double time_between_calls();

/*returns running average delay*/
double running_average(int n, double current_sample, double previous_avg);

/*returns if a call is general or general+specific*/
int get_type_call();

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

double generate_random() {

	return ((double) rand()+1)/RAND_MAX;
}

double time_between_calls(){

	double u = generate_random(), r;

	return r = -(1/lambda)*log(u);
}


double duration_of_call_area_specific(){

	double dm=0, d=0, u=0;
	u= generate_random();
	dm= 150;
	return d= 60 + (-dm)*log(u);
}
double duration_of_call_general(int call_type){

	double dm=0, d=0, u=0;
	double u1=0, u2=0, teta=0, r=0; // for Box-Muller

	if (call_type == 0){ //general

		u=generate_random();
		dm= 120;
		d= 60 + (-dm)*log(u); //exponential

		if(d > 300) d=300;

	}

	else if(call_type == 1){ //general + specific

		u1=generate_random();
		u2=generate_random();
		teta= 2*M_PI*u1;
		r= sqrt(-2*log(u2));

		/* I DUNNO THIS */

		if (d>120) d=120;
	}
	return d;
}

double running_average(int n, double current_sample, double previous_avg){
		return (previous_avg*(n-1))/n + current_sample/n;
}

int get_type_call(){

	double p = generate_random();

	if(p < 0.3) return 0;
	else return 1;
}