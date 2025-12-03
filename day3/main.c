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

int main(void)
{
    int total_joltage = 0;
    int line_count = 0;
    char **lines = read_lines("input.txt", &line_count);
    if (lines != NULL) {
        for (int i = 0; i < line_count; i++) {
            int len = strlen(lines[i]);
            if (len >= 2) {
                int best = 0;
                int right = 0;
                for (int j = len - 1; j >= 0; j--) {
                    int left = lines[i][j] - '0';
                    if (right != 0) {
                        if (left * 10 + right > best)
                            best = left * 10 + right;
                    }
                    if (left > right)
                        right = left;
                }
                total_joltage += best;
            }
            free(lines[i]);
        }
        free(lines);
    }
    printf("Count of the total output joltage: %d\n", total_joltage);
    return 0;
}
