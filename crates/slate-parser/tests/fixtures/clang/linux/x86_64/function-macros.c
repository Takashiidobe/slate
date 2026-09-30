#define VALUE 7
#define ADD(left, right) left + right
#define STRINGIFY(value) #value
#define JOIN(left, right) left ## right
#define REST(first, ...) __VA_ARGS__
#define WRAP(value) value

int added __attribute__((slate_macro(ADD(1, 2))));
int stringified __attribute__((slate_macro(STRINGIFY(hello world))));
int joined __attribute__((slate_macro(JOIN(foo, bar))));
int variadic __attribute__((slate_macro(REST(first, 1, 2))));
int prescanned() {
  return WRAP(VALUE);
}

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_added:[0-9]+]] added: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_stringified:[0-9]+]] stringified: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_joined:[0-9]+]] joined: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_variadic:[0-9]+]] variadic: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_prescanned:[0-9]+]] @prescanned() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
