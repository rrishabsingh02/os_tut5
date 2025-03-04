#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_STUDENTS 5
#define MAX_NAME_LENGTH 50

// Define a struct for student information
struct Student {
    char name[MAX_NAME_LENGTH];
    int student_id;
    int grade;
};

// Function to apply a bell curve to the grade
void* bellcurve(void* arg) {
    struct Student* student = (struct Student*)arg;
    float bellcurved_grade = student->grade * 1.50; // Apply bell curve

    printf("Student: %s, ID: %d, Original Grade: %d, Bellcurved Grade: %.2f\n",
           student->name, student->student_id, student->grade, bellcurved_grade);

    pthread_exit(NULL);
}

int main(void) {
    pthread_t threads[NUM_STUDENTS];
    struct Student students[NUM_STUDENTS];

    // Prompt professor for student information
    printf("Enter information for %d students:\n", NUM_STUDENTS);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        printf("Student %d:\n", i + 1);
        printf("Name: ");
        scanf("%s", students[i].name);
        printf("Student ID: ");
        scanf("%d", &students[i].student_id);
        printf("Grade: ");
        scanf("%d", &students[i].grade);
    }

    // Create threads after receiving all student information
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_create(&threads[i], NULL, bellcurve, (void*)&students[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
