/* PR middle-end/59138 */
/* Testcase by John Regehr <regehr@cs.utah.edu> */

extern void abort(void);

#pragma pack(1)

struct S0 {
  int   f0;
  int   f1;
  int   f2;
  short f3;
};

short a = 1;

struct S0 b = {1}, c, d, e;

struct S0 fn1() { return c; }

void fn2(void) {
  b = fn1();
  a = 0;
  d = e;
}

int main(void) {
  fn2();
  if (a != 0)
    abort();
  return 0;
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
// DEFAULT-NEXT:     type @type0 S0 = struct {
// DEFAULT-NEXT:         field0 f0: i32;
// DEFAULT-NEXT:         field1 f1: i32;
// DEFAULT-NEXT:         field2 f2: i32;
// DEFAULT-NEXT:         field3 f3: i16;
// DEFAULT-NEXT:     } [size=14, align=1, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     global %2 a: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %3 b: @type0 [storage=static] = aggregate<@type0, zero_fill=true>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %4 c: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 d: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 e: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @fn1() -> @type0 [linkage=external] [abi=sysv64() -> coerce<i64, i48>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fn2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type0>(%3, copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i64, i48>>(%7)));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i64, i48>>(%7));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type0>(%5, copy<@type0, reason=assign>(read<@type0>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
