#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

_Static_assert(sizeof(size_t) == 8, "size_t");
_Static_assert(sizeof(ptrdiff_t) == 8, "ptrdiff_t");
_Static_assert(sizeof(intptr_t) == 8, "intptr_t");
_Static_assert(sizeof(uintptr_t) == 8, "uintptr_t");
_Static_assert(sizeof(wchar_t) == 2, "wchar_t");
_Static_assert(sizeof(long) == 4, "long");
_Static_assert(sizeof(long long) == 8, "long long");
_Static_assert(sizeof(long double) == 8, "long double");

extern long long imported_msvc(size_t, ptrdiff_t, intptr_t, uintptr_t, wchar_t,
                               long, long long, long double);

long long call_imported_msvc(size_t size, ptrdiff_t difference,
                             intptr_t  signed_pointer,
                             uintptr_t unsigned_pointer, wchar_t wide,
                             long narrow_long, long long wide_long,
                             long double real) {
  return imported_msvc(size, difference, signed_pointer, unsigned_pointer, wide,
                       narrow_long, wide_long, real);
}

int main(void) {
  char buffer[8];
  strcpy(buffer, "slate");
  printf("%zu %d\n", strlen(buffer), isdigit('7') != 0);
  return 0;
}
// REWRITES-MSVC-DAG: fn imported_msvc(
// REWRITES-MSVC-DAG: _0: usize
// REWRITES-MSVC-DAG: _1: isize
// REWRITES-MSVC-DAG: _2: isize
// REWRITES-MSVC-DAG: _3: usize
// REWRITES-MSVC-DAG: _4: u16
// REWRITES-MSVC-DAG: _5: i32
// REWRITES-MSVC-DAG: _6: i64
// REWRITES-MSVC-DAG: _7: f64
// REWRITES-MSVC-DAG: ) -> i64;
// REWRITES-MSVC-NOT: pthread
// REWRITES-MSVC-NOT: ioctl
// REWRITES-MSVC-NOT: socket
// REWRITES-MSVC-NOT: fork
