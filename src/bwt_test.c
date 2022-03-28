#include "bwt.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Construct random nul-terminated string of length n (including sentinel)
static void random_string(size_t n, char x[n], const char *alpha, int alpha_size)
{
    for (size_t i = 0; i < n - 1; i++)
    {
        x[i] = alpha[rand() % alpha_size];
    }
    x[n - 1] = '\0';
}

int main(void)
{
    size_t n = 10;
    char x[n], y[n], z[n];

    for (int k = 0; k < 10; k++)
    {
        random_string(n, x, "acgt", 4);
        bwt(n, x, y);  // bwt(x) -> y
        rbwt(n, y, z); // rbwt(bwt(x)) -> z
        assert(strcmp(x, z) == 0);
    }

    return 0;
}
