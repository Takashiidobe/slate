#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                              \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

typedef int max_align_t;

_Static_assert(sizeof(size_t) == 8, "size_t");
_Static_assert(sizeof(ptrdiff_t) == 8, "ptrdiff_t");
_Static_assert(sizeof(intptr_t) == 8, "intptr_t");
_Static_assert(sizeof(uintptr_t) == 8, "uintptr_t");
_Static_assert(sizeof(wchar_t) == 2, "wchar_t");
_Static_assert(sizeof(long) == 4, "long");
_Static_assert(sizeof(long long) == 8, "long long");
_Static_assert(sizeof(long double) == 8, "long double");
_Static_assert(LONG_MAX == 2147483647L, "LONG_MAX");
_Static_assert(WCHAR_MAX == 65535, "WCHAR_MAX");
_Static_assert(LDBL_MANT_DIG == 53, "LDBL_MANT_DIG");
_Static_assert(MB_LEN_MAX == 5, "MB_LEN_MAX");
_Static_assert(_I8_MIN == -128, "_I8_MIN");
_Static_assert(_UI16_MAX == 0xffffU, "_UI16_MAX");
_Static_assert(_I32_MAX == 2147483647, "_I32_MAX");
_Static_assert(_UI64_MAX == 0xffffffffffffffffULL, "_UI64_MAX");
_Static_assert(SIZE_MAX == 0xffffffffffffffffULL, "SIZE_MAX");
_Static_assert(RSIZE_MAX == 0x7fffffffffffffffULL, "RSIZE_MAX");

#ifdef BOOL_WIDTH
#error "MSVC limits.h must not expose C23 width macros"
#endif
#ifdef _POSIX_ARG_MAX
#error "MSVC limits.h must not expose POSIX constants"
#endif
#ifdef __STDC_VERSION_ASSERT_H__
#error "MSVC assert.h must not expose the C23 version macro"
#endif
#ifdef static_assert
#error "MSVC assert.h must not expose static_assert"
#endif
#ifdef noreturn
#error "MSVC headers must not expose noreturn"
#endif

TYPE_IS(&_set_errno, errno_t (*)(int));
TYPE_IS(&_get_errno, errno_t (*)(int *));
TYPE_IS(&__threadid, unsigned long (*)(void));
TYPE_IS(&__threadhandle, uintptr_t (*)(void));

int main(void) { return 0; }

// SLATE-FILECHECK-BEGIN lowering-msvc
// LOWERING-MSVC: #![allow(
// LOWERING-MSVC-NEXT:     dead_code,
// LOWERING-MSVC-NEXT:     unused,
// LOWERING-MSVC-NEXT:     non_camel_case_types,
// LOWERING-MSVC-NEXT:     non_snake_case,
// LOWERING-MSVC-NEXT:     non_upper_case_globals,
// LOWERING-MSVC-NEXT:     arithmetic_overflow,
// LOWERING-MSVC-NEXT:     unconditional_panic,
// LOWERING-MSVC-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-MSVC-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-MSVC-NEXT:     unused_comparisons
// LOWERING-MSVC-NEXT: )]
// LOWERING-MSVC-EMPTY:
// LOWERING-MSVC-NEXT: fn main() {
// LOWERING-MSVC-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MSVC-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-MSVC-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-MSVC-NEXT: }
// SLATE-FILECHECK-END lowering-msvc

// SLATE-FILECHECK-BEGIN rewrites-msvc
// REWRITES-MSVC: #![allow(
// REWRITES-MSVC-NEXT:     dead_code,
// REWRITES-MSVC-NEXT:     unused,
// REWRITES-MSVC-NEXT:     non_camel_case_types,
// REWRITES-MSVC-NEXT:     non_snake_case,
// REWRITES-MSVC-NEXT:     non_upper_case_globals,
// REWRITES-MSVC-NEXT:     arithmetic_overflow,
// REWRITES-MSVC-NEXT:     unconditional_panic,
// REWRITES-MSVC-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-MSVC-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-MSVC-NEXT:     unused_comparisons
// REWRITES-MSVC-NEXT: )]
// REWRITES-MSVC-EMPTY:
// REWRITES-MSVC-NEXT: fn main() {
// REWRITES-MSVC-NEXT:     std::process::exit(0 as i32);
// REWRITES-MSVC-NEXT: }
// SLATE-FILECHECK-END rewrites-msvc
