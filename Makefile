CC = clang
CFLAGS = -pthread -std=gnu99
TARGETS = question1 question2 question3 question4 question5

all: $(TARGETS)

question1: question1.c
	$(CC) $(CFLAGS) question1.c -o question1

question2: question2.c
	$(CC) $(CFLAGS) question2.c -o question2

question3: question3.c
	$(CC) $(CFLAGS) question3.c -o question3

question4: question4.c
	$(CC) $(CFLAGS) question4.c -o question4

question5: question5.c
	$(CC) $(CFLAGS) question5.c -o question5

clean:
	rm -f $(TARGETS)
