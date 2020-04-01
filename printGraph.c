void printGraph(int histograma[25], int max){
	for(int height = max; height > 0; height--){
		printf("%02d ", height);
		for(int z = 0; z < 25; z++){
			if(histograma[z] >= height) printf(" = ");
			else printf("   ");
		}
		printf("\n");
	}	 
	printf("x: ");	
	for(int z = 0; z < 25; z++) printf("%02d ", z);
	
	printf("\n");
}
