#include <stdlib.h>

int old_compare(void *context, const void *lhs, const void *rhs) {
  return context != lhs && lhs != rhs;
}

int new_compare(const void *lhs, const void *rhs, void *context) {
  return context != lhs && lhs != rhs;
}

void sort_old(void *base, size_t count, size_t size, void *context) {
  qsort_r(base, count, size, context, old_compare);
}

void sort_new(void *base, size_t count, size_t size, void *context) {
  qsort_r(base, count, size, new_compare, context);
}
