#include <stdio.h>
#include <stdlib.h>

// 函数声明
void add_record(const char *filename);
void list_records(const char *filename);

int main(void) {
    int choice;
    const char *filename = "study_log.txt";

    while (1) {
        printf("==== Study Tracker ====\n");
        printf("1. Add study record\n");
        printf("2. Show all records\n");
        printf("3. Exit\n");
        printf("Choose (1-3): ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Please enter a number.\n\n");
            continue;
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}

        if (choice == 1) {
            add_record(filename);
        } else if (choice == 2) {
            list_records(filename);
        } else if (choice == 3) {
            printf("Bye. Come back and study tomorrow.\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n\n");
        }
    }

    return 0;
}

// 先写成占位实现，后面再慢慢改成真正逻辑
void add_record(const char *filename) {
    printf("[TODO] This will add a record to %s.\n\n", filename);
}

void list_records(const char *filename) {
    printf("[TODO] This will list all records from %s.\n\n", filename);
}
