#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define USERS_FILE "users.txt"
#define USERNAME_MAX_LENGTH 50
#define PASSWORD_MAX_LENGTH 50

struct student {
    char name[30];
    char dob[11];
    char fname[30];
    char number[30];
    char contactnumber[20];
    char address[50];
    struct student* NEXT;
};

struct book {
    char title[50];
    char author[30];
    char publisher[30];
    char pub_date[11];
    char loan_date[11];
    int available;
    struct book* NEXT;
};

typedef struct Node {
    char username[USERNAME_MAX_LENGTH];
    char password[PASSWORD_MAX_LENGTH];
    struct Node* next;
} Node;

Node* head = NULL;
struct student* head_student = NULL;
struct student* tail_student = NULL;
struct book* head_book = NULL;
struct book* tail_book = NULL;
char currentUser[USERNAME_MAX_LENGTH];

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
    system("cls");
}

void showMainMenu(int selectedOption) {
    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|          Main Menu         |\n");
    printf("==============================\n");
    printf("%s1. Login\n", (selectedOption == 0 ? "> " : "  "));
    printf("%s2. Sign Up\n", (selectedOption == 1 ? "> " : "  "));
    printf("%s3. Exit\n", (selectedOption == 2 ? "> " : "  "));
    printf("==============================\n");
}

void showLoggedInMenu(int selectedOption) {
    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|       Logged In Menu       |\n");
    printf("==============================\n");
    printf("%s1. �л� �߰�\n", (selectedOption == 0 ? "> " : "  "));
    printf("%s2. ����� ��� �л� ����\n", (selectedOption == 1 ? "> " : "  "));
    printf("%s3. �л� ����\n", (selectedOption == 2 ? "> " : "  "));
    printf("%s4. å �߰�\n", (selectedOption == 3 ? "> " : "  "));
    printf("%s5. ����� ��� å ����\n", (selectedOption == 4 ? "> " : "  "));
    printf("%s6. å ����\n", (selectedOption == 5 ? "> " : "  "));
    printf("%s7. å ������\n", (selectedOption == 6 ? "> " : "  "));
    printf("%s8. å ã��\n", (selectedOption == 7 ? "> " : "  "));
    printf("%s9. �л� ã��\n", (selectedOption == 8 ? "> " : "  "));
    printf("%s10. Logout\n", (selectedOption == 9 ? "> " : "  "));
    printf("==============================\n");
}

int isUserExists(const char* username) {
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int addUser(const char* username, const char* password) {
    if (isUserExists(username)) {
        return 0;
    }
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->username, username);
    strcpy(newNode->password, password);
    newNode->next = head;
    head = newNode;
    FILE* file = fopen(USERS_FILE, "a");
    if (file != NULL) {
        fprintf(file, "%s %s\n", username, password);
        fclose(file);
    }
    return 1;
}

void handleSignUp() {
    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|          Sign Up           |\n");
    printf("==============================\n");
    char username[USERNAME_MAX_LENGTH], password[PASSWORD_MAX_LENGTH];
    printf("���ο� ID�� �Է��ϼ���: ");
    scanf("%s", username);
    printf("���ο� password�� �Է��ϼ���: ");
    scanf("%s", password);

    if (addUser(username, password)) {
        printf("ȸ�����Կ� �����Ͽ����ϴ�!\n");
    }
    else {
        printf("�̹� �����ϴ� ID�Դϴ�!\n");
    }

    system("pause");
}

int login(const char* username, const char* password) {
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0) {
            strcpy(currentUser, username);
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void loadUsersFromFile() {
    FILE* file = fopen(USERS_FILE, "r");
    if (file != NULL) {
        char username[USERNAME_MAX_LENGTH];
        char password[PASSWORD_MAX_LENGTH];
        while (fscanf(file, "%s %s", username, password) != EOF) {
            addUser(username, password);
        }
        fclose(file);
    }
}

void getStudentFileName(char* filename) {
    sprintf(filename, "%s_students.txt", currentUser);
}

void loadStudentsFromFile() {
    char filename[100];
    getStudentFileName(filename);

    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        while (!feof(file)) {
            struct student* temp = (struct student*)malloc(sizeof(struct student));
            if (fscanf(file, "%s %s %s %s %s %s", temp->name, temp->dob, temp->fname, temp->number, temp->contactnumber, temp->address) == 6) {
                temp->NEXT = NULL;
                if (head_student == NULL) {
                    head_student = temp;
                    tail_student = temp;
                }
                else {
                    tail_student->NEXT = temp;
                    tail_student = temp;
                }
            }
            else {
                free(temp);
            }
        }
        fclose(file);
    }
}

