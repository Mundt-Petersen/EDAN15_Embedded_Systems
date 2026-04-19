#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int x = 0;  // shared variable

pthread_mutex_t lock;

// Thread 1: increment every 20 ms
void* increment_thread(void* arg) {
    while (1) {
        usleep(20000); // 20 ms
        pthread_mutex_lock(&lock);
        x++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// Thread 2: decrement every 200 ms
void* decrement_thread(void* arg) {
    while (1) {
        usleep(200000); // 200 ms
        pthread_mutex_lock(&lock);
        x -= 10;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// Thread 3: print every 400 ms
void* print_thread(void* arg) {
    while (1) {
        usleep(400000); // 400 ms
        pthread_mutex_lock(&lock);
        printf("x = %d\n", x);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, increment_thread, NULL);
    pthread_create(&t2, NULL, decrement_thread, NULL);
    pthread_create(&t3, NULL, print_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}

// Compared to LF this is concurrent and non-dterminisic. Each cycle gives around +-0 but the race conditions cause drift over time.

// I think LF is easier to read bcs it is shorter and has less "fill". This one needs locks bcs otherwise the different threads would try to change x at the same time.

// LF solution is allways the same, re-ordering it until expected solution is allways reached would probably be best.

// Compile: gcc alignmentchallenge.c -o run -lpthread
// Run: ./run