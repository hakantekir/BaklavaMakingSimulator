#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

char *ingredientNames[4] = {"Egg", "Starch", "Walnut", "Sugar"};
sem_t sem;

void *chef(void *arg) {
    int neededIngredients[2];
    int chefId = (int) arg;
    neededIngredients[0] = chefId % 4;
    neededIngredients[1] = (chefId < 4 ? chefId+1: chefId+2) % 4;
    printf("Chef %d is waiting for %s and %s\n", chefId, ingredientNames[neededIngredients[0]], ingredientNames[neededIngredients[1]]);
    sem_wait(&sem);

    sem_post(&sem);
    pthread_exit(NULL);
}

int main() {
    sem_init(&sem, 0, 1);
    pthread_t chefs[6];
    for (int i = 0; i < 6; i++) {
        pthread_create(&chefs[i], NULL, chef, (void *) i);
    }
    for (int i = 0; i < 6; i++) {
        pthread_join(chefs[i], NULL);
    }
    sem_destroy(&sem);

    return 0;
}
