#define _XOPEN_SOURCE 600 // Required for barriers
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_STUDENTS 10

// Global variables
int total_grade = 0;
float total_bellcurve = 0;
int student_grades[NUM_STUDENTS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

// Function to read grades from file
void* read_grades(void* arg) {
    FILE* file = fopen("grades.txt", "r");
    if (file == NULL) {
        perror("Error opening grades.txt");
        exit(1);
    }

    printf("Reading grades from file...\n");
    for (int i = 0; i < NUM_STUDENTS; i++) {
        fscanf(file, "%d", &student_grades[i]);
    }
    fclose(file);

    pthread_barrier_wait(&barrier); // Synchronize before proceeding
    pthread_exit(NULL);
}

// Function to process grades and save bellcurved grades
void* save_bellcurve(void* arg) {
    int grade = *(int*)arg;
    float bellcurved_grade = grade * 1.50;

    pthread_mutex_lock(&mutex);
    total_grade += grade;
    total_bellcurve += bellcurved_grade;
    pthread_mutex_unlock(&mutex);

    FILE* file = fopen("bellcurve.txt", "a");
    if (file == NULL) {
        perror("Error opening bellcurve.txt");
        exit(1);
    }

    fprintf(file, "%.2f\n", bellcurved_grade);
    fclose(file);

    pthread_exit(NULL);
}

int main(void) {
    pthread_t reader_thread, threads[NUM_STUDENTS];

    // Initialize barrier for 2 sync points (reader + main thread)
    pthread_barrier_init(&barrier, NULL, 2);

    // Create thread to read grades
    pthread_create(&reader_thread, NULL, read_grades, NULL);
    pthread_barrier_wait(&barrier); // Wait for reading to finish

    // Create threads to process grades
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_create(&threads[i], NULL, save_bellcurve, (void*)&student_grades[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print total grades and averages
    printf("Total Grade Before Bellcurve: %d\n", total_grade);
    printf("Class Average Before Bellcurve: %.2f\n", (float)total_grade / NUM_STUDENTS);
    printf("Total Grade After Bellcurve: %.2f\n", total_bellcurve);
    printf("Class Average After Bellcurve: %.2f\n", total_bellcurve / NUM_STUDENTS);

    // Destroy the barrier
    pthread_barrier_destroy(&barrier);

    return 0;
}
