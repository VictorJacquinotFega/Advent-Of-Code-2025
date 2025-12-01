#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_lines(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return 0;
    int count = 0;
    int chars;
    while ((chars = fgetc(file)) != EOF)
        if (chars == '\n')
            count++;
    fclose(file);
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
    int count = 0;
    int dial = 50;
    int line_count = 0;
    char **lines = read_lines("input.txt", &line_count);
    if (lines != NULL) {
        for (int i = 0; i < line_count; i++) {
            int value = atoi(&lines[i][1]);
            if (lines[i][0] == 'L') {
                for (int j = 0; j < value; j++){
                    dial--;
                    dial = ((dial % 100) + 100) % 100;
                    if (dial == 0)
                        count++;
                }
            }
            else if (lines[i][0] == 'R'){
                for (int j = 0; j < value; j++) {
                    dial++;
                    dial = ((dial % 100) + 100) % 100;
                    if (dial == 0)
                        count++;
                }
            }
            free(lines[i]);
        }
        free(lines);
    }
    printf("Count of times dial reached 0: %d\n", count);
    return 0;
}
