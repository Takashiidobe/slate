/* PR target/9348 */

void abort(void);

#define u_l_l unsigned long long
#define l_l   long long

l_l mpy_res;

u_l_l mpy(long a, long b) { return (u_l_l)a * (u_l_l)b; }

int main(void) {
  mpy_res = mpy(1, -1);
  if (mpy_res != -1LL)
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
// DEFAULT-NEXT:     global %[[VALUE_mpy_res:[0-9]+]] mpy_res: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_mpy:[0-9]+]] @mpy(%[[VALUE_a:[0-9]+]] a: i64, %[[VALUE_b:[0-9]+]] b: i64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_a]])), reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_b]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i64>(%[[VALUE_mpy_res]], reinterpret<i64, reason=assign, fits=unknown>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_mpy]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         reinterpret<i64, reason=assign, fits=unknown>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_mpy]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_mpy_res]]), neg<i64, overflow=ub>(const<i64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
