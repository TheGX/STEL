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

/*returns running average delay*/
double running_average(int n, double current_sample, double previous_avg);
int * histogram(double data, int size, int * histograma, double delta);

int main(int argc, char const *argv[]) {
    
    if(argc != 5){
        printf("Usage ./2CallCenter N_SAMPLES N_General_Channels General_Queue_Lenght N_Speci_Channels\n");
        return 0;
    } 
    int n_samples 			= 	atoi(argv[1]);
    int n_general_channels	= 	atoi(argv[2]);
    int capacity			= 	atoi(argv[3]);
    int n_specific_channels	= 	atoi(argv[4]);
    int lenght=0, blocked=0;
    lista  *system = NULL;
    lista  *general_queue = NULL;
    lista  *specific = NULL;
    lista  *specific_queue = NULL;
	double delta = (0.2)*(1/lambda);
    double max_delta = 5 * (1/lambda);
	
    srand(time(0));
    int size = (max_delta/delta);
    int delayed=0, general_bussy=0, specific_bussy=0, area = 0, aux_n_specific=0;
	int n_specific = 0, specific_delayed=0;
    double d=0, c = 0, total_delay=0, total_specific_delay=0; 
    double delay=0, specific_delay=0; 
	double avg=0, i_delay=0;
    int *general_histograma = (int *)malloc(size*sizeof(int));
    int *prediction_histograma = (int *)malloc(size*sizeof(int));
    int i=0; 
    while(i < n_samples) {
    	if(i == 0) system = adicionar(system, ARRIVAL, SPECIFIC, 0);
		
		if(system ->tipo == ARRIVAL){
			//GENERATE NEXT ARRIVAL
			area = get_call_area();
			i++;
			d = time_between_calls();
			system = adicionar(system, ARRIVAL, area, (system->tempo + d));
			
			//PROCESS EVENT THAT ARRIVED
			if(general_bussy < n_general_channels) {
				//GENERAL RESOURCES AVAILABLE 
				general_bussy++;
				d = duration_of_call_general(system->area); 
				system = adicionar(system, DEPARTURE, system->area, (system->tempo + d));
			}else if(lenght < capacity) {
				//QUEUE EVENT
				general_queue = adicionar(general_queue, ARRIVAL, system->area, system->tempo);
				lenght++;
				delayed++;
			} else blocked++; //QUEUE FULL -> BLOCKED
		}
		else { 	//GENERAL DEPARTURE
			general_bussy--;
			// PROCESS QUEUED EVENTS
			if(general_queue != NULL && capacity > 0) {
				general_bussy++;
				delay = system->tempo - general_queue->tempo;
				total_delay += delay;

				//ADD PREVIOUS (i-1) PREDICTION ERROR
				prediction_histograma = histogram(fabs(avg-delay), size, prediction_histograma, delta);
				//COMPUTE RUNNING AVG FOR CURRENT I
				printf("Sample %d - Running Avg: %lf\n", i, avg);
				avg = running_average(i, delay, avg);

				general_histograma = histogram(delay, size, general_histograma, delta);
				d = duration_of_call_general(general_queue->area); 
				system = adicionar(system, DEPARTURE, general_queue->area, (system->tempo + d));
				general_queue = remover(general_queue);
				lenght--;
			}
			
			if(system->area == SPECIFIC) {
				n_specific++;
				//PROCESS EVENT THAT ARRIVED

				specific = adicionar(specific, ARRIVAL, SPECIFIC, system->tempo);
				while(specific->tempo < system->tempo) {

					if(specific->tipo == DEPARTURE){
						specific_bussy--;

						// PROCESS QUEUED EVENTS
						if(specific_queue != NULL){
							specific_bussy++;
							specific_delay = (specific->tempo - specific_queue->tempo);
							total_specific_delay += specific_delay;
							d = duration_of_call_specific();
							specific_queue = remover(specific_queue); 
							specific = adicionar(specific, DEPARTURE, SPECIFIC, (specific->tempo + d));
						}
					} else {
						if(specific_bussy < n_specific_channels) {
							//SPECIFIC RESOURCES AVAILABLE
							specific_bussy++;
							d = duration_of_call_specific(); 
							specific = adicionar(specific, DEPARTURE, SPECIFIC, (specific->tempo + d));
						} else {
							//QUEUE SPECIFIC EVENT
							specific_queue = adicionar(specific_queue, ARRIVAL, SPECIFIC, specific->tempo);
							specific_delayed++;
						}
					}
					specific = remover(specific);
				}
			}
		}
		
		system = remover(system);
    }	
	// printGraph(prediction_histograma,25);
    printf("Avg Packets Blocked:\t %lf%%\n",(double)blocked/n_samples *100);
    printf("Avg Packets Delayed:\t %lf%%\n",(double)delayed/n_samples *100);
    printf("Avg Delayed Time:\t %lf\n",(double)total_delay/delayed);
    printf("Avg Spec Delayed:\t %lf%%\n",(double)specific_delayed/n_specific *100);
    printf("Avg Spec Delayed Time:\t %lf\n", (double)total_specific_delay/specific_delayed);

	FILE *f =fopen( "Delay.txt", "a");
	for (int i = 0; i < 25; i++)
	{
		fprintf(f, "%d\n", general_histograma[i]);
	}
	fclose(f);
    return 1;
}

double time_between_calls(){
	double u = generate_random(), r;
	return r = -(1/lambda)*log(u);
}

int get_call_area(){

	double p = generate_random();
	int x;
	if (p <= 0.3) x = GENERAL; 
	else x = SPECIFIC;
	return x;
}
double generate_random() {
	return ((double) rand()+1)/RAND_MAX;
}

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

double duration_of_call_specific(){
	double u = generate_random(), r, d =150.0;

	return (double) (r = 60 -d*log(u));
}

double running_average(int n, double current_sample, double previous_avg){
		return (previous_avg*(n-1))/n + current_sample/n;
}