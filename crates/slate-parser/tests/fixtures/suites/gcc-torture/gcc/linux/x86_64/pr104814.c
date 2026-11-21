/* PR rtl-optimization/104814 */

short       a = 0;
static long b = 0;
int         c = 7;
char        d = 0;
short      *e = &a;
long        f = 0;

unsigned long foo(unsigned long h, long j) { return j == 0 ? h : h / j; }

int main() {
  long k = f;
  for (; c; --c) {
    for (int i = 0; i < 7; ++i)
      ;
    long m = foo(f, --b);
    d      = ((char)m | *e) <= 43165;
  }
  if (b != -7)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: ptr<i16> [storage=static] = addr_of<ptr<i16>>(%[[VALUE_a]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_h:[0-9]+]] h: u64, %[[VALUE_j:[0-9]+]] j: i64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u64>(eq<i64>(read<i64>(%[[VALUE_j]]), widen<i64, reason=usual_arith>(const<i32>(0))), read<u64>(%[[VALUE_h]]), div<u64, by_zero=ub>(read<u64>(%[[VALUE_h]]), reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%[[VALUE_j]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i64 [storage=automatic] = read<i64>(%[[VALUE_f]]);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(7))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                     let %[[VALUE_m:[0-9]+]] m: i64 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_b]]);
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE6]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_b]], read<i64>(%[[VALUE7]]));
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_m]], reinterpret<i64, reason=assign, fits=unknown>(call<u64, signature=fn(u64, i64) -> u64>(%[[VALUE_foo]], reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%[[VALUE_f]])), read<i64>(%[[VALUE7]]))));
// DEFAULT-NEXT:                     write<i8>(%[[VALUE_d]], from_bool<i8, reason=assign>(le<i32>(or<i32>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_m]]))), widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%[[VALUE_e]]))))), const<i32>(43165))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_b]]), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(7))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
