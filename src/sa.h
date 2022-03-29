#ifndef SA_H
#define SA_H

#include <stdint.h>
#include <stdlib.h>

// If you care to read this, you will learn of a new suffix array
// construction algorithm called prefix doubling. It only runs in
// O(n log n), but it practise it is very fast, due to its low
// overhead.
// We don't cover this algorithm in class, but if you read the code,
// you should be able to follow it, and then you have learned a
// completely new way of attacking suffix array construction.
// Doesn't that sound great? Yeah!
void prefix_doubling(size_t n, const char x[n], uint32_t sa[n]);

#endif // SA_H
