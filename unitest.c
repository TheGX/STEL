#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"prototipo.c"
#include"printGraph.c"

#define	lambda 0.022f
#define	dm 120.0f
#define	ARRIVAL 0
#define	DEPARTURE 1
#define	SPECIFIC 0
#define	GENERAL 1
#define st_desv 20
#define mean 60

/*return random number between zero and one */
double generate_random();

/*returns if a call is general or general+specific*/
int get_call_area();

/*returns duration of the call in the area specific chhannel*/
double duration_of_call_general(int area);

/*returns duration of the call in the area specific chhannel*/
double duration_of_call_specific();

/*returns time between calls*/
double time_between_calls();
int * histogram(double data, int size, int * histograma, double delta);

int main(int argc, char const *argv[]) {
    
    if(argc != 3){
        printf("Usage ./unitest N_SAMPLES N_Speci_Channels\n");
        return 0;
    } 
    int n_samples 			= 	atoi(argv[1]);
    int n_specific_channels	= 	atoi(argv[2]);
    lista  *specific = NULL;
    lista  *specific_queue = NULL;
    double delta = (0.2)*(1/lambda);
    double max_delta = 5 * (1/lambda);
	
    srand(time(0));
    int size = (max_delta/delta);
    int delayed=0, specific_bussy=0, area = 0, aux_n_specific=0;
    double d=0, c = 0, total_delay=0; 
    double delay=0, specific_call_sum=0; 
    // int *histograma = (int *)malloc(size*sizeof(int));
    int i=0; 
    while(i < n_samples) {
		
		if(i==0) specific = adicionar(specific, ARRIVAL, SPECIFIC, 0);

		if(specific->tipo == ARRIVAL){
			i++;
			specific = adicionar(specific, ARRIVAL, SPECIFIC, specific->tempo + time_between_calls());
			if(specific_bussy < n_specific_channels) {
				//SPECIFIC RESOURCES AVAILABLE
				specific_bussy++;
				d = duration_of_call_specific(); 
				specific_call_sum += d;
				specific = adicionar(specific, DEPARTURE, SPECIFIC, (specific->tempo + d));
			} else {
				//QUEUE SPECIFIC EVENT
				delayed++;
				specific_queue = adicionar(specific_queue, ARRIVAL, SPECIFIC, specific->tempo);
			}
		}
		
		if(specific->tipo == DEPARTURE){
			specific_bussy--;
			aux_n_specific++;
			// PROCESS QUEUED EVENTS
			if(specific_queue != NULL){
				specific_bussy++;
				delay = (specific->tempo - specific_queue->tempo);
				total_delay += delay;
				//TODO calculate specific delay (specific->tempo-specific_queue->tempo)here
				d = duration_of_call_specific();
				specific_call_sum += d;
				specific_queue = remover(specific_queue); 
				specific = adicionar(specific, DEPARTURE, SPECIFIC, (specific->tempo + d));
			}
		}
		specific = remover(specific);
    }	
    // printGraph(histograma, size);
    printf("Avg Packets Delayed: 		%lf%%\n",(double)delayed/n_samples *100);
    printf("Avg Delayed Time: 	 	%lf\n",(double)total_delay/n_samples);

    return 1;
}

double time_between_calls(){
	double u = generate_random(), r;
	return r = -(1/lambda)*log(u);
}

int get_call_area(){

	double p = generate_random();
	int x;
	if (p < 0.3) x = GENERAL; 
	else x = SPECIFIC;
	return x;
}
double generate_random() {
	return ((double) rand()+1)/RAND_MAX;
}

double duration_of_call_general(int area){
	double r, u=generate_random();

	if(area == GENERAL) {
		//exponential avg 120, min 60 and max 300
		r =(double) 60 -dm*log(u);
		if( r > (double) 300) r = (double) 300;
	} else {
		//gaus avg 60, std 20, min 30, max 120
		double u2=generate_random();
		double teta = 2*M_PI*u;
		r = ((double) 30 + sqrt(-2*log(u2))*cos(teta));
		if( r > (double) 120) r = (double) 120;

		r = (double) (r*st_desv) + mean;
	}

	return r;
}

double duration_of_call_specific(){
	double u = generate_random(), r, d =150.0;

	return (double) (r = -d*log(u));
}
