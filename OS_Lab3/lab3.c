#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int *array; 

int threadsNumber;
int threadsCounter = 1;

typedef struct node {
	int i;
	int j;
} NODE;

void merge(int i, int j){
	int mid = (i + j)/2;
	int aIndex = i;
	int bIndex = mid + 1;
	
	int newArray[j-i+1];
	int newArrayIndex = 0;

	while(aIndex <= mid && bIndex <=j){
		if(array[aIndex] > array[bIndex]){
			newArray[newArrayIndex++] = array[bIndex++];
		}
		else {
			newArray[newArrayIndex++] = array[aIndex++];
		}
	}

	while(aIndex <= mid){
		newArray[newArrayIndex++] = array[aIndex++];
	}
	
	while(bIndex <= j){
		newArray[newArrayIndex++] = array[bIndex++];
	}
	
	for(aIndex = 0; aIndex < (j - i + 1); aIndex++){
		array[i + aIndex] = newArray[aIndex];
	}
}

void *megresort(void *nd){
	printf("Thread created. Thread %d of %d\n", threadsCounter, threadsNumber);

	NODE *p = (NODE *)nd;
	NODE n1, n2;

	int k = 0, l = 0;
	int mid = (p->i + p->j)/2;
	pthread_t tid1, tid2;
	int ret;
	
	n1.i = p->i;
	n1.j = mid;

	n2.i = mid + 1;
	n2.j = p->j;

	if(p->i >= p->j) return;

	if(threadsCounter < threadsNumber){	
		ret = pthread_create(&tid1, NULL, megresort, &n1);
		k = 1;
		if(ret){
			printf("%d %s -unable to create tread - ret - %d\n", __LINE__, __FUNCTION__, ret);
			exit(1);
		}
		else threadsCounter++;	
	}
	else {
		megresort(&n1);
	}

	if(threadsCounter < threadsNumber){
		ret = pthread_create(&tid2, NULL, megresort, &n2);
		l = 1;
		if(ret){
			printf("%d %s -unable to create tread - ret - %d\n", __LINE__, __FUNCTION__, ret);
			exit(1);
		}
		else threadsCounter++;
	}
	else {
		megresort(&n2);
	}

	if(k == 1){
		pthread_join(tid1, NULL);
	}
	if(l == 1){
		pthread_join(tid2, NULL);
	}

	merge(p->i, p->j);
	pthread_exit(NULL);
}

int* randomArray(int n){
	time_t t;
	srand((unsigned) time(&t)); 
	int *randArray = (int*)malloc(n * sizeof(int));
	if(randArray){
		int i;
		for(i = 0; i < n; i++){
			randArray[i] = rand() % 100;
		}
	}
	return randArray;
}

int main(int argc, char * argv[]){
	int i;
	int n;

	if(argc > 1){
		n = atoi(argv[1]);
	}
	else {
		printf("Enter number to create a random array: ");
		scanf("%d", &n);
	}
	if(n <= 0){
		printf("Wrong number. Need a number > 0.\n");
		return 0;
	}

	if(argc > 2){
		threadsNumber = atoi(argv[2]);
	}
	else {
		printf("How many threads can be used? ");
		scanf("%d", &n);
	}

	array = randomArray(n);

	for(i = 0; i < n; i++){
		printf("%d ", array[i]);
	}
	printf("\n");

	NODE m;
	m.i = 0;
	m.j = n-1;

	pthread_t tid;

	int ret;

	ret = pthread_create(&tid, NULL, megresort, &m);
	if(ret){
		printf("%d %s -unable to create tread - ret - %d\n", __LINE__, __FUNCTION__, ret);
		exit(1);
	}

	pthread_join(tid, NULL);

	for(i = 0; i < n; i++){
		printf("%d ", array[i]);
	}

	printf("\n");

	printf("Total number of threads is %d\n", threadsCounter);
	return 0;
}


	

