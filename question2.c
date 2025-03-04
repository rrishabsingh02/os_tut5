#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_STUDENTS 5

// Function to apply a bell curve to the grade
void* bellcurve(void* arg) {
    int grade = *(int*)arg;
    float bellcurved_grade = grade * 1.50; // Apply bell curve

    printf("Original Grade: %d, Bellcurved Grade: %.2f\n", grade, bellcurved_grade);

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
        pthread_create(&threads[i], NULL, bellcurve, (void*)&student_grades[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
