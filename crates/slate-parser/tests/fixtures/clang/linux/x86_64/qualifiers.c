const volatile int value;
static const int cached;
extern int external;
_Atomic int atomic_value;
typedef const int ConstInt;
int *const pointer;
const int *restrict qualified_pointer;
static inline int helper() {
  return 1;
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
// DEFAULT-NEXT:     type @type[[TYPE_ConstInt:[0-9]+]] ConstInt = i32;
// DEFAULT-NEXT:     global %[[VALUE_value:[0-9]+]] value: volatile i32 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cached:[0-9]+]] cached: i32 [storage=static] [const] [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE_external:[0-9]+]] external: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_atomic_value:[0-9]+]] atomic_value: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pointer:[0-9]+]] pointer: ptr<i32> [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_qualified_pointer:[0-9]+]] qualified_pointer: ptr<const i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_helper:[0-9]+]] @helper() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
