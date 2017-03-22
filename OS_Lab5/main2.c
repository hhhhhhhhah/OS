#include <stdio.h>
#include <dlfcn.h>
#include "Vector.h"

int main() {
    void *lib = dlopen("./myvector.so", RTLD_NOW);
    if (lib) {

        void (*vectorInit)(vector *);
        int (*vectorUsed)(vector *);
        int (*vectorEmpty)(vector *);
        static void (*vectorResize)(vector *, int);
        void (*vectorAdd)(vector *, double);
        void (*vectorSet)(vector *, int, double);
        double (*vectorGet)(vector *, int);
        void (*vectorDelete)(vector *, int);
        void (*vectorFree)(vector *);
        void (*vectorPrint)(vector *);
        void (*printMenu)();

        vectorInit = dlsym(lib, "vectorInit");
        vectorUsed = dlsym(lib, "vectorUsed");
        vectorEmpty = dlsym(lib, "vectorEmpty");
        vectorResize = dlsym(lib, "vectorResize");
        vectorAdd = dlsym(lib, "vectorAdd");
        vectorGet = dlsym(lib, "vectorGet");
        vectorSet = dlsym(lib, "vectorSet");
        vectorDelete = dlsym(lib, "vectorDelete");
        vectorFree = dlsym(lib, "vectorFree");
        vectorPrint = dlsym(lib, "vectorPrint");
        printMenu = dlsym(lib, "printMenu");


        vector *vec = malloc(sizeof(vector));
        vectorInit(vec);


        while (1) {
            int n, pos;
            double temp;
            printMenu();
            scanf("%d", &n);
            switch (n) {
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
                    if (vectorEmpty(vec))
                        printf("Empty\n");
                    else printf("Not empty\n");
                    break;

                case 6:
                    if (vectorEmpty(vec)) {
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
}