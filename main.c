#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

sem_t wholesalerSem;
char *ingredientNames[4] = {"Egg", "Starch", "Walnut", "Sugar"};
int wholesalerIngredients[4] = {0, 0, 0, 0};
struct chef {
    pthread_t thread;
    sem_t sem;
    int neededIngredients[2];
} chefs[6];

void *chef(void *arg) {
    int chefId = (int) arg;
    while (1) {
        printf("Chef %d is waiting for %s and %s\n", chefId, ingredientNames[chefs[chefId].neededIngredients[0]],
               ingredientNames[chefs[chefId].neededIngredients[1]]);
        sem_wait(&chefs[chefId].sem);
        printf("Chef %d has obtained the ingredients\n", chefId);
        sleep(1);
        printf("Chef %d is preparing the baklava\n", chefId);
        sleep(1);
        printf("Chef %d has delivered the baklava to the wholesaler\n", chefId);
        sleep(1);
        printf("Wholesaler has obtained the baklava and left to sell it\n");
        sleep(1);
        sem_post(&wholesalerSem);
    }
}

int isEnoughIngredients() {
    int i, count = 0;
    for (i = 0; i < 4; i++) {
        if (wholesalerIngredients[i] > 0) {
            if (count) {
                return 1;
            }
            count++;
        }
    }
    return 0;
}

void addIngredient(char ingredient) {
    switch (ingredient) {
        case 'Y':
            wholesalerIngredients[0]++;
            printf("\nWholesaler has bought %s\n", ingredientNames[0]);
            break;
        case 'N':
            wholesalerIngredients[1]++;
            printf("\nWholesaler has bought %s\n", ingredientNames[1]);
            break;
        case 'C':
            wholesalerIngredients[2]++;
            printf("\nWholesaler has bought %s\n", ingredientNames[2]);
            break;
        case 'S':
            wholesalerIngredients[3]++;
            printf("\nWholesaler has bought %s\n", ingredientNames[3]);
            break;
        default:
            break;
    }
}

void *wholesaler(void *arg) {
    int i=0;
    char *inputFile = (char *) arg;
    FILE *fp;
    fp = fopen(inputFile, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }
    char c1, c2;
    while (fscanf(fp, "%c%c\n", &c1, &c2) != EOF) {
        addIngredient(c1);
        sleep(1);
        addIngredient(c2);
        sleep(1);
    }
    while (isEnoughIngredients()) {
        for (i = 0; i < 6; i++) {
            if (wholesalerIngredients[chefs[i].neededIngredients[0]] > 0 && wholesalerIngredients[chefs[i].neededIngredients[1]] > 0) {
                sem_wait(&wholesalerSem);
                printf("\nWholesaler has %d egg, %d starch, %d walnut and %d sugar\n", wholesalerIngredients[0],
                       wholesalerIngredients[1], wholesalerIngredients[2], wholesalerIngredients[3]);
                printf("\nWholesaler has delivered %s and %s to the chef %d\n\n", ingredientNames[chefs[i].neededIngredients[0]],
                       ingredientNames[chefs[i].neededIngredients[1]], i);
                wholesalerIngredients[chefs[i].neededIngredients[0]]--;
                wholesalerIngredients[chefs[i].neededIngredients[1]]--;
                sleep(1);
                sem_post(&chefs[i].sem);
            }
        }
    }
    sleep(5);
    printf("---------------------------\n");
    printf("Wholesaler hasn't enough ingredients to make baklava\n");
    printf("---------------------------\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return 1;
    }
    sem_init(&wholesalerSem, 0, 1);
    pthread_t wholesalerThread;
    for (int i = 0; i < 6; i++) {
        sem_init(&chefs[i].sem, 0, 0);
        chefs[i].neededIngredients[0] = i % 4;
        chefs[i].neededIngredients[1] = (i < 4 ? i+1: i+2) % 4;
        pthread_create(&chefs[i].thread, NULL, chef, (void *) i);
    }

    char *inputFile = argv[1];
    pthread_create(&wholesalerThread, NULL, wholesaler, (void *) inputFile);
    pthread_join(wholesalerThread, NULL);

    for (int i = 0; i < 6; i++) {
        pthread_cancel(chefs[i].thread);
        sem_destroy(&chefs[i].sem);
    }

    sem_destroy(&wholesalerSem);
    return 0;
}
