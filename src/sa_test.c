#include "sa.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

static void random_string(size_t n, char x[n], const char *alpha, int alpha_size) // FlawFinder: ignore
{
    for (size_t i = 0; i < n - 1; i++)
    {
        x[i] = alpha[rand() % alpha_size];
    }
    x[n - 1] = '\0';
}

static void check_suffixes(size_t n, const char x[n], uint32_t sa[n]) // FlawFinder: ignore
{
    for (size_t i = 1; i < n; i++)
    {
        assert(strcmp(x + sa[i - 1], x + sa[i]) < 0);
    }
}

int main(void)
{
    size_t n = 10;
    char x[n]; // FlawFinder: ignore
    uint32_t sa[n];
    for (int k = 0; k < 5; k++)
    {
        random_string(n, x, "acgt", 4);
        prefix_doubling(n, x, sa);
        check_suffixes(n, x, sa);
    }

    return 0;
}