void saveStudentsToFile() {
    char filename[100];
    getStudentFileName(filename);

    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        struct student* temp = head_student;
        while (temp != NULL) {
            fprintf(file, "%s %s %s %s %s %s\n", temp->name, temp->dob, temp->fname, temp->number, temp->contactnumber, temp->address);
            temp = temp->NEXT;
        }
        fclose(file);
    }
}

void getBookFileName(char* filename) {
    sprintf(filename, "%s_books.txt", currentUser);
}

void loadBooksFromFile() {
    char filename[100];
    getBookFileName(filename);

    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        while (!feof(file)) {
            struct book* temp = (struct book*)malloc(sizeof(struct book));
            if (fscanf(file, "%s %s %s %s %s %d", temp->title, temp->author, temp->publisher, temp->pub_date, temp->loan_date, &temp->available) == 6) {
                temp->NEXT = NULL;
                if (head_book == NULL) {
                    head_book = temp;
                    tail_book = temp;
                }
                else {
                    tail_book->NEXT = temp;
                    tail_book = temp;
                }
            }
            else {
                free(temp);
            }
        }
        fclose(file);
    }
}

void saveBooksToFile() {
    char filename[100];
    getBookFileName(filename);

    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        struct book* temp = head_book;
        while (temp != NULL) {
            fprintf(file, "%s %s %s %s %s %d\n", temp->title, temp->author, temp->publisher, temp->pub_date, temp->loan_date, temp->available);
            temp = temp->NEXT;
        }
        fclose(file);
    }
}

int handleLogin() {
    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|           Login            |\n");
    printf("==============================\n");
    char username[USERNAME_MAX_LENGTH], password[PASSWORD_MAX_LENGTH];
    printf("ID�� �Է��Ͻʽÿ�: ");
    scanf("%s", username);
    printf("password�� �Է��Ͻʽÿ�: ");
    scanf("%s", password);

    if (login(username, password)) {
        printf("�α��ο� �����ϼ̽��ϴ�!\n");

        struct student* temp_student = head_student;
        while (temp_student != NULL) {
            struct student* toFree = temp_student;
            temp_student = temp_student->NEXT;
            free(toFree);
        }
        head_student = NULL;
        tail_student = NULL;

        struct book* temp_book = head_book;
        while (temp_book != NULL) {
            struct book* toFree = temp_book;
            temp_book = temp_book->NEXT;
            free(toFree);
        }
        head_book = NULL;
        tail_book = NULL;

        loadStudentsFromFile();
        loadBooksFromFile();
        system("pause");
        return 1;
    }
    else {
        printf("Login failed!\n");
        system("pause");
        return 0;
    }
}

void addStudent() {
    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|        �л� �߰��ϱ�       |\n");
    printf("==============================\n");
    getchar();
    struct student* temp = (struct student*)malloc(sizeof(struct student));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("\n�л��� �̸��� ��������: ");
    scanf("%[^\n]s", temp->name);
    getchar();

    printf("\n��������� �������� (YYYY/MM/DD): ");
    scanf("%s", temp->dob);
    getchar();

    printf("\n�й��� ���� ��������: ");
    scanf("%s", temp->contactnumber);
    getchar();

    printf("\n������ ��������: ");
    scanf("%[^\n]s", temp->fname);
    getchar();

    printf("\n��ȭ��ȣ�� ��������: ");
    scanf("%[^\n]s", temp->number);
    getchar();

    printf("\n�ּҸ� ��������: ");
    scanf("%[^\n]s", temp->address);
    getchar();

    temp->NEXT = NULL;

    if (head_student == NULL) {
        head_student = temp;
        tail_student = temp;
    }
    else {
        tail_student->NEXT = temp;
        tail_student = temp;
    }

    saveStudentsToFile();
    system("pause");
}

void displayAllStudents() {
    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|      View All Students     |\n");
    printf("==============================\n");

    struct student* temp = head_student;

    if (head_student == NULL) {
        printf("\n��� ��ϵ� �����ϴ�\n");
    }
    else {
        while (temp != NULL) {
            printf("\n�̸�: %s", temp->name);
            printf("\n�������: %s", temp->dob);
            printf("\n�й�: %s", temp->contactnumber);
            printf("\n����: %s", temp->fname);
            printf("\n��ȭ��ȣ: %s", temp->number);
            printf("\n�ּ�: %s", temp->address);
            temp = temp->NEXT;
            if (temp != NULL) {
                printf("\n-----------------------------------------");
            }
        }
    }
    system("pause");
}

