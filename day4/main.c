#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_lines(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return 0;
    int count = 0;
    int in_line = 0;
    int chars;
    while ((chars = fgetc(file)) != EOF) {
        if (chars == '\n') {
            count++;
            in_line = 0;
        } else
            in_line = 1;
    }
    fclose(file);
    if (in_line)
        count++;
    return count;
}

char **read_lines(const char *filename, int *line_count)
{
    int num_lines = count_lines(filename);
    if (num_lines == 0) {
        *line_count = 0;
        return NULL;
    }
    FILE *file = fopen(filename, "r");  
    char **lines = malloc(sizeof(char *) * (num_lines + 1));
    char buffer[1024];
    for (int i = 0; i < num_lines; i++) {
        fgets(buffer, sizeof(buffer), file);
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
        lines[i] = malloc(strlen(buffer) + 1);
        strcpy(lines[i], buffer);
    }    
    fclose(file);
    *line_count = num_lines;
    return lines;
}

int is_accessible(int row, int col, char **lines, int line_count)
{
    int count = 0;
    int neighbor_row = 0;
    int neighbor_column = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;
            neighbor_row = row + i;
            neighbor_column = col + j;
            if (neighbor_row < 0 || neighbor_row >= line_count)
                continue;
            int line_len = strlen(lines[neighbor_row]);
            if (neighbor_column < 0 || neighbor_column >= line_len)
                continue;
            if (lines[neighbor_row][neighbor_column] == '@')
                count++;
        }
    }
    return count < 4;
}

int main(void)
{
    int total_rollsofpaper = 0;
    int line_count = 0;
    char **lines = read_lines("input.txt", &line_count);
    if (lines != NULL) {
        for (int i = 0; i < line_count; i++) {
            for (int j = 0; lines[i][j] != '\0'; j++)
                if (lines[i][j] == '@')
                    total_rollsofpaper += is_accessible(i, j, lines, line_count);
        }
        for (int i = 0; i < line_count; i++)
            free(lines[i]);
        free(lines);
    }
    printf("Count of the total rolls of paper: %d\n", total_rollsofpaper);
    return 0;
}
