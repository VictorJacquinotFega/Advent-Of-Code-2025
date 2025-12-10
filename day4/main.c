#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

int count_neighbors(int row, int col, char **lines, int line_count)
{
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;
            int neighbour_row = row + i;
            int neighbour_column = col + j;
            if (neighbour_row < 0 || neighbour_row >= line_count)
                continue;
            int line_len = strlen(lines[neighbour_row]);
            if (neighbour_column < 0 || neighbour_column >= line_len)
                continue;
            if (lines[neighbour_row][neighbour_column] == '@')
                count++;
        }
    }
    return count;
}

int main(void)
{
    int total_removed = 0;
    int line_count = 0;
    char **lines = read_lines("input.txt", &line_count);
    bool removed_this_round = false;
    if (lines != NULL) {
        while (1) {
            removed_this_round = false;
            char **marked = malloc(sizeof(char *) * line_count);
            for (int i = 0; i < line_count; i++) {
                size_t len = strlen(lines[i]);
                marked[i] = calloc(len + 1, sizeof(char));
            }
            for (int i = 0; i < line_count; i++) {
                for (int j = 0; lines[i][j] != '\0'; j++) {
                    if (lines[i][j] != '@')
                        continue;
                    int neighbors = count_neighbors(i, j, lines, line_count);
                    if (neighbors < 4) {
                        marked[i][j] = 1;
                        removed_this_round = true;
                    }
                }
            }
            if (!removed_this_round) {
                for (int i = 0; i < line_count; i++)
                    free(marked[i]);
                free(marked);
                break;
            }
            for (int i = 0; i < line_count; i++) {
                for (int j = 0; lines[i][j] != '\0'; j++) {
                    if (marked[i][j]) {
                        lines[i][j] = '.';
                        total_removed++;
                    }
                }
                free(marked[i]);
            }
            free(marked);
        }
        for (int i = 0; i < line_count; i++)
            free(lines[i]);
        free(lines);
    }
    printf("Count of the total rolls of paper: %d\n", total_removed);
    return 0;
}
