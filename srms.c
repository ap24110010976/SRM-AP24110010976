#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIALS_FILE "credentials.txt"
#define MAX_CREDENTIALS_LEN 50
#define MAX_ROLE_LEN 10

char CURRENT_USER_ROLE[MAX_ROLE_LEN] = ""; 
char CURRENT_USERNAME[MAX_CREDENTIALS_LEN] = "";

struct Student {
    int roll;
    char name[50];
    float marks;
};

struct User {
    char username[MAX_CREDENTIALS_LEN];
    char password[MAX_CREDENTIALS_LEN];
    char role[MAX_ROLE_LEN];
};

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

void adminMenu();
void staffMenu();
void guestMenu();

int login();
void mainMenuDispatcher();

int main() {
    mainMenuDispatcher();
    return 0;
}

void mainMenuDispatcher() {
    int logged_in = login();

    if (logged_in) {
        if (strcmp(CURRENT_USER_ROLE, "ADMIN") == 0) {
            adminMenu();
        } else if (strcmp(CURRENT_USER_ROLE, "STAFF") == 0) {
            staffMenu();
        } else if (strcmp(CURRENT_USER_ROLE, "GUEST") == 0) {
            guestMenu();
        } else {
            printf("Error: Unknown role assigned. Exiting.\n");
        }
    } else {
        printf("Exiting application.\n");
    }
}

int login() {
    char input_username[MAX_CREDENTIALS_LEN];
    char input_password[MAX_CREDENTIALS_LEN];
    struct User user;
    FILE *fp = fopen(CREDENTIALS_FILE, "r");
    int logged_in = 0;
    int c;

    printf("\n==== Login Screen ====\n");
    printf("Username: ");
    scanf("%s", input_username);
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Password: ");
    scanf("%s", input_password); 
    while ((c = getchar()) != '\n' && c != EOF);

    if (!fp) {
        printf("Error: Credentials file not found (%s).\n", CREDENTIALS_FILE);
        printf("Login failed.\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", user.username, user.password, user.role) == 3) {
        if (strcmp(input_username, user.username) == 0 && strcmp(input_password, user.password) == 0) {
            strcpy(CURRENT_USER_ROLE, user.role);
            strcpy(CURRENT_USERNAME, user.username);
            printf("Login successful. Welcome, %s (%s).\n", CURRENT_USERNAME, CURRENT_USER_ROLE);
            logged_in = 1;
            break;
        }
    }

    fclose(fp);

    if (!logged_in) {
        printf("Login failed: Invalid username or password.\n");
    }

    return logged_in;
}

void addStudent() {
    FILE *fp = fopen(STUDENT_FILE, "a");
    struct Student st;

    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    printf("Enter Roll No: ");
    scanf("%d", &st.roll);
    getchar();

    printf("Enter Name: ");
    scanf(" %49[^\n]", st.name);

    printf("Enter Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("Student Added Successfully!\n");
}

void displayStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;

    if (!fp) {
        printf("No Student records found\n");
        return;
    }

    printf("\nRoll\tName\tMarks\n");
    printf("------------------------------\n");

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        printf("%d\t%s\t%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
}

void searchStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;
    int roll, found = 0;

    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    printf("Enter Roll to search: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("Record found\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Record not found\n");
    }

    fclose(fp);
}

void updateStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student st;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("Error opening file\n");
        return;
    }

    printf("Enter Roll to update: ");
    scanf("%d", &roll);
    getchar();

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("Enter new Name: ");
            scanf(" %49[^\n]", st.name);
            printf("Enter new Marks: ");
            scanf("%f", &st.marks);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Updated Successfully!\n");
    else
        printf("Record not found\n");
}

void deleteStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student st;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("Error opening file\n");
        return;
    }

    printf("Enter Roll to delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Deleted Successfully!\n");
    else
        printf("Record not found\n");
}

void adminMenu() {
    int choice;
    while (1) {
        printf("\n==== ADMIN MENU ====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");

        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice\n");
        }
    }
}

void staffMenu() {
    int choice;
    while (1) {
        printf("\n==== STAFF MENU ====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Update Student\n");
        printf("4. Logout\n");

        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4: printf("Logging out...\n"); return;
            default: printf("Invalid choice\n");
        }
    }
}

void guestMenu() {
    int choice;
    while (1) {
        printf("\n==== GUEST MENU ====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");

        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid choice\n");
        }
    }
}
