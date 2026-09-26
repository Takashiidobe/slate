/* { dg-require-effective-target int32plus } */
/* { dg-options "-fno-strict-overflow" } */

extern void abort(void);
extern void exit(int);

__attribute__((noinline)) void foo(short unsigned int *p1,
                                   short unsigned int *p2) {
  short unsigned int x1, x4;
  int                x2, x3, x5, x6;
  unsigned int       x7;

  x1 = *p1;
  x2 = (int)x1;
  x3 = x2 * 65536;
  x4 = *p2;
  x5 = (int)x4;
  x6 = x3 + x4;
  x7 = (unsigned int)x6;
  if (x7 <= 268435455U)
    abort();
  exit(0);
}

int main() {
  short unsigned int x, y;
  x = -5;
  y = -10;
  foo(&x, &y);
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%15 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 p1: ptr<u16>, %4 p2: ptr<u16>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 x1: u16 [storage=automatic];
// DEFAULT-NEXT:         let %6 x4: u16 [storage=automatic];
// DEFAULT-NEXT:         let %7 x2: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 x3: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 x5: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 x6: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 x7: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(%5, read<u16>(deref(read<ptr<u16>>(%3))));
// DEFAULT-NEXT:         write<i32>(%7, reinterpret<i32, reason=explicit, fits=unknown>(widen<u32, reason=explicit>(read<u16>(%5))));
// DEFAULT-NEXT:         write<i32>(%8, mul<i32, overflow=ub>(read<i32>(%7), const<i32>(65536)));
// DEFAULT-NEXT:         write<u16>(%6, read<u16>(deref(read<ptr<u16>>(%4))));
// DEFAULT-NEXT:         write<i32>(%9, reinterpret<i32, reason=explicit, fits=unknown>(widen<u32, reason=explicit>(read<u16>(%6))));
// DEFAULT-NEXT:         write<i32>(%10, add<i32, overflow=ub>(read<i32>(%8), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%6)))));
// DEFAULT-NEXT:         write<u32>(%11, reinterpret<u32, reason=explicit, fits=unknown>(read<i32>(%10)));
// DEFAULT-NEXT:         if le<u32>(read<u32>(%11), const<u32>(268435455))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 x: u16 [storage=automatic];
// DEFAULT-NEXT:         let %14 y: u16 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(%13, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(5)))));
// DEFAULT-NEXT:         write<u16>(%14, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(10)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u16>, ptr<u16>) -> void>(%2, addr_of<ptr<u16>>(%13), addr_of<ptr<u16>>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
