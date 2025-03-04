#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_STUDENTS 10

// Global variable for total grade
int total_grade = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for synchronization

// Function to add grade to total_grade with mutual exclusion
void* class_total(void* arg) {
    int grade = *(int*)arg;

    pthread_mutex_lock(&mutex); // Lock the mutex
    total_grade += grade;
    pthread_mutex_unlock(&mutex); // Unlock the mutex

    pthread_exit(NULL);
}

int main(void) {
    pthread_t threads[NUM_STUDENTS];
    int student_grades[NUM_STUDENTS];

    // Prompt professor for student grades
    printf("Enter grades for %d students:\n", NUM_STUDENTS);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        printf("Student %d: ", i + 1);
        scanf("%d", &student_grades[i]);
    }

    // Create threads after receiving all grades
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_create(&threads[i], NULL, class_total, (void*)&student_grades[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the total class grade
    printf("The total grade of the class is: %d\n", total_grade);

    return 0;
}
