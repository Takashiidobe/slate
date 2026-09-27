/* { dg-do run } */

extern void abort(void) __attribute__((noreturn));

int
main() {
  int sum = 0;
  {
    int  a[20];
    int *c;
    c = a;
    asm("" : "=r"(c) : "0"(c));
    *c = 0;
    asm("" : "=r"(c) : "0"(c));
    sum += *c;
  }
  {
    long  b[10];
    long *c;
    c = b;
    asm("" : "=r"(c) : "0"(c));
    *c = 1;
    asm("" : "=r"(c) : "0"(c));
    sum += *c;
  }

  if (sum != 1)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %3 a: array<i32, 20> [storage=automatic] [align=16];
// DEFAULT-NEXT:             let %4 c: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<i32>>(%4, array_decay<ptr<i32>, length=Some(20)>(%3));
// DEFAULT-NEXT:             asm "" [dialect=att] {
// DEFAULT-NEXT:                 inlateout 0 "r" place<ptr<i32>>(%4) from read<ptr<i32>>(%4);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<i32>(deref(read<ptr<i32>>(%4)), const<i32>(0));
// DEFAULT-NEXT:             asm "" [dialect=att] {
// DEFAULT-NEXT:                 inlateout 0 "r" place<ptr<i32>>(%4) from read<ptr<i32>>(%4);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %7: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:             let %8: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%7), read<i32>(deref(read<ptr<i32>>(%4))));
// DEFAULT-NEXT:             write<i32>(%2, read<i32>(%8));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %5 b: array<i64, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:             let %6 c: ptr<i64> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<i64>>(%6, array_decay<ptr<i64>, length=Some(10)>(%5));
// DEFAULT-NEXT:             asm "" [dialect=att] {
// DEFAULT-NEXT:                 inlateout 0 "r" place<ptr<i64>>(%6) from read<ptr<i64>>(%6);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<i64>(deref(read<ptr<i64>>(%6)), widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:             asm "" [dialect=att] {
// DEFAULT-NEXT:                 inlateout 0 "r" place<ptr<i64>>(%6) from read<ptr<i64>>(%6);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %9: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:             let %10: i32 [synthetic] = truncate<i32, reason=assign, fits=unknown>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%9)), read<i64>(deref(read<ptr<i64>>(%6)))));
// DEFAULT-NEXT:             write<i32>(%2, read<i32>(%10));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
