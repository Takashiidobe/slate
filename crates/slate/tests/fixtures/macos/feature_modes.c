#include <features.h>
#include <langinfo.h>
#include <stdio.h>

#if defined(EXPECT_DARWIN_FULL) ||                                             \
    (!defined(EXPECT_POSIX_2001) && !defined(EXPECT_POSIX_2008))
_Static_assert(__DARWIN_C_LEVEL == __DARWIN_C_FULL, "Darwin full level");
#if !defined(D_MD_ORDER) || !defined(YESSTR)
#error "Darwin extensions missing"
#endif
#elif defined(EXPECT_POSIX_2001)
_Static_assert(__DARWIN_C_LEVEL == 200112L, "POSIX.1-2001 level");
#if defined(D_MD_ORDER) || defined(YESSTR)
#error "Darwin extensions leaked into POSIX.1-2001"
#endif
#elif defined(EXPECT_POSIX_2008)
_Static_assert(__DARWIN_C_LEVEL == 200809L, "POSIX.1-2008 level");
#if defined(D_MD_ORDER) || defined(YESSTR)
#error "Darwin extensions leaked into POSIX.1-2008"
#endif
#endif

long feature_level(void) { return __DARWIN_C_LEVEL; }

// SLATE-FILECHECK-BEGIN lowering-macos
// LOWERING-MACOS: #![allow(
// LOWERING-MACOS-NEXT:     dead_code,
// LOWERING-MACOS-NEXT:     unused,
// LOWERING-MACOS-NEXT:     non_camel_case_types,
// LOWERING-MACOS-NEXT:     non_snake_case,
// LOWERING-MACOS-NEXT:     non_upper_case_globals,
// LOWERING-MACOS-NEXT:     arithmetic_overflow,
// LOWERING-MACOS-NEXT:     unconditional_panic,
// LOWERING-MACOS-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-MACOS-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-MACOS-NEXT:     unused_comparisons
// LOWERING-MACOS-NEXT: )]
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: compile_error!("\"Darwin extensions missing\"");
// LOWERING-MACOS-EMPTY:
// LOWERING-MACOS-NEXT: fn feature_level() -> i64 {
// LOWERING-MACOS-NEXT:     let {{__v[0-9]+}}: i64 = 900000;
// LOWERING-MACOS-NEXT:     return {{__v[0-9]+}};
// LOWERING-MACOS-NEXT: }
// SLATE-FILECHECK-END lowering-macos

// SLATE-FILECHECK-BEGIN rewrites-macos
// REWRITES-MACOS: #![allow(
// REWRITES-MACOS-NEXT:     dead_code,
// REWRITES-MACOS-NEXT:     unused,
// REWRITES-MACOS-NEXT:     non_camel_case_types,
// REWRITES-MACOS-NEXT:     non_snake_case,
// REWRITES-MACOS-NEXT:     non_upper_case_globals,
// REWRITES-MACOS-NEXT:     arithmetic_overflow,
// REWRITES-MACOS-NEXT:     unconditional_panic,
// REWRITES-MACOS-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-MACOS-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-MACOS-NEXT:     unused_comparisons
// REWRITES-MACOS-NEXT: )]
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: compile_error!("\"Darwin extensions missing\"");
// REWRITES-MACOS-EMPTY:
// REWRITES-MACOS-NEXT: fn feature_level() -> i64 {
// REWRITES-MACOS-NEXT:     let {{__v[0-9]+}}: i64 = 900000;
// REWRITES-MACOS-NEXT:     {{__v[0-9]+}}
// REWRITES-MACOS-NEXT: }
// SLATE-FILECHECK-END rewrites-macos
