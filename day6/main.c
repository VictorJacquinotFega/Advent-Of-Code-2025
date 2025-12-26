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
    lines[num_lines] = NULL;
    fclose(file);
    *line_count = num_lines;
    return lines;
}

int main(void)
{
    int grand_total = 0;
    int line_count = 0;
    char **lines = read_lines("input.txt", &line_count);
    if (lines != NULL) {
        ;
    }
    for (int i = 0; i < line_count; i++)
        free(lines[i]);
    free(lines);
    printf("Count of the Grand total: %d\n", grand_total);
    return 0;
}
