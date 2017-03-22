#include <stdio.h>
#include "Vector.h"


int main() {
    vector *vec = malloc(sizeof(vector));
    vectorInit(vec);
/*
    double array[15];
    printf("enter numbers\n");
    for(int i = 0; i < 15; i++){
        scanf("%lf", &array[i]);
        vectorAdd(vec, array[i]);
    }

    for(int i = 0; i < 15; i++) {
        printf("%lf\t",vectorGet(vec, i));
    }
*/
    while(1){
        int n, pos;
        double temp;
        printMenu();
        scanf("%d", &n);
        switch(n){
            case 1:
                printf("Enter number: ");
                scanf("%lf", &temp);
                vectorAdd(vec, temp);
                break;

            case 2:
                printf("Enter position: ");
                scanf("%d", &pos);
                printf("%lf\n", vectorGet(vec, pos));
                break;

            case 3:
                printf("Enter position and number: ");
                scanf("%d %lf", &pos, &temp);
                vectorSet(vec, pos, temp);
                break;

            case 4:
                printf("Enter position: ");
                scanf("%d", &pos);
                vectorDelete(vec, pos);
                break;

            case 5:
                if(vectorEmpty(vec))
                    printf("Empty\n");
                else printf("Not empty\n");
                break;

            case 6:
                if(vectorEmpty(vec)){
                    printf("Empty\n");
                    break;
                }
                vectorPrint(vec);
                break;

            case 7:
                printf("Current number of elements: %d\n", vectorUsed(vec));
                break;

            case 8:
                vectorFree(vec);
                return 0;

            default:
                printf("Wrong number. Try again.");
                break;
        }
    }
}