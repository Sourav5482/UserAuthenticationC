#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define maxPassUser 50
#define MAX_USER 100

typedef struct User {
    char userName[maxPassUser];
    char passWord[maxPassUser];
} User;

User user[MAX_USER];

// Function to remove newline from fgets input
void fgetHandle(char* string) {
    string[strcspn(string, "\n")] = '\0';
}

// Function to input password (showing * for each character immediately)
void inputPassword(char *password) {
    struct termios oldt, newt;
    int i = 0;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("Enter password: ");
    fflush(stdout);

    while (1) {
        ch = getchar();
        if (ch == '\n' || ch == '\r') {
            break;
        }
        else if (ch == 127 || ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
        }
        else if (i < maxPassUser - 1) {
            password[i++] = ch;
            printf("*");
            fflush(stdout);
        }
    }
    password[i] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
}

// Registration function
int registration() {
    char username[maxPassUser];
    char passwordInput[maxPassUser];

    printf("Enter your username: ");
    fgets(username, maxPassUser, stdin);
    fgetHandle(username);

    inputPassword(passwordInput);

    for (int i = 0; i < MAX_USER; i++) {
        if (user[i].userName[0] == '\0') {
            strcpy(user[i].userName, username);
            strcpy(user[i].passWord, passwordInput);
            printf("Registration successful!\n");
            return 0;
        }
    }
    printf("Registration failed! No space for new users.\n");
    return -1;
}

// Login function
void login() {
    char username[maxPassUser];
    char passwordInput[maxPassUser];

    printf("Enter your username: ");
    fgets(username, maxPassUser, stdin);
    fgetHandle(username);

    inputPassword(passwordInput);

    for (int i = 0; i < MAX_USER; i++) {
        if (strcmp(user[i].userName, username) == 0 && strcmp(user[i].passWord, passwordInput) == 0) {
            printf("Login successful!\n");
            return;
        }
    }

    printf("Invalid username or password!\n");
}

// Forgot password function
void forgotPassword() {
    char username[maxPassUser];
    printf("Enter your username: ");
    fgets(username, maxPassUser, stdin);
    fgetHandle(username);

    for (int i = 0; i < MAX_USER; i++) {
        if (strcmp(user[i].userName, username) == 0) {
            printf("Your password is: %s\n", user[i].passWord);
            return;
        }
    }

    printf("Username not found!\n");
}

// Main function
int main() {
    int input;
    while (1) {
        printf("\nPlease choose an option:\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Forgot Password\n");
        printf("4. Exit\n");
        printf("Enter your input: ");

        if (scanf("%d", &input) == 1) {
            getchar(); // clear newline
            switch (input) {
                case 1:
                    registration();
                    break;
                case 2:
                    login();
                    break;
                case 3:
                    forgotPassword();
                    break;
                case 4:
                    printf("Exiting...\n");
                    return 0;
                default:
                    printf("Invalid input! Please enter a valid option.\n");
                    break;
            }
        } else {
            printf("Please enter an integer.\n");
            while (getchar() != '\n'); // clear buffer
        }
    }

    return 0;
}