// SLATE-FILECHECK-DEFINES DEFAULT

/* PR bootstrap/4128 */

extern int bar (char *, char *, int, int);
extern long baz (char *, char *, int, int);

int sgt (char *a, char *b, int c, int d)
{
  return bar (a, b, c, d) > 0;
}

long dgt (char *a, char *b, int c, int d)
{
  return baz (a, b, c, d) > 0;
}

int sne (char *a, char *b, int c, int d)
{
  return bar (a, b, c, d) != 0;
}

long dne (char *a, char *b, int c, int d)
{
  return baz (a, b, c, d) != 0;
}

int seq (char *a, char *b, int c, int d)
{
  return bar (a, b, c, d) == 0;
}

long deq (char *a, char *b, int c, int d)
{
  return baz (a, b, c, d) == 0;
}

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
// DEFAULT-NEXT:     fn %0 @bar(%32 <unnamed>: ptr<i8>, %33 <unnamed>: ptr<i8>, %34 <unnamed>: i32, %35 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @baz(%36 <unnamed>: ptr<i8>, %37 <unnamed>: ptr<i8>, %38 <unnamed>: i32, %39 <unnamed>: i32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @sgt(%3 a: ptr<i8>, %4 b: ptr<i8>, %5 c: i32, %6 d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(call<i32, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i32>(%0, read<ptr<i8>>(%3), read<ptr<i8>>(%4), read<i32>(%5), read<i32>(%6)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @dgt(%8 a: ptr<i8>, %9 b: ptr<i8>, %10 c: i32, %11 d: i32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i64, reason=return>(gt<i64>(call<i64, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i64>(%1, read<ptr<i8>>(%8), read<ptr<i8>>(%9), read<i32>(%10), read<i32>(%11)), widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @sne(%13 a: ptr<i8>, %14 b: ptr<i8>, %15 c: i32, %16 d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i32>(call<i32, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i32>(%0, read<ptr<i8>>(%13), read<ptr<i8>>(%14), read<i32>(%15), read<i32>(%16)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @dne(%18 a: ptr<i8>, %19 b: ptr<i8>, %20 c: i32, %21 d: i32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i64, reason=return>(ne<i64>(call<i64, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i64>(%1, read<ptr<i8>>(%18), read<ptr<i8>>(%19), read<i32>(%20), read<i32>(%21)), widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @seq(%23 a: ptr<i8>, %24 b: ptr<i8>, %25 c: i32, %26 d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(call<i32, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i32>(%0, read<ptr<i8>>(%23), read<ptr<i8>>(%24), read<i32>(%25), read<i32>(%26)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @deq(%28 a: ptr<i8>, %29 b: ptr<i8>, %30 c: i32, %31 d: i32) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i64, reason=return>(eq<i64>(call<i64, signature=fn(ptr<i8>, ptr<i8>, i32, i32) -> i64>(%1, read<ptr<i8>>(%28), read<ptr<i8>>(%29), read<i32>(%30), read<i32>(%31)), widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
