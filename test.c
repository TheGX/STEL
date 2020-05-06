#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"prototipo.c"
#include"printGraph.c"

#define	lambda 0.022f
#define	ARRIVAL 0
#define	DEPARTURE 1
#define	SPECIFIC 0
#define	GENERAL 1
#define st_desv 20
#define mean 60

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
int get_call_area();

int * histogram(double data, int size, int * histograma, double delta);

int main(int argc, char const *argv[]) {
    if(argc != 5){
        printf("Usage ./CallCenter Samples N_General_Channels General_Queue_Lenght N_Specific_Channels\n");
        return 0;
    } 
    int n_samples 	= 	atoi(argv[1]);
    int general_channels = 	atoi(argv[2]);
    int capacity	= 	atoi(argv[3]);
    int specific_channels = 	atoi(argv[4]);
    int lenght=0, blocked=0;
    lista  *system_arrival = NULL; 			
    lista  *specific = NULL; 		
    lista  *specific_queue = NULL; 			//INFINITE QUEUE
    lista  *general_queue = NULL;			//FINITE QUEUE
    double delta = (0.2)*(1/lambda);
    double max_delta = 5 * (1/lambda);
	
    srand(time(0));
    int size = (max_delta/delta);
    int general_bussy=0, specific_bussy=0, delayed=0;
    double d=0, c = 0, total_delay=0; 
    int *histograma = (int *)malloc(size*sizeof(int));
    double delay=0, specific_delay=0;
	   
    int i=0; 
	system_arrival = adicionar(system_arrival, ARRIVAL, GENERAL, 0);
    while(i < n_samples) {

		if (system_arrival->tipo == DEPARTURE){
			general_bussy--;/*
			if(system_arrival->area == SPECIFIC) {
				specific = adicionar(specific, ARRIVAL, SPECIFIC, system_arrival->tempo);
				if(specific->tipo == DEPARTURE) {
					specific_bussy--;

					if(specific_queue != NULL){
						specific_delay = (specific->tempo - specific_queue->tempo);
						specific_bussy++;
						double d = duration_of_call_area_specific();
						specific = adicionar(specific, DEPARTURE, SPECIFIC, specific->tempo+d);
						specific_queue = remover(specific_queue);
					}
				} else{
					if(specific_bussy < specific_channels){
						specific_bussy++;
						double d = duration_of_call_area_specific();
						specific = adicionar(specific, DEPARTURE, SPECIFIC, specific->tempo + d);
					} else{
						specific_queue = adicionar(specific_queue, ARRIVAL, SPECIFIC, system_arrival->tempo);			
					}	
				}
				specific = remover(specific);
			}*/
			if(general_queue != NULL) { 									//RESOURCES FREED AND THERES ELEMENTS IN THE QUEUE 
				delay = (system_arrival->tempo - general_queue->tempo);
				total_delay += delay;
				// printf("Delay: %lf\n", delay);

				histograma = histogram(delay, size, histograma, delta);
				general_bussy++;
				double d = duration_of_call_general(GENERAL);
				// double d = duration_of_call_general(system_arrival->area);
				system_arrival = adicionar(system_arrival, DEPARTURE, general_queue->area, system_arrival->tempo + d);
				general_queue = remover(general_queue);
				lenght--;
			}
		}else{
			i++;
			int area = get_call_area();									//DETERMINE IF CALL NEEDS SPECIFIC 
			// c = time_between_calls();
			// system_arrival = adicionar(system_arrival, ARRIVAL, area, system_arrival->tempo + c); 		//ADD NEXT ARRIVAL
			if(i==1) system_arrival = adicionar(system_arrival, ARRIVAL, area, 5); 		
			else if(i ==2) system_arrival = adicionar(system_arrival, ARRIVAL, area, 7); 		
			else if(i ==3) system_arrival = adicionar(system_arrival, ARRIVAL, area, 9); 		
			else if(i ==4) system_arrival = adicionar(system_arrival, ARRIVAL, area, 10); 		
			if(general_bussy < general_channels) { 								//RESOURCES AVAILABLE
				general_bussy++;
				printf("All good @ i=%d\n", i);
				// double d = duration_of_call_general(system_arrival->area);
				// double d = duration_of_call_general(GENERAL);
				// system_arrival = adicionar(system_arrival, DEPARTURE, system_arrival->area, system_arrival->tempo + d);
				if(i == 1)system_arrival = adicionar(system_arrival, DEPARTURE, system_arrival->area, 6);
				if(i == 2)system_arrival = adicionar(system_arrival, DEPARTURE, system_arrival->area, 8);
				if(i == 3)system_arrival = adicionar(system_arrival, DEPARTURE, system_arrival->area, 11);
				if(i == 4)system_arrival = adicionar(system_arrival, DEPARTURE, system_arrival->area, 12);
			} else if(lenght < capacity){									//CHECK IF QUEUE IS FULL
				printf("Delayed @ i=%d\n", i);
				delayed++;
				lenght++;
				general_queue = adicionar(general_queue, ARRIVAL, system_arrival->area, system_arrival->tempo);		//ADD DELAYED ELEMENT TO QUEUE WITH TIME OF ARRIVAL
				// printf("Running average: %lf\n", avg);
			} else {
				printf("Blocked @ i=%d\n", i);
				blocked++;}
		}
		system_arrival = remover(system_arrival);
    }	
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
	double dm=150, d=0;
	double u = generate_random();
	d = 60 + (-dm)*log(u);

	return d;
}
double duration_of_call_general(int call_type){
	double dm=120, d=0, u=0;
	double u1=0, u2=0, teta=0; // for Box-Muller

	if (call_type == GENERAL){ 
		u = generate_random();
		d = -dm*log(u); //exponential
	
		if(d > 300) d = 300;
	}else if(call_type == SPECIFIC){  //Box-Muller -> Gaussian
		u1=generate_random();
		u2=generate_random();
		teta = 2*M_PI*u2;
		d = 30 + sqrt(-2*log(u1))*sin(teta);

		if(d > 120) d = 120;
	}
	return d;
	// return (d*st_desv) + mean;
}

double running_average(int n, double current_sample, double previous_avg){
		return (previous_avg*(n-1))/n + current_sample/n;
}

int get_call_area(){

	double p = generate_random();
	int x = (p < 0.3) ? SPECIFIC : GENERAL;
	return x;
}
