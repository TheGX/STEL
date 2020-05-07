#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include"prototipo.c"
#include"printGraph.c"

#define	lambda 0.0347f
#define	dm 120.0f
#define	ARRIVAL 0
#define	DEPARTURE 1
#define	SPECIFIC 0
#define	GENERAL 1
#define st_desv 20
#define mean 60

/*return random number between zero and one */
double generate_random();
double media(double* values, int n);
/*returns if a call is general or general+specific*/
int get_call_area();

/*returns duration of the call in the area specific chhannel*/
double duration_of_call_general(int area);

/*returns duration of the call in the area specific chhannel*/
double duration_of_call_specific();

/*returns time between calls*/
double time_between_calls();
double stddev(double* values, int n, double average);

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
    int delayed=0, general_bussy=0, specific_bussy=0, area = 0;
	int n_specific = 0, specific_delayed=0, error_count=0;
    double d=0, total_delay=0, total_specific_delay=0, rel_error_sum=0, abs_error_sum=0; 
    double delay=0, specific_delay=0, avg_specific_total_delay=0; 
	double avg=0, abs_error=0, rel_error=0;
	Array Array_delays;
	initArray(&Array_delays,10);
    int *general_histograma = (int *)malloc(size*sizeof(int));
    int *prediction_histograma = (int *)malloc(size*sizeof(int));
    int i=0; 
    while(i < n_samples) {
    	if(i == 0) system = adicionar(system, ARRIVAL, SPECIFIC, 0, 0);
		
		if(system ->tipo == ARRIVAL){
			//GENERATE NEXT ARRIVAL
			area = get_call_area();
			i++;
			d = time_between_calls();
			system = adicionar(system, ARRIVAL, area, 0, (system->tempo + d));
			
			//PROCESS EVENT THAT ARRIVED
			if(general_bussy < n_general_channels) {
				//GENERAL RESOURCES AVAILABLE 
				general_bussy++;
				d = duration_of_call_general(system->area); 
				system = adicionar(system, DEPARTURE, system->area, 0, (system->tempo + d));
			}else if(lenght < capacity) {
				//QUEUE EVENT
				general_queue = adicionar(general_queue, ARRIVAL, system->area, 0, system->tempo);
				lenght++;
				delayed++;
				// avg = running_average(delayed, delay, avg);
				// printf("Sample %d - Running Avg: %lf\n", i, avg);
			} else blocked++; //QUEUE FULL -> BLOCKED
		}
		else { 	//GENERAL DEPARTURE
			general_bussy--;
			// PROCESS QUEUED EVENTS
			if(general_queue != NULL && capacity > 0) {
				general_bussy++;
				delay = system->tempo - general_queue->tempo;
				general_queue->suffered_delay +=delay;
				total_delay += delay;

				//ADD PREVIOUS (i-1) PREDICTION ERROR
				abs_error = fabs(delay-avg);
				error_count++;
				rel_error_sum += rel_error;
				abs_error_sum += abs_error;
				prediction_histograma = histogram(abs_error, size, prediction_histograma, delta);
				//COMPUTE RUNNING AVG FOR CURRENT I
				avg = running_average(error_count, delay, avg);

				general_histograma = histogram(delay, size, general_histograma, delta);
				d = duration_of_call_general(general_queue->area); 
				system = adicionar(system, DEPARTURE, general_queue->area, general_queue->suffered_delay, (system->tempo + d));
				general_queue = remover(general_queue);
				lenght--;
			}
			
			if(system->area == SPECIFIC) {
				int i =n_specific;
				n_specific++;
				//PROCESS EVENT THAT ARRIVED

				specific = adicionar(specific, ARRIVAL, SPECIFIC, system->suffered_delay, system->tempo);
				while(specific->tempo < system->tempo && n_specific_channels > 0 && specific != NULL) {

					if(specific->tipo == DEPARTURE){
						specific_bussy--;

						// PROCESS QUEUED EVENTS
						if(specific_queue != NULL){
							specific_bussy++;
							specific_delay = (specific->tempo - specific_queue->tempo);
							specific_queue->suffered_delay += specific_delay;
							total_specific_delay += specific_delay;

							d = duration_of_call_specific();
							// test[i] = specific->suffered_delay; 
							i++;
							avg_specific_total_delay+= specific_queue->suffered_delay; 
							insertArray(&Array_delays, specific_queue->suffered_delay);
							specific = adicionar(specific, DEPARTURE, SPECIFIC, specific_queue->suffered_delay, (specific->tempo + d));
							specific_queue = remover(specific_queue); 
						}
					} else {
						if(specific_bussy < n_specific_channels) {
							//SPECIFIC RESOURCES AVAILABLE
							specific_bussy++;
							d = duration_of_call_specific();
							// test[i] = specific->suffered_delay; 
							i++;
							avg_specific_total_delay+= specific->suffered_delay;
							insertArray(&Array_delays, specific->suffered_delay);
							specific = adicionar(specific, DEPARTURE, SPECIFIC, specific->suffered_delay, (specific->tempo + d));
						} else {
							//QUEUE SPECIFIC EVENT
							specific_queue = adicionar(specific_queue, ARRIVAL, SPECIFIC, specific->suffered_delay, specific->tempo);
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
	double avg_delay = (double)total_delay/delayed;
    printf("Avg Packets Blocked:\t %lf%%\n",(double)blocked/n_samples *100);
    printf("Avg Packets Delayed:\t %lf%%\n",(double)delayed/n_samples *100);
    printf("Avg Delayed Time:\t %lf\n",avg_delay);
    printf("Avg Spec Delayed:\t %lf%%\n",(double)specific_delayed/n_specific *100);
    printf("Avg Spec Delayed Time:\t %lf\n", (double)total_specific_delay/specific_delayed);
	double avg_queue_time_spec = media(Array_delays.array, Array_delays.used); 
    printf("Avg Time Spec Packets in Queues:\t %lf\n", avg_queue_time_spec);
	
	double avg_abs =  (double)abs_error_sum/error_count;
    printf("\nAvg Abs Prediction Error:\t %lf\n", avg_abs);
	rel_error = fabs(abs_error/avg_delay);
    printf("Avg Rel Prediction Error:\t %lf%%\n", rel_error *100);
	

	double desv_padrao = stddev(Array_delays.array , Array_delays.used, avg_queue_time_spec);
	double erro_padrao_media = desv_padrao/(sqrt(Array_delays.used));

	double limite_confi = 1.65;
	double interv_confi = limite_confi*erro_padrao_media;
	printf("\nIntervalo confiança, com limite 90%% %lf\n", interv_confi);

	// FILE *f =fopen( "Delay.txt", "a");
	// for (int i = 0; i < 25; i++)
	// {
	// 	fprintf(f, "%d\n", prediction_histograma[i]);
	// }
	// fclose(f);
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