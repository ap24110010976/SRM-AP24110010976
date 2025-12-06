#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

/* Structure to store student information */
struct Student {
    int roll;
    char name[50];
    float marks;
};

/* Global variables to track logged-in user */
char currentRole[10];
char currentUser[50];

/* Function declarations */
int loginSystem();
void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();
void userMenu();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

/* -------------------------- MAIN FUNCTION -------------------------- */
int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nAccess Denied. Exiting...\n");
    }
    return 0;
}

/* -------------------------- LOGIN SYSTEM -------------------------- */
int loginSystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[10];
    int attempts = 0;

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: %s not found!\n", CREDENTIAL_FILE);
        return 0;
    }

    while (attempts < 3) {
        printf("\n=== LOGIN PAGE ===\n");
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);

        rewind(fp);
        int found = 0;

        while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
            if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
                strcpy(currentRole, fileRole);
                strcpy(currentUser, fileUser);
                printf("Login successful!\n");
                fclose(fp);
                return 1;
            }
        }

        attempts++;
        printf("Invalid credentials! Attempts left: %d\n", 3 - attempts);
    }

    fclose(fp);
    return 0;
}

/* -------------------------- MAIN MENU -------------------------- */
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else if (strcmp(currentRole, "USER") == 0)
        userMenu();
    else
        guestMenu();
}

/* -------------------------- ADMIN MENU -------------------------- */
void adminMenu() {
    int choice;
    do {
        printf("\n=== ADMIN MENU ===\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* -------------------------- STAFF MENU -------------------------- */
void staffMenu() {
    int choice;
    do {
        printf("\n=== STAFF MENU ===\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Update Student\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* -------------------------- GUEST MENU -------------------------- */
void guestMenu() {
    int choice;
    do {
        printf("\n=== GUEST MENU ===\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* -------------------------- USER MENU -------------------------- */
void userMenu() {
    int choice;
    do {
        printf("\n=== USER MENU ===\n");
        printf("1. Display Students\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* -------------------------- CRUD OPERATIONS -------------------------- */

/* Add Student */
void addStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Roll: "); scanf("%d", &s.roll);
    printf("Enter Name: "); scanf(" %[^\n]", s.name); // full name with spaces
    printf("Enter Marks: "); scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);
    printf("Student added successfully!\n");
}

/* Display all students */
void displayStudents() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    printf("\n--- Student Records ---\n");
    while (fscanf(fp, "%d %[^\n] %f", &s.roll, s.name, &s.marks) == 3) {
        printf("Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
}

/* Search student */
void searchStudent() {
    struct Student s;
    int choice, roll;
    char name[50];
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    printf("Search by: 1.Roll 2.Name : ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter Roll: ");
        scanf("%d", &roll);
        while (fscanf(fp, "%d %[^\n] %f", &s.roll, s.name, &s.marks) == 3) {
            if (s.roll == roll) {
                printf("Found => Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
                fclose(fp);
                return;
            }
        }
    } else {
        printf("Enter Name: ");
        scanf(" %[^\n]", name);
        while (fscanf(fp, "%d %[^\n] %f", &s.roll, s.name, &s.marks) == 3) {
            if (strcmp(s.name, name) == 0) {
                printf("Found => Roll: %d | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
                fclose(fp);
                return;
            }
        }
    }

    printf("Student not found!\n");
    fclose(fp);
}

/* Update student */
void updateStudent() {
    struct Student s;
    int roll, found = 0;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Roll to update: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %[^\n] %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            printf("Enter New Name: ");
            scanf(" %[^\n]", s.name);
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Student updated successfully!\n");
    else
        printf("Roll not found!\n");
}

/* Delete student */
void deleteStudent() {
    struct Student s;
    int roll, found = 0;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Roll to delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %[^\n] %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Student deleted successfully!\n");
    else
        printf("Roll not found!\n");
}
