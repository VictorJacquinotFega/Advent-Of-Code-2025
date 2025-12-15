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

long int* create_ingredients_list(char **lines, int line_count)
{
    int i = 0;
    int index = 0;
    while (i < line_count && lines[i][0] != '\0')
        i++;
    if (i >= line_count - 1)
        return NULL;
    int ingredient_count = line_count - i - 1;
    long int *ingredients = calloc(ingredient_count + 1, sizeof(long int));
    for (int j = 0; j < ingredient_count; j++) {
        ingredients[index] = strtol(lines[i + 1 + j], NULL, 10);
        index++;
    }
    return ingredients;
}

int is_fresh(long int ingredient, char **lines, int line_count)
{
    for (int i = 0; i < line_count && lines[i][0] != '\0'; i++) {
        long int start = 0;
        long int end = 0;
        if (sscanf(lines[i], "%ld-%ld", &start, &end) != 2)
            continue;
        if (ingredient >= start && ingredient <= end)
            return 1;
    }
    return 0;
}

int main(void)
{
    int fresh_ingredients = 0;
    int line_count = 0;
    char **lines = read_lines("input.txt", &line_count);
    long int *available_ingredients = NULL;
    if (lines != NULL) {
        available_ingredients = create_ingredients_list(lines, line_count);
        for (int i = 0; available_ingredients[i] != 0; i++)
            fresh_ingredients += is_fresh(available_ingredients[i], lines, line_count);
    }
    if (available_ingredients != NULL)
        free(available_ingredients);
    for (int i = 0; i < line_count; i++)
        free(lines[i]);
    free(lines);
    printf("Count of the total fresh ingredients: %d\n", fresh_ingredients);
    return 0;
}
