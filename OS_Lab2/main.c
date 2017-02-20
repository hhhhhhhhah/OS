#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <unistd.h>
#include <sys/types.h>
#include <memory.h>

void insertionSort(int arr[], int n);
void merge(int array1[], int m, int array2[], int n, int array3[]);

int main(int argc, char* argv[]){
    char filename1[255];
    char filename2[255];
    char filename3[255];

    printf("Please enter filename1: ");
    scanf("%s",filename1);
    printf("Please enter filename2: ");
    scanf("%s",filename2);

    int parToChild1[2];
    int parToChild2[2];
    pid_t childpid1, childpid2;

    pipe(parToChild1);
    pipe(parToChild2);

    char readbuffer[20];

    if((childpid1 = fork()) == -1){
        perror("fork");
        exit(1);
    }
    if(childpid1 == 0){
        close(parToChild1[0]);
        read(parToChild1, filename1, readbuffer);

        FILE *file1 = fopen(filename1,"r");

        int array1[100];
        int i;
        int num;
        for(i = 0; fscanf(file1, "%d", &num) > 0; i++) {
            array1[i] = num;
        }
		fclose (file1);
		fopen (filename1, "w");

        insertionSort(array1, i);
        for(int n = 0; n < i; n++){
			fprintf(file1, "%d\n", array1[n]);
        }

		fclose(file1);
		exit(0);
    }

    if((childpid2 = fork()) == -1){
        perror("fork");
        exit(1);
    }
    if(childpid2 == 0){
        close(parToChild2[0]);
        read(parToChild2, filename2, readbuffer);

        FILE *file2 = fopen(filename2,"r");

        int array2[100];
        int i;
        int num;
        for(i = 0; fscanf(file2, "%d", &num) > 0; i++) {
            array2[i] = num;
        }
    	fclose (file2);
		fopen (filename2, "w");

        insertionSort(array2, i);
        for(int n = 0; n < i; n++){
			fprintf(file2, "%d\n", array2[n]);
        }

		fclose(file2);

		exit(0);
    }

	waitpid (childpid1, NULL, 0);
	waitpid (childpid2, NULL, 0);

	FILE* file1 = fopen (filename1, "r");
    FILE* file2 = fopen (filename2, "r");

    int array1[100];
    int m;
    int num;

    for(m = 0; fscanf(file1, "%d", &num) > 0; m++) {
        array1[m] = num;
    }

    int array2[100];
    int n;

    for(n = 0; fscanf(file2, "%d", &num) > 0; n++) {
        array2[n] = num;
    }

    fclose(file1);
    fclose(file2);

	printf("Please enter result filename: ");
    scanf("%s",filename3);
	FILE* file3 = fopen (filename3,"w");

	int sorted[m+n];

    merge(array1, m, array2, n, sorted);

	for(int x = 0; x < m + n; x++) {
		fprintf (file3, "%d\n", sorted[x]);
	}
	fclose(file3);

	return 0;
}

void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void merge(int array1[], int m, int array2[], int n, int array3[]) {
  int i, j, k;

  i = j = k = 0;

  while (i < m && j < n)
    {
        if (array1[i] < array2[j])
        {
            array3[k] = array1[i];
            i++;
        }
        else
        {
            array3[k] = array2[j];
            j++;
        }
        k++;
    }
    if (i >= m)
    {
        while (j < n)
        {
            array3[k] = array2[j];
            j++;
            k++;
        }
    }
    if (j >= n)
    {
        while (i < m)
        {
            array3[k] = array1[i];
            i++;
            k++;
        }
    }
}
