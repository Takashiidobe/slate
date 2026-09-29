/* Valid __thread specifiers.  */
/* { dg-require-effective-target tls } */

__thread int g1;
extern __thread int g2;
static __thread int g3;

void foo()
{
  extern __thread int l1;
  static __thread int l2;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     global %[[VALUE_g1:[0-9]+]] g1: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_g2:[0-9]+]] g2: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3:[0-9]+]] g3: i32 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE_l1:[0-9]+]] l1: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l2:[0-9]+]] l2: i32 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
