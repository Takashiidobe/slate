/* Test for _Thread_local in C11.  Test of valid code.  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors" } */

_Thread_local int a;
static _Thread_local long b;
extern _Thread_local int c, a;
_Thread_local static int d;
long _Thread_local extern b;
_Thread_local int extern a;
_Thread_local struct s; /* { dg-warning "useless" } */
_Thread_local int a = 1;
extern _Thread_local int c = 2; /* { dg-warning "initialized and" } */
void
f (void)
{
  static _Thread_local int x;
  extern _Thread_local long b;
  _Thread_local extern int a;
}

inline void
fi (void)
{
  static _Thread_local const int v;
  (void) a;
  static _Thread_local int (*const p)[a];
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct incomplete;
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=thread] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i64 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=thread] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i32 [storage=thread] [const] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<vla<i32, %[[VALUE0:[0-9]+]]>> [storage=thread] [const] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fi:[0-9]+]] @fi() -> void [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
