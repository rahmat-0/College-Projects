#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TOTAL 5

//One question = text + 4 options + correct answer
struct Question {
    char text[200];
    char options[4][60];
    char correct;          //'A', 'B', 'C' or 'D' 
};

// Reads a valid answer (A-D) from the user 
char getAnswer(void) {
    char ch;
    while (1) {
        printf("Your answer (A/B/C/D): ");
        if (scanf(" %c", &ch) != 1) {
            exit(0);      
        }
        ch = toupper(ch);
        if (ch >= 'A' && ch <= 'D') {
            return ch;
        }
        printf("Invalid choice! Please enter A, B, C or D.\n");
    }
}

//Displays one question
void showQuestion(struct Question q, int no) {
    printf("\nQ%d. %s\n", no, q.text);
    for (int i = 0; i < 4; i++) {
        printf("   %c) %s\n", 'A' + i, q.options[i]);
    }
}

//Prints the final result with percentage and remark
void showResult(int score) {
    float percent = (score * 100.0f) / TOTAL;
    printf("\n=== RESULT ===\n");
    printf("Score      : %d / %d\n", score, TOTAL);
    printf("Percentage : %.1f%%\n", percent);
    if (percent >= 80)      printf("Remark     : Excellent!\n");
    else if (percent >= 60) printf("Remark     : Good job!\n");
    else if (percent >= 40) printf("Remark     : Fair, keep practicing.\n");
    else                    printf("Remark     : Needs improvement.\n");
    printf("===\n");
}

//File handling: save the score permanently
void saveScore(char name[], int score) {
    FILE *fp = fopen("scores.txt", "a");
    if (fp == NULL) {
        printf("Could not save score.\n");
        return;
    }
    fprintf(fp, "%s %d/%d\n", name, score, TOTAL);
    fclose(fp);
}

// File handling: show all previous scores
void viewScores(void) {
    char line[100];
    FILE *fp = fopen("scores.txt", "r");
    if (fp == NULL) {
        printf("\nNo scores saved yet.\n");
        return;
    }
    printf("\n------ Previous Scores ------\n");
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }
    fclose(fp);
}

void startQuiz(void) {
    struct Question q[TOTAL] = {
        {"Which symbol ends a statement in C?",
         {":", ";", ".", ","}, 'B'},
        {"Which header file is needed for printf()?",
         {"stdlib.h", "string.h", "stdio.h", "math.h"}, 'C'},
        {"What is the index of the first element of an array in C?",
         {"0", "1", "-1", "Depends on compiler"}, 'A'},
        {"Which operator gives the address of a variable?",
         {"*", "&", "#", "@"}, 'B'},
        {"Which loop always executes at least once?",
         {"for", "while", "do-while", "None of these"}, 'C'}
    };

    char name[50];
    int score = 0;

    printf("\nEnter your name: ");
    scanf(" %49[^\n]", name);
    printf("\nWelcome %s! Let's begin the quiz.\n", name);

    for (int i = 0; i < TOTAL; i++) {
        showQuestion(q[i], i + 1);
        char ans = getAnswer();
        if (ans == q[i].correct) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Wrong! Correct answer is %c.\n", q[i].correct);
        }
    }

    showResult(score);
    saveScore(name, score);
}

int main(void) {
    int choice;
    do {
        printf("\n===== C QUIZ APPLICATION =====\n");
        printf("1. Start Quiz\n");
        printf("2. View Previous Scores\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            break;
        }
        switch (choice) {
            case 1: startQuiz();  break;
            case 2: viewScores(); break;
            case 3: printf("Thank you! Goodbye.\n"); break;
            default: printf("Invalid choice, try again.\n");
        }
    } while (choice != 3);
    return 0;
}