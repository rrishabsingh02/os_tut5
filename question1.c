#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // for sleep
#include <time.h>   // for random sleep duration

// Function to print "Hello, World!" after a random delay
void* hello_world(void* arg) {
    int sleep_time = rand() % 3 + 1; // Random sleep between 1-3 seconds
    sleep(sleep_time);
    printf("Hello, World!\n");
    pthread_exit(NULL);
}

// Function to print "Goodbye!" after a random delay
void* goodbye(void* arg) {
    int sleep_time = rand() % 3 + 1; // Random sleep between 1-3 seconds
    sleep(sleep_time);
    printf("Goodbye!\n");
    pthread_exit(NULL);
}

int main(void) {
    pthread_t thread1, thread2;

    srand(time(NULL)); // Seed the random number generator

    // Create two threads
    pthread_create(&thread1, NULL, hello_world, NULL);
    pthread_create(&thread2, NULL, goodbye, NULL);

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
