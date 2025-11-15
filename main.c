#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATE_LEN    20
#define MAX_SUBJECT_LEN 256
#define MAX_LINE_LEN    512

void add_record(const char *filename);
void list_records(const char *filename);
int  calc_total_minutes(const char *filename);

void strip_newline(char *s);

// 主程序：菜单循环
int main(void) {
    int choice;
    const char *filename = "study_log.txt";

    while (1) {
        printf("==== Study Tracker ====\n");
        printf("1. Add study record\n");
        printf("2. Show all records\n");
        printf("3. Show total study minutes\n");
        printf("4. Exit\n");
        printf("Choose (1-4): ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Please enter a number.\n\n");
            continue;
        }

        // 清理掉缓冲区里的回车，方便后面用 fgets
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}

        if (choice == 1) {
            add_record(filename);
        } else if (choice == 2) {
            list_records(filename);
        } else if (choice == 3) {
            int total = calc_total_minutes(filename);
            printf("Total study time: %d minutes.\n\n", total);
        } else if (choice == 4) {
            printf("Bye. Come back and study tomorrow.\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n\n");
        }
    }

    return 0;
}

// 去掉字符串末尾的 '\n'
void strip_newline(char *s) {
    if (s == NULL) return;
    size_t len = strlen(s);
    if (len == 0) return;
    if (s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

// 添加一条学习记录
void add_record(const char *filename) {
    char date[MAX_DATE_LEN];
    char subject[MAX_SUBJECT_LEN];
    int minutes;

    printf("\n--- Add study record ---\n");

    printf("Enter date (e.g. 2025-11-15): ");
    if (!fgets(date, sizeof(date), stdin)) {
        printf("Failed to read date.\n\n");
        return;
    }
    strip_newline(date);

    if (date[0] == '\0') {
        printf("Date cannot be empty.\n\n");
        return;
    }

    printf("Enter study minutes (integer): ");
    if (scanf("%d", &minutes) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Invalid minutes.\n\n");
        return;
    }

    // 清理掉 scanf 留下的回车
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    if (minutes <= 0) {
        printf("Minutes should be positive.\n\n");
        return;
    }

    printf("Enter subject / notes (can contain spaces): ");
    if (!fgets(subject, sizeof(subject), stdin)) {
        printf("Failed to read subject.\n\n");
        return;
    }
    strip_newline(subject);

    if (subject[0] == '\0') {
        printf("Subject cannot be empty.\n\n");
        return;
    }

    FILE *fp = fopen(filename, "a");
    if (!fp) {
        printf("Cannot open file %s for writing.\n\n", filename);
        return;
    }

    // 文件格式：date minutes subject(整行)
    // 例：2025-11-15 60 复习 数据结构 树
    fprintf(fp, "%s %d %s\n", date, minutes, subject);
    fclose(fp);

    printf("Record saved.\n\n");
}

// 显示所有记录
void list_records(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("\nNo records yet. File %s not found.\n\n", filename);
        return;
    }

    char line[MAX_LINE_LEN];
    char date[MAX_DATE_LEN];
    char subject[MAX_SUBJECT_LEN];
    int minutes;
    int count = 0;

    printf("\n--- All study records ---\n");

    while (fgets(line, sizeof(line), fp)) {
        // 解析：date minutes subject(剩余整行)
        if (sscanf(line, "%19s %d %255[^\n]", date, &minutes, subject) == 3) {
            printf("Date: %s | Minutes: %d | Subject: %s\n",
                   date, minutes, subject);
            count++;
        }
    }

    if (count == 0) {
        printf("No valid records found.\n");
    }

    printf("-------------------------\n\n");

    fclose(fp);
}

// 统计总学习时长（分钟）
int calc_total_minutes(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("\nNo records yet. File %s not found.\n\n", filename);
        return 0;
    }

    char line[MAX_LINE_LEN];
    char date[MAX_DATE_LEN];
    char subject[MAX_SUBJECT_LEN];
    int minutes;
    int total = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%19s %d %255[^\n]", date, &minutes, subject) == 3) {
            if (minutes > 0)
                total += minutes;
        }
    }

    fclose(fp);
    return total;
}
