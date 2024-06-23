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
    printf("%s1. 학생 추가\n", (selectedOption == 0 ? "> " : "  "));
    printf("%s2. 저장된 모든 학생 보기\n", (selectedOption == 1 ? "> " : "  "));
    printf("%s3. 학생 수정\n", (selectedOption == 2 ? "> " : "  "));
    printf("%s4. 책 추가\n", (selectedOption == 3 ? "> " : "  "));
    printf("%s5. 저장된 모든 책 보기\n", (selectedOption == 4 ? "> " : "  "));
    printf("%s6. 책 수정\n", (selectedOption == 5 ? "> " : "  "));
    printf("%s7. 책 빌리기\n", (selectedOption == 6 ? "> " : "  "));
    printf("%s8. 책 찾기\n", (selectedOption == 7 ? "> " : "  "));
    printf("%s9. 학생 찾기\n", (selectedOption == 8 ? "> " : "  "));
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
    printf("새로운 ID을 입력하세요: ");
    scanf("%s", username);
    printf("새로운 password를 입력하세요: ");
    scanf("%s", password);

    if (addUser(username, password)) {
        printf("회원가입에 성공하였습니다!\n");
    }
    else {
        printf("이미 존재하는 ID입니다!\n");
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
    printf("ID를 입력하십시오: ");
    scanf("%s", username);
    printf("password를 입력하십시오: ");
    scanf("%s", password);

    if (login(username, password)) {
        printf("로그인에 성공하셨습니다!\n");

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
    printf("|        학생 추가하기       |\n");
    printf("==============================\n");
    getchar();
    struct student* temp = (struct student*)malloc(sizeof(struct student));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("\n학생의 이름을 적으세요: ");
    scanf("%[^\n]s", temp->name);
    getchar();

    printf("\n생년월일을 적으세요 (YYYY/MM/DD): ");
    scanf("%s", temp->dob);
    getchar();

    printf("\n학번에 대해 적으세요: ");
    scanf("%s", temp->contactnumber);
    getchar();

    printf("\n성별을 적으세요: ");
    scanf("%[^\n]s", temp->fname);
    getchar();

    printf("\n전화번호을 적으세요: ");
    scanf("%[^\n]s", temp->number);
    getchar();

    printf("\n주소를 적으세요: ");
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
        printf("\n어떠한 기록도 없습니다\n");
    }
    else {
        while (temp != NULL) {
            printf("\n이름: %s", temp->name);
            printf("\n생년월일: %s", temp->dob);
            printf("\n학번: %s", temp->contactnumber);
            printf("\n성별: %s", temp->fname);
            printf("\n전화번호: %s", temp->number);
            printf("\n주소: %s", temp->address);
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
    printf("|         학생 수정하기      |\n");
    printf("==============================\n");
    getchar();
    printf("수정할 학생의 이름을 적으세요: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; 

    struct student* temp = head_student;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("\nUpdating details for %s\n", temp->name);

            printf("생년월일 (YYYY/MM/DD) (현재: %s): ", temp->dob);
            scanf("%s", temp->dob);
            getchar();

            printf("학번 (현재: %s): ", temp->contactnumber);
            scanf("%s", temp->contactnumber);
            getchar();

            printf("성별 (현재: %s): ", temp->fname);
            scanf("%[^\n]s", temp->fname);
            getchar();

            printf("전화번호 (현재: %s): ", temp->number);
            scanf("%[^\n]s", temp->number);
            getchar();


            printf("주소 (현재: %s): ", temp->address);
            scanf("%[^\n]s", temp->address);
            getchar();

            printf("학생의 정보를 성공적으로 수정하였습니다!\n");
            saveStudentsToFile();
            system("pause");
            return;
        }
        temp = temp->NEXT;
    }
    printf("그러한 이름을 가진 학생을 찾을 수 없습니다! %s\n", name);
    system("pause");
}

void searchStudent() {
    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|           학생 찾기        |\n");
    printf("==============================\n");
    char name[30];
    printf("찾고싶은 학생의 이름을 입력하세요: ");
    scanf("%s", name);

    struct student* temp = head_student;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("이름: % s\n", temp->name);
            printf("생년월일: %s\n", temp->dob);
            printf("학번: %s\n", temp->contactnumber);
            printf("성별: %s\n", temp->fname);
            printf("전화번호: %s\n", temp->number);
            printf("주소: %s\n", temp->address);
            printf("-----------------------------\n");
            system("pause");
            return;
        }
        temp = temp->NEXT;
    }
    printf("학생을 찾을 수 없습니다!\n");
    system("pause");
}

