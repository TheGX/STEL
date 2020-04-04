int * histogram(double data, int size, int * histograma, double delta){
	for(int z = 0; z < size; z++){
		if(data >= z*delta && data < (z+1)*data) {
			histograma[z]++;
		} if(z == 24 && data >= (z+1)*data)
			histograma[z]++;
	}
	return histograma;
}	
void printGraph(int histograma[25], int size){
	int max =0, min=99999; 
	for(int z = 0; z < size; z++){
		if(histograma[z] > max) max = histograma[z];
		else if(histograma[z] !=0 && histograma[z] < min) min = histograma[z];
	}
	for(int height = max; height > 0; height--){
		printf("%2d", height);
		for(int z = 0; z < size; z++){
			if(histograma[z] >= height) printf("  =   ");
			else if(z!=0) printf("      ");
		}
		printf("\n");
	}	 
	printf("x: ");	
	for(int z = 0; z < 25; z++) printf("%.3f ", (1+z)*0.001);
	printf("count:");	
	for(int z = 1; z < 25; z++) printf("%03d ", histograma[z]);
	
	printf("\n");
	printf("max:%d min:%d\n", max, min);
}

