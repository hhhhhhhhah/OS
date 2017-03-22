//
// Created by ayy on 3/22/17.
//
#include "Vector.h"


void vectorInit(vector *v)
{
    v->size = 10;
    v->used = 0;
    v->array = malloc(sizeof(double *) * v->size);
}

int vectorUsed(vector *v)
{
    return v->used;
}

int vectorEmpty(vector *v){
    if(v->used == 0)
        return 1;
    else return 0;
}

static void vectorResize(vector *v, int size)
{
    v->array = realloc(v->array, sizeof(double *) * size);
    v->size = size;
}

void vectorAdd(vector *v, double item)
{
    if (v->size == v->used)
        vectorResize(v, v->size * 2);
    v->array[v->used] = item;
    v->used++;
}

void vectorSet(vector *v, int index, double item)
{
    if (index >= 0 && index < v->used)
        v->array[index] = item;
}

double vectorGet(vector *v, int index)
{
    if (index >= 0 && index < v->used)
        return v->array[index];
    //return NULL;
}

void vectorDelete(vector *v, int index)
{
    if (index < 0 || index >= v->used)
        return;

    for (int i = index; i < v->used - 1; i++) {
        v->array[i] = v->array[i + 1];
        //v->array[i + 1] = NULL;
    }
    v->used--;
}

void vectorFree(vector *v)
{
    free(v->array);
}

void vectorPrint(vector *v){
    for(int i = 0; i < v->used; i++){
        printf("%lf\t", v->array[i]);
    }
    printf("\n");
}

void printMenu(){
    printf("1. Add item\n");
    printf("2. Get item\n");
    printf("3. Set item\n");
    printf("4. Delete item\n");
    printf("5. Check if empty\n");
    printf("6. Print\n");
    printf("7. Current number of elements\n");
    printf("8. Quit\n");
    printf(">>>>");
}