void addBook() {

    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|          책 추가하기       |\n");
    printf("==============================\n");

    struct book* temp = (struct book*)malloc(sizeof(struct book));
    if (temp == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("제목을 입력하세요: ");
    scanf("%s", temp->title);
    printf("작가를 입력하세요: ");
    scanf("%s", temp->author);
    printf("출판사를 입력하세요: ");
    scanf("%s", temp->publisher);
    printf("출판일을 입력하세요: (YYYY/MM/DD): ");
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
    printf("성공적으로 책을 추가하였습니다!\n");
    system("pause");
}

void displayAllBooks() {

    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|          모든 책 보기      |\n");
    printf("==============================\n");

    struct book* temp = head_book;

    if (head_book == NULL) {
        printf("\n기록이 없습니다!!\n");
    }
    else {
        while (temp != NULL) {
            printf("제목: %s\n", temp->title);
            printf("작가: %s\n", temp->author);
            printf("출판사: %s\n", temp->publisher);
            printf("출판일: %s\n", temp->pub_date);
            printf("대출일: %s\n", temp->loan_date);
            printf("이용상태: %s\n", temp->available ? "가능" : "불가능");
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
    printf("|          책 업데이트       |\n");
    printf("==============================\n");
    getchar();
    printf("Enter the title of the book to update: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    struct book* temp = head_book;
    while (temp != NULL) {
        if (strcmp(temp->title, title) == 0) {
            printf("\nUpdating details for %s\n", temp->title);

            printf("작가: ");
            scanf("%s", temp->author);
            printf("출판사: ");
            scanf("%s", temp->publisher);
            printf("출판일 (YYYY/MM/DD): ");
            scanf("%s", temp->pub_date);

            printf("책의 정보를 성공적으로 수정하였습니다!\n");
            saveBooksToFile();
            system("pause");
            return;
        }
        temp = temp->NEXT;
    }
    printf("그러한 제목을 가진 책을 찾을 수 없습니다! %s\n", title);
    system("pause");
}

void handleLoanBook() {
    clearScreen();
    setCursorPosition(0, 0);
    printf("==============================\n");
    printf("|             대출            |\n");
    printf("==============================\n");

    char title[50];
    printf("대출 또는 반납할 책의 이름을 적어주세요:");
    scanf("%s", title);

    struct book* temp = head_book;
    while (temp != NULL) {
        if (strcmp(temp->title, title) == 0) {
            if (temp->available) {
                printf("대출일 (YYYY/MM/DD): ");
                scanf("%s", temp->loan_date);
                temp->available = 0;
                printf("책을 성공적으로 대출하였습니다!\n");
            }
            else {
                strcpy(temp->loan_date, "");
                temp->available = 1;
                printf("성공적으로 반납하였습니다!\n");
            }

            saveBooksToFile();

            system("pause");
            return;
        }
        temp = temp->NEXT;
    }

    printf("책을 찾을 수 없습니다!\n");
    system("pause");
}

void searchBook() {
    char title[50];
    clearScreen();
    printf("==============================\n");
    printf("|          책 검색하기       |\n");
    printf("==============================\n");
    getchar();
    printf("Enter the title of the book to search: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;

    struct book* temp = head_book;
    int found = 0;
    while (temp != NULL) {
        if (strstr(temp->title, title) != NULL) {
            printf("\n제목: %s", temp->title);
            printf("\n작가: %s", temp->author);
            printf("\n출판사: %s", temp->publisher);
            printf("\n출판일: %s", temp->pub_date);
            printf("\n대출일: %s", temp->loan_date);
            printf("\n이용가능상태: %s", temp->available ? "가능" : "불가능");
            printf("\n-----------------------------------------");
            found = 1;
        }
        temp = temp->NEXT;
    }

    if (!found) {
        printf("\n그러한 제목을 가진 책을 찾을 수 없습니다 %s\n", title);
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
                        printf("3회 이상 로그인을 실패하셨습니다.강제종료합니다...\n");
                        return 1;
                    }
                }
                break;
            case 1:
                handleSignUp();
                break;
            case 2:
                printf("종료중...\n");
                return 0;
            }
        }
    }

    return 0;
}