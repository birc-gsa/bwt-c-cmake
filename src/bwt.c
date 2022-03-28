#include "bwt.h"
#include "sa.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void bwt(size_t n, const char x[n], char y[n]) // FlawFinder: ignore (x[n] is fine)
{
    uint32_t *sa = malloc(n * sizeof *sa);
    prefix_doubling(n, x, sa);

    for (size_t i = 0; i < n; i++)
    {
        // Previous index, with wrap at index zero...
        int j = sa[i] + (sa[i] == 0) * n;
        y[i] = x[j - 1];
    }

    free(sa);
}

struct alphabet
{
    int sigma;
    unsigned char map[256];    // FlawFinder: ignore
    unsigned char revmap[256]; // FlawFinder: ignore
};

static void extract_alphabet(size_t n, const char x[n], struct alphabet *alpha) // FlawFinder: ignore (x[n] is fine)
{
    memset(alpha->map, 0, 256);
    memset(alpha->revmap, 0, 256);
    for (size_t i = 0; i < n; i++)
    {
        alpha->map[(unsigned char)x[i]] = 1;
    }
    alpha->sigma = 0;
    for (int i = 0; i < 256; i++)
    {
        if (alpha->map[i])
        {
            alpha->map[i] = alpha->sigma++;
            alpha->revmap[alpha->map[i]] = i;
        }
    }
}

static inline void map_alphabet(size_t n, const char x[n], char y[n], struct alphabet *alpha) // FlawFinder: ignore (x[n] is fine)
{
    for (size_t i = 0; i < n; i++)
    {
        y[i] = alpha->map[(unsigned char)x[i]];
    }
}

static inline void revmap_alphabet(size_t n, const char y[n], char x[n], struct alphabet *alpha) // FlawFinder: ignore (x[n] is fine)
{
    for (size_t i = 0; i < n; i++)
    {
        x[i] = alpha->revmap[(unsigned char)y[i]];
    }
}

struct c_table
{
    int sigma;
    int cumsum[];
};
#define C(A) (ctab->cumsum[(unsigned char)A])

struct o_table
{
    int sigma;
    size_t n;
    int table[];
};
// Direct look-up, not compensating for row zero
#define O_RAW(A, I) (otab->table[(I) * (otab)->sigma + (unsigned char)(A)])
// Correcting for implict zero row
#define O(A, I) (((I) == 0) ? 0 : O_RAW((A), (I)-1))

// Get the size of one element of a variable array member
#define ELM_SIZE(TYPE, ARR) (sizeof *(((TYPE *)0)->ARR))
// Allocate a variable array member struct with N elements
#define VAR_ARRAY_ALLOC(TYPE, ARR, N) malloc(offsetof(TYPE, ARR) + (N)*ELM_SIZE(TYPE, ARR))

struct c_table *build_c_table(size_t n, const char x[n], int sigma) // FlawFinder: ignore (x[n] is fine)
{
    struct c_table *ctab = VAR_ARRAY_ALLOC(struct c_table, cumsum, sigma);
    assert(ctab);

    ctab->sigma = sigma;
    for (int i = 0; i < sigma; i++)
    {
        ctab->cumsum[i] = 0;
    }
    for (size_t i = 0; i < n; i++)
    {
        ctab->cumsum[(unsigned char)x[i]]++;
    }
    for (int i = 0, acc = 0; i < sigma; i++)
    {
        int k = ctab->cumsum[i];
        ctab->cumsum[i] = acc;
        acc += k;
    }

    return ctab;
}

struct o_table *build_o_table(size_t n, const char bwt[n], struct c_table const *ctab) // FlawFinder: ignore (bwt[n] is fine)
{
    struct o_table *otab = VAR_ARRAY_ALLOC(struct o_table, table, ctab->sigma * n);
    assert(otab);

    otab->sigma = ctab->sigma;
    otab->n = n;
    for (int a = 0; a < ctab->sigma; a++)
    {
        O_RAW(a, 0) = (a == bwt[0]);
        for (size_t i = 1; i < n; i++)
        {
            O_RAW(a, i) = O_RAW(a, i - 1) + (a == bwt[i]);
        }
    }

    return otab;
}

void rbwt(size_t n, const char y[n], char x[n]) // FlawFinder: ignore (x[n] is fine)
{
    char *z = malloc(n);
    assert(z);

    struct alphabet alpha;
    extract_alphabet(n, y, &alpha);
    map_alphabet(n, y, z, &alpha);

    struct c_table *ctab = build_c_table(n, z, alpha.sigma);
    struct o_table *otab = build_o_table(n, z, ctab);

    x[n - 1] = 0; // Sentinel at the end of x.
    // The sentinel is also at the beginning of row 0 in the BWT
    // matrix, so i starts at zero.
    for (int i = 0, j = n - 2; j >= 0; j--)
    {
        x[j] = z[i];
        i = C(z[i]) + O(z[i], i);
    }

    revmap_alphabet(n, x, x, &alpha);

    free(otab);
    free(ctab);
    free(z);
}
