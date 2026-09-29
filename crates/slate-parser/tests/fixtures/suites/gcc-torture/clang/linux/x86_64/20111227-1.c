/* PR rtl-optimization/51667 */
/* Testcase by Uros Bizjak <ubizjak@gmail.com> */

extern void abort(void);

void __attribute__((noinline, noclone)) bar(int a) {
  if (a != -1)
    abort();
}

void __attribute__((noinline, noclone)) foo(short *a, int t) {
  short r = *a;

  if (t)
    bar((unsigned short)r);
  else
    bar((signed short)r);
}

short v = -1;

int main(void) {
  foo(&v, 0);
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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i16 [storage=static] = truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_a:[0-9]+]] a: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a]]), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a_2:[0-9]+]] a: ptr<i16>, %[[VALUE_t:[0-9]+]] t: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i16 [storage=automatic] = read<i16>(deref(read<ptr<i16>>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], reinterpret<i32, reason=arg, fits=unknown>(widen<u32, reason=arg>(reinterpret<u16, reason=explicit, fits=unknown>(read<i16>(%[[VALUE_r]])))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], widen<i32, reason=arg>(read<i16>(%[[VALUE_r]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i16>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<i16>>(%[[VALUE_v]]), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
