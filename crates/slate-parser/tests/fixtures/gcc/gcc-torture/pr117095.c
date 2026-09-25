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
// DEFAULT-NEXT:     global %0 a: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 z: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 g: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 h: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%2) [linkage=external];
// DEFAULT-NEXT:     global %8 i: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 j: volatile i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 k: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 l: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %12 @foo(%13 n: i8, %14 o: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%13)), widen<i32, reason=promotion>(read<i8>(%14))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @bar(%16 n: i8, %17 o: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(conditional<i32>(eq<i32>(widen<i32, reason=promotion>(read<i8>(%17)), const<i32>(0)), widen<i32, reason=promotion>(read<i8>(%16)), div<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%16)), widen<i32, reason=promotion>(read<i8>(%17)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @baz(%19 n: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%19)), widen<i32, reason=promotion>(read<i16>(%0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %21 q: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%11);
// DEFAULT-NEXT:         let %22 s: ptr<ptr<i32>> [storage=automatic] = addr_of<ptr<ptr<i32>>>(%8);
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%22)), addr_of<ptr<i32>>(%4));
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%3), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%26));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %24
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i64>(%6, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:                     condition: le<i64>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %27: i64 [synthetic] = read<i64>(%6);
// DEFAULT-NEXT:                         let %28: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%27), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                         write<i64>(%6, read<i64>(%28));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i8>(%10, call<i8, signature=fn(i8, i8) -> i8>(%12, truncate<i8, reason=arg, fits=always>(const<i32>(9)), truncate<i8, reason=arg, fits=unknown>(read<i32>(deref(read<ptr<i32>>(%8))))));
// DEFAULT-NEXT:                             call<i8, signature=fn(i8, i8) -> i8>(%12, truncate<i8, reason=arg, fits=always>(const<i32>(9)), truncate<i8, reason=arg, fits=unknown>(read<i32>(deref(read<ptr<i32>>(%8)))));
// DEFAULT-NEXT:                             write<i32>(deref(read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%22)))), widen<i32, reason=assign>(call<i8, signature=fn(i8, i8) -> i8>(%15, from_bool<i8, reason=arg>(gt<i64>(read<i64>(%5), widen<i64, reason=usual_arith>(const<i32>(1)))), truncate<i8, reason=arg, fits=unknown>(xor<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i128>(read<i128, volatile>(%9), const<i128>(0)))), widen<i32, reason=promotion>(read<i8>(%10)))))));
// DEFAULT-NEXT:                             widen<i32, reason=assign>(call<i8, signature=fn(i8, i8) -> i8>(%15, from_bool<i8, reason=arg>(gt<i64>(read<i64>(%5), widen<i64, reason=usual_arith>(const<i32>(1)))), truncate<i8, reason=arg, fits=unknown>(xor<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(1), const<i32>(0)), ne<i128>(read<i128, volatile>(%9), const<i128>(0)))), widen<i32, reason=promotion>(read<i8>(%10))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         write<i16>(%1, call<i16, signature=fn(i16) -> i16>(%18, from_bool<i16, reason=arg>(eq<ptr<i8>>(read<ptr<i8>>(%21), addr_of<ptr<i8>>(%11)))));
// DEFAULT-NEXT:         call<i16, signature=fn(i16) -> i16>(%18, from_bool<i16, reason=arg>(eq<ptr<i8>>(read<ptr<i8>>(%21), addr_of<ptr<i8>>(%11))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%7)), widen<i32, reason=assign>(read<i16>(%1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
