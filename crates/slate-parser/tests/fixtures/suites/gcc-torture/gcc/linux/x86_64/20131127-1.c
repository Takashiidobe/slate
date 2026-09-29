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
// DEFAULT-NEXT:     type @type[[TYPE_S0:[0-9]+]] S0 = struct {
// DEFAULT-NEXT:         field0 f0: i32;
// DEFAULT-NEXT:         field1 f1: i32;
// DEFAULT-NEXT:         field2 f2: i32;
// DEFAULT-NEXT:         field3 f3: i16;
// DEFAULT-NEXT:     } [size=14, align=1, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE_S0]] [storage=static] = aggregate<@type[[TYPE_S0]], zero_fill=true>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: @type[[TYPE_S0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: @type[[TYPE_S0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: @type[[TYPE_S0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1() -> @type[[TYPE_S0]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_S0]], reason=return>(read<@type[[TYPE_S0]]>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type[[TYPE_S0]]>(%[[VALUE_b]], copy<@type[[TYPE_S0]], reason=assign>(call<@type[[TYPE_S0]], signature=fn() -> @type[[TYPE_S0]], abi=sysv64() -> native_c>(%[[VALUE_fn1]])));
// DEFAULT-NEXT:         copy<@type[[TYPE_S0]], reason=assign>(call<@type[[TYPE_S0]], signature=fn() -> @type[[TYPE_S0]], abi=sysv64() -> native_c>(%[[VALUE_fn1]]));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_a]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<@type[[TYPE_S0]]>(%[[VALUE_d]], copy<@type[[TYPE_S0]], reason=assign>(read<@type[[TYPE_S0]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fn2]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_a]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
