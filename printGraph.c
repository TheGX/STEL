int * histogram(double data, int size, int * histograma, double delta){
	for(int z = 0; z < size; z++){
		if(data >= z*delta && data < (z+1)*data) {
			histograma[z]++;
		} if(z == 24 && data >= (z+1)*data)
			histograma[z]++;
	}
	return histograma;
}	
void printGraph(int histograma[25], int max){
	for(int height = max; height > 0; height--){
		printf("%02d ", height);
		for(int z = 0; z < 25; z++){
			if(histograma[z] >= height) printf(" = ");
			else printf("  ");
		}
		printf("\n");
	}	 
	printf("x: ");	
	for(int z = 0; z < 25; z++) printf("%.3f ", z*0.001);
	
	printf("\n");
}

int findmax(int *histograma, int size){
	int max =0;
	for(int z = 0; z < size; z++){
		if(histograma[z] > max) max = histograma[z];
	}

	return max;
}
