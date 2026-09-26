/* { dg-do run } */

extern void abort(void) __attribute__((noreturn));

union setconflict {
  short a[20];
  int   b[10];
};

int
main() {
  int sum = 0;
  {
    union setconflict a;
    short            *c;
    c = a.a;
    asm("" : "=r"(c) : "0"(c));
    *c = 0;
    asm("" : "=r"(c) : "0"(c));
    sum += *c;
  }
  {
    union setconflict a;
    int              *c;
    c = a.b;
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
// DEFAULT-NEXT:     type @type0 setconflict = union {
// DEFAULT-NEXT:         field0 a: array<i16, 20>;
// DEFAULT-NEXT:         field1 b: array<i32, 10>;
// DEFAULT-NEXT:     } [size=40, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %4 a: @type0 [storage=automatic];
// DEFAULT-NEXT:             let %5 c: ptr<i16> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<i16>>(%5, array_decay<ptr<i16>, length=Some(20)>(field0(%4)));
// DEFAULT-NEXT:             asm "" {
// DEFAULT-NEXT:                 out 0 "=r" place<ptr<i16>>(%5);
// DEFAULT-NEXT:                 in 1 "0" read<ptr<i16>>(%5);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<i16>(deref(read<ptr<i16>>(%5)), truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             asm "" {
// DEFAULT-NEXT:                 out 0 "=r" place<ptr<i16>>(%5);
// DEFAULT-NEXT:                 in 1 "0" read<ptr<i16>>(%5);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %8: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:             let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%5)))));
// DEFAULT-NEXT:             write<i32>(%3, read<i32>(%9));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %6 a: @type0 [storage=automatic];
// DEFAULT-NEXT:             let %7 c: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<i32>>(%7, array_decay<ptr<i32>, length=Some(10)>(field1(%6)));
// DEFAULT-NEXT:             asm "" {
// DEFAULT-NEXT:                 out 0 "=r" place<ptr<i32>>(%7);
// DEFAULT-NEXT:                 in 1 "0" read<ptr<i32>>(%7);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<i32>(deref(read<ptr<i32>>(%7)), const<i32>(1));
// DEFAULT-NEXT:             asm "" {
// DEFAULT-NEXT:                 out 0 "=r" place<ptr<i32>>(%7);
// DEFAULT-NEXT:                 in 1 "0" read<ptr<i32>>(%7);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %10: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:             let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), read<i32>(deref(read<ptr<i32>>(%7))));
// DEFAULT-NEXT:             write<i32>(%3, read<i32>(%11));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
