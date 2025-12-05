#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUD_FILE "students.txt"
#define CRE_FILE  "credentials.txt"

char currentUser[50];
char currentRole[20];

// Utility function to reverse a string (simple "encryption")
void reverseStr(char *str) {
    int n = strlen(str);
    for(int i = 0; i < n/2; i++) {
        char temp = str[i];
        str[i] = str[n-i-1];
        str[n-i-1] = temp;
    }
}

int login() {
    char u[50], p[50], r[20];
    char inUser[50], inPass[50];

    printf("USERNAME: ");
    scanf("%s", inUser);
    printf("PASSWORD: ");
    scanf("%s", inPass);


    FILE *fp = fopen(CRE_FILE, "r");
    if (!fp) {
        printf("Credential file missing!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", u, p, r) == 3) {
        if (strcmp(inUser, u) == 0 && strcmp(inPass, p) == 0) {
            strcpy(currentUser, u);
            strcpy(currentRole, r);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void addStudent() {
    int roll;
    char name[50];
    float mark;

    printf("Roll: ");
    scanf("%d", &roll);
    printf("Name: ");
    scanf(" %[^\n]", name);
    printf("Mark: ");
    scanf("%f", &mark);

    FILE *fp = fopen(STUD_FILE, "a");
    fprintf(fp, "%d %s %.2f\n", roll, name, mark);
    fclose(fp);

    printf("Student added!\n");
}

void displayStudents() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    int roll;
    char name[50];
    float mark;

    printf("Roll\tName\tMark\n");
    printf("----\t----\t----\n");
    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        printf("%d\t%s\t%.2f\n", roll, name, mark);
    }

    fclose(fp);
}

void searchStudent() {
    int find, roll;
    char name[50];
    float mark;

    printf("Enter roll to search: ");
    scanf("%d", &find);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll == find) {
            printf("Found: %d %s %.2f\n", roll, name, mark);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    printf("Student not found!\n");
}

void deleteStudent() {
    int delRoll;
    printf("Enter roll to delete: ");
    scanf("%d", &delRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll != delRoll) {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student deleted!\n");
    else printf("Roll not found!\n");
}

void updateStudent() {
    int updateRoll;
    printf("Enter roll to update: ");
    scanf("%d", &updateRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll == updateRoll) {
            found = 1;
            char newName[50];
            float newMark;

            printf("New Name: ");
            scanf(" %[^\n]", newName);
            printf("New Mark: ");
            scanf("%f", &newMark);

            fprintf(temp, "%d %s %.2f\n", roll, newName, newMark);
        } else {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student updated!\n");
    else printf("Roll not found!\n");
}

// Password change function
void changePassword() {
    char oldPass[50], newPass[50], confirmPass[50];
    char u[50], p[50], r[20];

    printf("Enter current password: ");
    scanf("%s", oldPass);


    FILE *fp = fopen(CRE_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    int found = 0;

    if (!fp) {
        printf("Credential file missing!\n");
        return;
    }

    while (fscanf(fp, "%s %s %s", u, p, r) == 3) {
        if (strcmp(u, currentUser) == 0) {
            if (strcmp(p, oldPass) == 0) {
                found = 1;
                printf("Enter new password: ");
                scanf("%s", newPass);
                printf("Confirm new password: ");
                scanf("%s", confirmPass);

                if (strcmp(newPass, confirmPass) != 0) {
                    printf("Passwords do not match!\n");
                    fprintf(temp, "%s %s %s\n", u, p, r); // keep old password
                } else {
                    reverseStr(newPass);
                    fprintf(temp, "%s %s %s\n", u, newPass, r);
                    printf("Password changed successfully!\n");
                }
            } else {
                printf("Current password incorrect!\n");
                fprintf(temp, "%s %s %s\n", u, p, r);
            }
        } else {
            fprintf(temp, "%s %s %s\n", u, p, r);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(CRE_FILE);
    rename("temp.txt", CRE_FILE);

    if (!found) printf("User not found!\n");
}

void adminMenu() {
    int c;
    while (1) {
        printf("\nADMIN MENU\n");
        printf("1.Add\n2.Display\n3.Search\n4.Update\n5.Delete\n6.Change Password\n7.Logout\n");
        scanf("%d",&c);
        if(c==1)addStudent();
        else if(c==2)displayStudents();
        else if(c==3)searchStudent();
        else if(c==4)updateStudent();
        else if(c==5)deleteStudent();
        else if(c==6)changePassword();
        else return;
    }
}

void staffMenu() {
    int c;
    while (1) {
        printf("\nSTAFF MENU\n");
        printf("1.Add\n2.Display\n3.Search\n4.Update\n5.Change Password\n6.Logout\n");
        scanf("%d",&c);
        if(c==1)addStudent();
        else if(c==2)displayStudents();
        else if(c==3)searchStudent();
        else if(c==4)updateStudent();
        else if(c==5)changePassword();
        else return;
    }
}

void guestMenu() {
    int c;
    while (1) {
        printf("\nGUEST MENU\n");
        printf("1.Display\n2.Search\n3.Change Password\n4.Logout\n");
        scanf("%d",&c);
        if(c==1)displayStudents();
        else if(c==2)searchStudent();
        else if(c==3)changePassword();
        else return;
    }
}

int main() {
    if (!login()) {
        printf("Invalid login!\n");
        return 0;
    }

    printf("Logged in as: %s\n", currentRole);

    if (strcmp(currentRole,"admin")==0) adminMenu();
    else if (strcmp(currentRole,"staff")==0) staffMenu();
    else guestMenu();

    return 0;
}