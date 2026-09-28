// SLATE-FILECHECK-DEFINES COMPATIBLE COMPATIBLE
// SLATE-FILECHECK-STD COMPATIBLE c17
// SLATE-FILECHECK-DEFINES SAME_TAGS_C23 SAME_TAGS_C23
// SLATE-FILECHECK-STD SAME_TAGS_C23 c23
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#if defined(COMPATIBLE)
int f();
int f(int);
int g(int);
int g(const int);
#else
struct S { int a; };
struct S { int a; };
enum E { A };
enum E { A };
struct S s;
enum E e;
#endif

// SLATE-FILECHECK-BEGIN COMPATIBLE
// COMPATIBLE: module {
// COMPATIBLE-NEXT:     target "x86_64-unknown-linux-gnu" {
// COMPATIBLE-NEXT:         endian = little;
// COMPATIBLE-NEXT:         pointer [size=8, align=8];
// COMPATIBLE-NEXT:         stack_alignment = 16;
// COMPATIBLE-NEXT:         long_double = f80;
// COMPATIBLE-NEXT:         storage bool [size=1, align=1];
// COMPATIBLE-NEXT:         storage i8, u8 [size=1, align=1];
// COMPATIBLE-NEXT:         storage i16, u16 [size=2, align=2];
// COMPATIBLE-NEXT:         storage i32, u32 [size=4, align=4];
// COMPATIBLE-NEXT:         storage i64, u64 [size=8, align=8];
// COMPATIBLE-NEXT:         storage i128, u128 [size=16, align=16];
// COMPATIBLE-NEXT:         storage bf16 [size=2, align=2];
// COMPATIBLE-NEXT:         storage f16 [size=2, align=2];
// COMPATIBLE-NEXT:         storage f32 [size=4, align=4];
// COMPATIBLE-NEXT:         storage f64 [size=8, align=8];
// COMPATIBLE-NEXT:         storage f80 [size=16, align=16];
// COMPATIBLE-NEXT:         storage f128 [size=16, align=16];
// COMPATIBLE-NEXT:         storage d32 [size=4, align=4];
// COMPATIBLE-NEXT:         storage d64 [size=8, align=8];
// COMPATIBLE-NEXT:         storage d128 [size=16, align=16];
// COMPATIBLE-NEXT:     }
// COMPATIBLE-NEXT:     fn %0 @f(%2 <unnamed>: i32) -> i32 [linkage=external];
// COMPATIBLE-NEXT:     fn %1 @g(%3 <unnamed>: i32) -> i32 [linkage=external];
// COMPATIBLE-NEXT: }
// SLATE-FILECHECK-END COMPATIBLE
// SLATE-FILECHECK-BEGIN SAME_TAGS_C23
// SAME_TAGS_C23: module {
// SAME_TAGS_C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// SAME_TAGS_C23-NEXT:         endian = little;
// SAME_TAGS_C23-NEXT:         pointer [size=8, align=8];
// SAME_TAGS_C23-NEXT:         stack_alignment = 16;
// SAME_TAGS_C23-NEXT:         long_double = f80;
// SAME_TAGS_C23-NEXT:         storage bool [size=1, align=1];
// SAME_TAGS_C23-NEXT:         storage i8, u8 [size=1, align=1];
// SAME_TAGS_C23-NEXT:         storage i16, u16 [size=2, align=2];
// SAME_TAGS_C23-NEXT:         storage i32, u32 [size=4, align=4];
// SAME_TAGS_C23-NEXT:         storage i64, u64 [size=8, align=8];
// SAME_TAGS_C23-NEXT:         storage i128, u128 [size=16, align=16];
// SAME_TAGS_C23-NEXT:         storage bf16 [size=2, align=2];
// SAME_TAGS_C23-NEXT:         storage f16 [size=2, align=2];
// SAME_TAGS_C23-NEXT:         storage f32 [size=4, align=4];
// SAME_TAGS_C23-NEXT:         storage f64 [size=8, align=8];
// SAME_TAGS_C23-NEXT:         storage f80 [size=16, align=16];
// SAME_TAGS_C23-NEXT:         storage f128 [size=16, align=16];
// SAME_TAGS_C23-NEXT:         storage d32 [size=4, align=4];
// SAME_TAGS_C23-NEXT:         storage d64 [size=8, align=8];
// SAME_TAGS_C23-NEXT:         storage d128 [size=16, align=16];
// SAME_TAGS_C23-NEXT:     }
// SAME_TAGS_C23-NEXT:     type @type0 S = struct {
// SAME_TAGS_C23-NEXT:         field0 a: i32;
// SAME_TAGS_C23-NEXT:     } [size=4, align=4, offsets=[0]];
// SAME_TAGS_C23-NEXT:     type @type1 E = enum : u32 {
// SAME_TAGS_C23-NEXT:         %0 A = const<i32>(0);
// SAME_TAGS_C23-NEXT:     } [size=4, align=4];
// SAME_TAGS_C23-NEXT:     global %3 s: @type0 [storage=static] [linkage=external];
// SAME_TAGS_C23-NEXT:     global %4 e: @type1 [storage=static] [linkage=external];
// SAME_TAGS_C23-NEXT: }
// SLATE-FILECHECK-END SAME_TAGS_C23
