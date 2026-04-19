#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

typedef enum { Bread=0, Cheese=1, Ham=2 } Ingredient;

// Semaphores for each student
sem_t s1_sem, s2_sem, s3_sem;

// Signals from supplier
sem_t bread, cheese, ham;

// Mutex for printing
pthread_mutex_t lock;

// Helper: random ingredient pair
void pick_two(Ingredient* a, Ingredient* b) {
    *a = rand() % 3;
    do {
        *b = rand() % 3;
    } while (*b == *a);
}

// Supplier thread
void* supplier(void* arg) {
    while (1) {
        Ingredient i1, i2;
        pick_two(&i1, &i2);

        pthread_mutex_lock(&lock);
        printf("Supplier puts: %d and %d -> ", i1, i2);
        pthread_mutex_unlock(&lock);

        // Signal ingredients
        if (i1 == Bread || i2 == Bread) sem_post(&bread);
        if (i1 == Cheese || i2 == Cheese) sem_post(&cheese);
        if (i1 == Ham || i2 == Ham) sem_post(&ham);

        usleep(1000000); // slow things down a bit
    }
    return NULL;
}

// Pusher threads (coordinate ingredients)
sem_t mutex;
int has_bread = 0, has_cheese = 0, has_ham = 0;

void* bread_pusher(void* arg) {
    while (1) {
        sem_wait(&bread);
        sem_wait(&mutex);

        if (has_cheese) {
            has_cheese = 0;
            sem_post(&s3_sem); // ham student
        } else if (has_ham) {
            has_ham = 0;
            sem_post(&s2_sem); // cheese student
        } else {
            has_bread = 1;
        }

        sem_post(&mutex);
    }
}

void* cheese_pusher(void* arg) {
    while (1) {
        sem_wait(&cheese);
        sem_wait(&mutex);

        if (has_bread) {
            has_bread = 0;
            sem_post(&s3_sem);
        } else if (has_ham) {
            has_ham = 0;
            sem_post(&s1_sem);
        } else {
            has_cheese = 1;
        }

        sem_post(&mutex);
    }
}

void* ham_pusher(void* arg) {
    while (1) {
        sem_wait(&ham);
        sem_wait(&mutex);

        if (has_bread) {
            has_bread = 0;
            sem_post(&s2_sem);
        } else if (has_cheese) {
            has_cheese = 0;
            sem_post(&s1_sem);
        } else {
            has_ham = 1;
        }

        sem_post(&mutex);
    }
}

// Student threads

void* student_bread(void* arg) {
    while (1) {
        sem_wait(&s1_sem);
        pthread_mutex_lock(&lock);
        printf("s1 eats.\n");
        pthread_mutex_unlock(&lock);
    }
}

void* student_cheese(void* arg) {
    while (1) {
        sem_wait(&s2_sem);
        pthread_mutex_lock(&lock);
        printf("s2 eats.\n");
        pthread_mutex_unlock(&lock);
    }
}

void* student_ham(void* arg) {
    while (1) {
        sem_wait(&s3_sem);
        pthread_mutex_lock(&lock);
        printf("s3 eats.\n");
        pthread_mutex_unlock(&lock);
    }
}

int main() {
    srand(time(NULL));

    pthread_t sup, b_p, c_p, h_p;
    pthread_t s1, s2, s3;

    // Init semaphores
    sem_init(&bread, 0, 0);
    sem_init(&cheese, 0, 0);
    sem_init(&ham, 0, 0);

    sem_init(&s1_sem, 0, 0);
    sem_init(&s2_sem, 0, 0);
    sem_init(&s3_sem, 0, 0);

    sem_init(&mutex, 0, 1);

    pthread_mutex_init(&lock, NULL);

    // Create threads
    pthread_create(&sup, NULL, supplier, NULL);
    pthread_create(&b_p, NULL, bread_pusher, NULL);
    pthread_create(&c_p, NULL, cheese_pusher, NULL);
    pthread_create(&h_p, NULL, ham_pusher, NULL);

    pthread_create(&s1, NULL, student_bread, NULL);
    pthread_create(&s2, NULL, student_cheese, NULL);
    pthread_create(&s3, NULL, student_ham, NULL);

    // Join (runs forever)
    pthread_join(sup, NULL);

    return 0;
}

// I think the LF implementation is more is easier to  understans and debug, ticket systems and things running in parallel is confusing...

// Because LF is ordered and deterministic by nature, with multiple threads locks are needed to prevent race conditions and semaphores help coordinate the operations.

// Compile: gcc simultaneitychallenge.c -o run -lpthread
// Run: ./run