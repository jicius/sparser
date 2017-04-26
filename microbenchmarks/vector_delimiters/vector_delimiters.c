#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#include <immintrin.h>

#include "common.h"

const char *test_string = "United,Airlines,An airline based in San Francisco CA,Flig1423123";
const int iterations = 1000000;

double measure_strsep() {
    
    int count = 0;
    double time = 0.0;

    char *token;

    for (int i = 0; i < iterations; i++) {
        char *data = (char *)malloc(sizeof(char) * 4096);
        strncpy((char *)data, test_string, strlen(test_string));
        time_start();
        while((token = strsep(&data, ",")) != NULL) {
            count++;
        }
        time += time_stop();
        free(data);
    }

    printf("%.3f seconds\n", time);
    printf("Tokens found: %d\n", count);
    return time;
}

double measure_vector() {
    int count = 0;

    unsigned length = strlen(test_string) * sizeof(char);
    __m256i delimiter = _mm256_set1_epi8(',');

    time_start();
    for (int i = 0; i < iterations; i++) {
        for (int i = 0; i + 32 <= length; i += 32) {
            __m256i word = _mm256_load_si256((__m256i const *)(test_string + i));
            __m256i mask = _mm256_cmpeq_epi8(word, delimiter);
            int imask = _mm256_movemask_epi8(mask);
            while (imask) {
                int idx = ffs(imask) - 1;
                count++;
                imask &= ~(1 << idx);
            }
        }
        count++;
        // for the token after the last delimiter.
    }
    double time = time_stop();

    printf("%.3f seconds\n", time);
    printf("Tokens found: %d\n", count);

    return time;
}

int main() {
    // Compare exact match without parsing.
    double c = measure_strsep();
    double d = measure_vector();
    printf("Speedup: %f\n", c / d);
 }