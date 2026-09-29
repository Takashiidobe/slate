/* PR rtl-optimization/117095 */

short a, b;
int   c, e, z;
long  f, g;
int  *h = &c, *i;
#ifdef __SIZEOF_INT128__
volatile __int128 j;
#else
volatile long long j;
#endif
char k, l;

char foo(char n, char o) { return n + o; }

char bar(char n, char o) { return o == 0 ? n : n / o; }

short baz(short n) { return n - a; }

int main() {
  char *q = &l;
  int **s = &i;
  *s      = &z;
  for (e = 0; e <= 5; e++)
    for (g = 1; g <= 5; g++) {
      k   = foo(9, *i);
      **s = bar(f > 1, (1 && j) ^ k);
    }
  b  = baz(q == &l);
  *h = b;
  if (c != 1)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z:[0-9]+]] z: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_c]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: volatile i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_n:[0-9]+]] n: i8, %[[VALUE_o:[0-9]+]] o: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_o]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_n_2:[0-9]+]] n: i8, %[[VALUE_o_2:[0-9]+]] o: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(conditional<i32>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_o_2]])), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_2]])), div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_n_2]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_o_2]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_n_3:[0-9]+]] n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_n_3]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_a]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_l]]);
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%[[VALUE_i]]);
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_s]])), addr_of<ptr<i32>>(%[[VALUE_z]]));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_e]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i64>(%[[VALUE_g]], widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                     condition: le<i64>(read<i64>(%[[VALUE_g]]), widen<i64, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_g]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE4]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                         write<i64>(%[[VALUE_g]], read<i64>(%[[VALUE5]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i8>(%[[VALUE_k]], call<i8, signature=fn(i8, i8) -> i8>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=always>(const<i32>(9)), truncate<i8, reason=arg, fits=unknown>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                             call<i8, signature=fn(i8, i8) -> i8>(%[[VALUE_foo]], truncate<i8, reason=arg, fits=always>(const<i32>(9)), truncate<i8, reason=arg, fits=unknown>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                             write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_s]])))), widen<i32, reason=assign>(call<i8, signature=fn(i8, i8) -> i8>(%[[VALUE_bar]], from_bool<i8, reason=arg>(gt<i64>(read<i64>(%[[VALUE_f]]), widen<i64, reason=usual_arith>(const<i32>(1)))), truncate<i8, reason=arg, fits=unknown>(xor<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i128>(read<i128, volatile>(%[[VALUE_j]]), const<i128>(0)))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_k]])))))));
// DEFAULT-NEXT:                             widen<i32, reason=assign>(call<i8, signature=fn(i8, i8) -> i8>(%[[VALUE_bar]], from_bool<i8, reason=arg>(gt<i64>(read<i64>(%[[VALUE_f]]), widen<i64, reason=usual_arith>(const<i32>(1)))), truncate<i8, reason=arg, fits=unknown>(xor<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i128>(read<i128, volatile>(%[[VALUE_j]]), const<i128>(0)))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_k]]))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         write<i16>(%[[VALUE_b]], call<i16, signature=fn(i16) -> i16>(%[[VALUE_baz]], from_bool<i16, reason=arg>(eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_q]]), addr_of<ptr<i8>>(%[[VALUE_l]])))));
// DEFAULT-NEXT:         call<i16, signature=fn(i16) -> i16>(%[[VALUE_baz]], from_bool<i16, reason=arg>(eq<ptr<i8>>(read<ptr<i8>>(%[[VALUE_q]]), addr_of<ptr<i8>>(%[[VALUE_l]]))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_h]])), widen<i32, reason=assign>(read<i16>(%[[VALUE_b]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