void updateStudent() {
    char name[30];
    clearScreen();
    printf("==============================\n");
    printf("|         �л� �����ϱ�      |\n");
    printf("==============================\n");
    getchar();
    printf("������ �л��� �̸��� ��������: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; 

    struct student* temp = head_student;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("\nUpdating details for %s\n", temp->name);

            printf("������� (YYYY/MM/DD) (����: %s): ", temp->dob);
            scanf("%s", temp->dob);
            getchar();

            printf("�й� (����: %s): ", temp->contactnumber);
            scanf("%s", temp->contactnumber);
            getchar();

            printf("���� (����: %s): ", temp->fname);
            scanf("%[^\n]s", temp->fname);
            getchar();

            printf("��ȭ��ȣ (����: %s): ", temp->number);
            scanf("%[^\n]s", temp->number);
            getchar();


            printf("�ּ� (����: %s): ", temp->address);
            scanf("%[^\n]s", temp->address);
            getchar();

            printf("�л��� ������ ���������� �����Ͽ����ϴ�!\n");
            saveStudentsToFile();
            system("pause");
            return;
        }
        temp = temp->NEXT;
    }
    printf("�׷��� �̸��� ���� �л��� ã�� �� �����ϴ�! %s\n", name);
    system("pause");
}

void searchStudent() {
    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|           �л� ã��        |\n");
    printf("==============================\n");
    char name[30];
    printf("ã����� �л��� �̸��� �Է��ϼ���: ");
    scanf("%s", name);

    struct student* temp = head_student;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("�̸�: % s\n", temp->name);
            printf("�������: %s\n", temp->dob);
            printf("�й�: %s\n", temp->contactnumber);
            printf("����: %s\n", temp->fname);
            printf("��ȭ��ȣ: %s\n", temp->number);
            printf("�ּ�: %s\n", temp->address);
            printf("-----------------------------\n");
            system("pause");
            return;
        }
        temp = temp->NEXT;
    }
    printf("�л��� ã�� �� �����ϴ�!\n");
    system("pause");
}

void addBook() {

    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|          å �߰��ϱ�       |\n");
    printf("==============================\n");

    struct book* temp = (struct book*)malloc(sizeof(struct book));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("������ �Է��ϼ���: ");
    scanf("%s", temp->title);
    printf("�۰��� �Է��ϼ���: ");
    scanf("%s", temp->author);
    printf("���ǻ縦 �Է��ϼ���: ");
    scanf("%s", temp->publisher);
    printf("�������� �Է��ϼ���: (YYYY/MM/DD): ");
    scanf("%s", temp->pub_date);

    temp->available = 1;
    temp->NEXT = NULL;

    strcpy(temp->loan_date, "");
    temp->available = 1;

    if (head_book == NULL) {
        head_book = temp;
        tail_book = temp;
    }
    else {
        tail_book->NEXT = temp;
        tail_book = temp;
    }

    saveBooksToFile();
    printf("���������� å�� �߰��Ͽ����ϴ�!\n");
    system("pause");
}

void displayAllBooks() {

    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|          ��� å ����      |\n");
    printf("==============================\n");

    struct book* temp = head_book;

    if (head_book == NULL) {
        printf("\n����� �����ϴ�!!\n");
    }
    else {
        while (temp != NULL) {
            printf("����: %s\n", temp->title);
            printf("�۰�: %s\n", temp->author);
            printf("���ǻ�: %s\n", temp->publisher);
            printf("������: %s\n", temp->pub_date);
            printf("������: %s\n", temp->loan_date);
            printf("�̿����: %s\n", temp->available ? "����" : "�Ұ���");
            printf("==============================\n");
            temp = temp->NEXT;
        }
    }
    system("pause");
}

