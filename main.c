#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t wholesalerSem;
sem_t chefSem;
char *ingredientNames[4] = {"Egg", "Starch", "Walnut", "Sugar"};
int wholesalerIngredients[4] = {5, 5, 5, 5};
struct chef {
    pthread_t thread;
    int neededIngredients[2];
} chefs[6];


void *chef(void *arg) {
    int chefId = (int) arg;
    printf("Chef %d is waiting for %s and %s\n", chefId, ingredientNames[chefs[chefId].neededIngredients[0]], ingredientNames[chefs[chefId].neededIngredients[1]]);
    sem_wait(&chefSem);
    do {
        sem_wait(&wholesalerSem);
        if (wholesalerIngredients[chefs[chefId].neededIngredients[0]] > 0 && wholesalerIngredients[chefs[chefId].neededIngredients[1]] > 0) {
            printf("Wholesaler has delivered %s and %s to chef %d\n", ingredientNames[chefs[chefId].neededIngredients[0]], ingredientNames[chefs[chefId].neededIngredients[1]], chefId);
            sleep(1);
            wholesalerIngredients[chefs[chefId].neededIngredients[0]]--;
            wholesalerIngredients[chefs[chefId].neededIngredients[1]]--;
            break;
        }
        sem_post(&wholesalerSem);
    } while (1);
    printf("Chef %d is preparing the baklava\n", chefId);
    sleep(1);
    printf("Chef %d has delivered the baklava to the wholesaler\n", chefId);
    sleep(1);
    sem_post(&chefSem);
    printf("Wholesaler has obtained the baklava and left to sell it\n");
    sleep(1);
    sem_post(&wholesalerSem);
}

int main() {
    sem_init(&wholesalerSem, 0, 1);
    for (int i = 0; i < 6; i++) {
        sem_init(&chefSem, 0, 1);
        chefs[i].neededIngredients[0] = i % 4;
        chefs[i].neededIngredients[1] = (i < 4 ? i+1: i+2) % 4;
        pthread_create(&chefs[i].thread, NULL, chef, (void *) i);
    }

    for (int i = 0; i < 6; i++) {
        pthread_join(chefs[i].thread, NULL);
    }

    return 0;
}
