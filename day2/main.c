#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = malloc((size_t)filesize + 1);
    if (content == NULL) {
        fclose(file);
        return NULL;
    }
    fread(content, 1, (size_t)filesize, file);
    content[filesize] = '\0';
    fclose(file);
    return content;
}

static int is_invalid_id(long long value)
{
    char buffer[32];
    int len = snprintf(buffer, sizeof(buffer), "%lld", value);
    if (len <= 0)
        return 0;
    for (int pattern_len = 1; pattern_len <= len / 2; pattern_len++) {
        if (len % pattern_len != 0)
            continue;
        int repeats = len / pattern_len;
        if (repeats < 2)
            continue;
        int matched = 1;
        for (int i = pattern_len; i < len; i++) {
            if (buffer[i] != buffer[i % pattern_len]) {
                matched = 0;
                break;
            }
        }
        if (matched)
            return 1;
    }
    return 0;
}

static long long sum_invalid_in_range(long long start, long long end)
{
    long long sum = 0;
    for (long long id = start; id <= end; id++)
        if (is_invalid_id(id))
            sum += id;
    return sum;
}

int main(void)
{
    long long total = 0;
    char *content = read_file("input.txt");
    if (content != NULL) {
        char *token = strtok(content, ",\n\r");
        while (token != NULL) {
            char *dash = strchr(token, '-');
            if (dash != NULL) {
                *dash = '\0';
                long long start = atoll(token);
                long long end = atoll(dash + 1);
                total += sum_invalid_in_range(start, end);
            }
            token = strtok(NULL, ",\n\r");
        }
        free(content);
        printf("Sum of invalid IDs: %lld\n", total);
    }
    return 0;
}
