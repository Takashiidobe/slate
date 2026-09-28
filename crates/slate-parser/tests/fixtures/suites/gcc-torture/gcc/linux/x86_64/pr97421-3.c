/* PR rtl-optimization/97421 */
/* { dg-additional-options "-fmodulo-sched" } */

int   a, b, c;
short d;
void  e(void) {
  unsigned f = 0;
  for (; f <= 2; f++) {
    int g[1];
    int h = (long)g;
    c     = 0;
    for (; c < 10; c++)
      g[0] = a = 0;
    for (; a <= 2; a++)
      b = d;
  }
}
int main(void) {
  e();
  if (a != 3)
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @e() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 f: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: le<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                 let %14: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%13), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%5, read<u32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %6 g: array<i32, 1> [storage=automatic];
// DEFAULT-NEXT:                     let %7 h: i32 [storage=automatic] = truncate<i32, reason=assign, fits=unknown>(ptr_to_int<i64, reason=explicit>(array_decay<ptr<i32>, length=Some(1)>(%6)));
// DEFAULT-NEXT:                     write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:                     for %10
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%2), const<i32>(10))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %15: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                             let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%2, read<i32>(%16));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<i32>(%0, const<i32>(0));
// DEFAULT-NEXT:                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1)>(%6), const<i32>(0))), const<i32>(0));
// DEFAULT-NEXT:                     for %11
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: le<i32>(read<i32>(%0), const<i32>(2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %17: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                             let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%0, read<i32>(%18));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<i32>(%1, widen<i32, reason=assign>(read<i16>(%3)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
