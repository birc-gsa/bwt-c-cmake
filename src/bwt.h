#ifndef BWT_H
#define BWT_H

#include <stdlib.h>

// For both of these functions, n is the length of the string
// including the \0 sentinel. For bwt() that means n = strlen(x) + 1
// but since the sentinel can be anywhere in y, we don't know how n
// relates to strlen(y), which doesn't say anything about how long y
// is.
void bwt(size_t n, const char x[n], char y[n]);
void rbwt(size_t n, const char y[n], char x[n]);

#endif // BWT_H
