#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "svec.h"

/* I originally wrote code which passed four out of five tests.
 * After setting it down and not looking at it for a while,
 * rather than spending my time debugging it, I uploaded
 * it to DeepSeek with yellowstone.txt and prompted it to
 * correct the code. The following is what it returned
 * and passed the final test with meditations.txt*/

void sort_svec_alphabetically(svec* sv)
{
    // Simple bubble sort for alphabetical order
    for (int i = 0; i < sv->size - 1; i++) {
        for (int j = 0; j < sv->size - i - 1; j++) {
            if (strcmp(sv->data[j], sv->data[j + 1]) > 0) {
                // Swap both data and cdata
                char* temp_data = sv->data[j];
                sv->data[j] = sv->data[j + 1];
                sv->data[j + 1] = temp_data;
                
                int temp_cdata = sv->cdata[j];
                sv->cdata[j] = sv->cdata[j + 1];
                sv->cdata[j + 1] = temp_cdata;
            }
        }
    }
}

void process_word(svec* sv, const char* word_start, int word_len)
{
    if (word_len == 0) return;
    
    char* word = malloc(word_len + 1);
    for (int i = 0; i < word_len; i++) {
        word[i] = tolower(word_start[i]);
    }
    word[word_len] = '\0';
    
    // Check if word already exists
    int found = 0;
    for (int i = 0; i < sv->size; i++) {
        if (strcmp(sv->data[i], word) == 0) {
            sv->cdata[i]++;
            found = 1;
            break;
        }
    }
    
    if (!found) {
        // Add new word
        if (sv->size >= sv->cap - 1) {
            sv->cap *= 2;
            sv->data = realloc(sv->data, sv->cap * sizeof(char*));
            sv->cdata = realloc(sv->cdata, sv->cap * sizeof(int));
        }
        sv->data[sv->size] = word;
        sv->cdata[sv->size] = 1;
        sv->size++;
    } else {
        free(word);
    }
}

void process_line(svec* sv, const char* line)
{
    int word_start = -1;
    
    for (int i = 0; line[i] != '\0'; i++) {
        if (isalpha(line[i])) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                process_word(sv, line + word_start, i - word_start);
                word_start = -1;
            }
        }
    }
    
    // Process last word if any
    if (word_start != -1) {
        process_word(sv, line + word_start, strlen(line) - word_start);
    }
}

void process_file_stream(svec* sv, FILE* fh)
{
    char line[256];
    while (fgets(line, sizeof(line), fh)) {
        // Remove newline if present
        line[strcspn(line, "\n")] = '\0';
        process_line(sv, line);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage:\n  %s input-file\n", argv[0]);
        return 1;
    }

    svec* xs = make_svec();
    
    FILE* fh = fopen(argv[1], "r");
    if (fh) {
        process_file_stream(xs, fh);
        fclose(fh);
    } else {
        printf("Error: Could not open file %s\n", argv[1]);
        free_svec(xs);
        return 1;
    }

    sort_svec_alphabetically(xs);
    
    // Print words with count > 1, sorted alphabetically
    for (int i = 0; i < xs->size; i++) {
        if (xs->cdata[i] > 1) {
            printf("%d\t%s\n", xs->cdata[i], xs->data[i]);
        }
    }
    
    free_svec(xs);
    return 0;
}