void updateBook() {
    char title[50];
    clearScreen();
    printf("==============================\n");
    printf("|          å ������Ʈ       |\n");
    printf("==============================\n");
    getchar();
    printf("Enter the title of the book to update: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    struct book* temp = head_book;
    while (temp != NULL) {
        if (strcmp(temp->title, title) == 0) {
            printf("\nUpdating details for %s\n", temp->title);

            printf("�۰�: ");
            scanf("%s", temp->author);
            printf("���ǻ�: ");
            scanf("%s", temp->publisher);
            printf("������ (YYYY/MM/DD): ");
            scanf("%s", temp->pub_date);

            printf("å�� ������ ���������� �����Ͽ����ϴ�!\n");
            saveBooksToFile();
            system("pause");
            return;
        }
        temp = temp->NEXT;
    }
    printf("�׷��� ������ ���� å�� ã�� �� �����ϴ�! %s\n", title);
    system("pause");
}

void handleLoanBook() {
    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|             ����            |\n");
    printf("==============================\n");

    char title[50];
    printf("���� �Ǵ� �ݳ��� å�� �̸��� �����ּ���:");
    scanf("%s", title);

    struct book* temp = head_book;
    while (temp != NULL) {
        if (strcmp(temp->title, title) == 0) {
            if (temp->available) {
                printf("������ (YYYY/MM/DD): ");
                scanf("%s", temp->loan_date);
                temp->available = 0;
                printf("å�� ���������� �����Ͽ����ϴ�!\n");
            }
            else {
                strcpy(temp->loan_date, "");
                temp->available = 1;
                printf("���������� �ݳ��Ͽ����ϴ�!\n");
            }

            saveBooksToFile();

            system("pause");
            return;
        }
        temp = temp->NEXT;
    }

    printf("å�� ã�� �� �����ϴ�!\n");
    system("pause");
}

void searchBook() {
    char title[50];
    clearScreen();
    printf("==============================\n");
    printf("|          å �˻��ϱ�       |\n");
    printf("==============================\n");
    getchar();
    printf("Enter the title of the book to search: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    struct book* temp = head_book;
    int found = 0;
    while (temp != NULL) {
        if (strstr(temp->title, title) != NULL) {
            printf("\n����: %s", temp->title);
            printf("\n�۰�: %s", temp->author);
            printf("\n���ǻ�: %s", temp->publisher);
            printf("\n������: %s", temp->pub_date);
            printf("\n������: %s", temp->loan_date);
            printf("\n�̿밡�ɻ���: %s", temp->available ? "����" : "�Ұ���");
            printf("\n-----------------------------------------");
            found = 1;
        }
        temp = temp->NEXT;
    }

    if (!found) {
        printf("\n�׷��� ������ ���� å�� ã�� �� �����ϴ� %s\n", title);
    }

    system("pause");
}

void handleLoggedInMenu() {
    int selectedOption = 0;

    while (1) {
        showLoggedInMenu(selectedOption);

        char key = _getch();
        if (key == 72) {
            selectedOption = (selectedOption == 0) ? 8 : (selectedOption - 1);
        }
        else if (key == 80) {
            selectedOption = (selectedOption == 9) ? 0 : (selectedOption + 1);
        }
        else if (key == '\r') {
            switch (selectedOption) {
            case 0:
                addStudent();
                break;
            case 1:
                displayAllStudents();
                break;
            case 2:
                updateStudent();
                break;
            case 3:
                addBook();
                break;
            case 4:
                displayAllBooks();
                break;
            case 5:
                updateBook();
                break;
            case 6:
                handleLoanBook();
                break;
            case 7:
                searchBook();
                break;
            case 8:
                searchStudent();
                break;
            case 9:
                return;
            }
        }
    }
}


int main() {
    loadUsersFromFile();
    int selectedOption = 0;
    int failedLoginAttempts = 0;

    while (1) {
        showMainMenu(selectedOption);

        char key = _getch();
        if (key == 72) {
            selectedOption = (selectedOption == 0) ? 2 : (selectedOption - 1);
        }
        else if (key == 80) {
            selectedOption = (selectedOption == 2) ? 0 : (selectedOption + 1);
        }
        else if (key == '\r') {
            switch (selectedOption) {
            case 0:
                if (handleLogin()) {
                    failedLoginAttempts = 0;
                    handleLoggedInMenu();
                }
                else {
                    failedLoginAttempts++;
                    if (failedLoginAttempts >= 3) {
                        printf("3ȸ �̻� �α����� �����ϼ̽��ϴ�.���������մϴ�...\n");
                        return 1;
                    }
                }
                break;
            case 1:
                handleSignUp();
                break;
            case 2:
                printf("������...\n");
                return 0;
            }
        }
    }

    return 0;
}