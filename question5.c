#define _XOPEN_SOURCE 600 // Required for POSIX compatibility
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
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int grades_read = 0;

// Function to read grades from file
void* read_grades(void* arg) {
    FILE* file = fopen("grades-1.txt", "r");
    if (file == NULL) {
        perror("Error opening grades.txt");
        exit(1);
    }

    printf("Reading grades from file...\n");
    for (int i = 0; i < NUM_STUDENTS; i++) {
        fscanf(file, "%d", &student_grades[i]);
    }
    fclose(file);

    // Signal all threads that grades are available
    pthread_mutex_lock(&lock);
    grades_read = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

// Function to process grades and save bellcurved grades
void* save_bellcurve(void* arg) {
    pthread_mutex_lock(&lock);
    while (!grades_read) {
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);

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

    // Create thread to read grades
    pthread_create(&reader_thread, NULL, read_grades, NULL);

    // Create threads to process grades
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_create(&threads[i], NULL, save_bellcurve, (void*)&student_grades[i]);
    }

    // Wait for all threads to finish
    pthread_join(reader_thread, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print total grades and averages (Fixed print statements)
    printf("Total Grade Before Bellcurve: %d\n", total_grade);
    printf("Class Average Before Bellcurve: %.2f\n", (float)total_grade / NUM_STUDENTS);
    printf("Total Grade After Bellcurve: %.2f\n", total_bellcurve);
    printf("Class Average After Bellcurve: %.2f\n", total_bellcurve / NUM_STUDENTS);

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
