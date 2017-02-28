#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int array[10] = {10, 8, 5, 2, 3, 6, 7, 1, 4, 9}; 

int threadCounter = 0;

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

void *megresort(void *array){
	NODE *p = (NODE *)array;
	NODE n1, n2;

	int mid = (p->i + p->j)/2;
	pthread_t tid1, tid2;
	int ret;
	
	n1.i = p->i;
	n1.j = mid;

	n2.i = mid + 1;
	n2.j = p->j;

	if(p->i >= p->j) return;

	ret = pthread_create(&tid1, NULL, megresort, &n1);

	if(ret){
		printf("%d %s -unable to create tread - ret - %d\n", __LINE__, __FUNCTION__, ret);
		exit(1);
	}
	else threadCounter++;	
	
	ret = pthread_create(&tid2, NULL, megresort, &n2);
	
	if(ret){
		printf("%d %s -unable to create tread - ret - %d\n", __LINE__, __FUNCTION__, ret);
		exit(1);
	}
	else threadCounter++;

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	merge(p->i, p->j);
	pthread_exit(NULL);
}

int main(){
	int i;
	NODE m;
	m.i = 0;
	m.j = 9;

	pthread_t tid;

	int ret;

	ret = pthread_create(&tid, NULL, megresort, &m);
	if(ret){
		printf("%d %s -unable to create tread - ret - %d\n", __LINE__, __FUNCTION__, ret);
		exit(1);
	}
	else threadCounter++;

	pthread_join(tid, NULL);

	for(i = 0; i < 10; i++){
		printf("%d ", array[i]);
	}

	printf("\n");

	printf("Total number of threads is %d\n", threadCounter);
	return 0;
}


	

