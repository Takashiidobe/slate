/* PR tree-optimization/111469 */

long                           f;
char                          *g;
__attribute__((noinline)) char o() {
  char l;
  while (f)
    ;
  l = *g;
  return l;
}

/* factor_out_conditional_conversion is able to remove the casts
   from the 2 bbs (correctly)
   but then minmax_replacement should not optimize this to a MIN_EXPR
   as o has side effects. */

__attribute__((noinline)) unsigned short gg(unsigned short a,
                                            unsigned short b) {
  short d;
  if (a > b) {
    d = b;
  } else {
    o();
    d = a;
  }
  return d;
}

int main(void) { gg(3, 2); }


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
// DEFAULT-NEXT:     global %0 f: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 g: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @o() -> i8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 l: i8 [storage=automatic];
// DEFAULT-NEXT:         while %9 ne<i64>(read<i64>(%0), const<i64>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(deref(read<ptr<i8>>(%1))));
// DEFAULT-NEXT:         return read<i8>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @gg(%5 a: u16, %6 b: u16) -> u16 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 d: i16 [storage=automatic];
// DEFAULT-NEXT:         if gt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%5))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%6))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i16>(%7, reinterpret<i16, reason=assign, fits=unknown>(read<u16>(%6)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i8, signature=fn() -> i8>(%2);
// DEFAULT-NEXT:                 write<i16>(%7, reinterpret<i16, reason=assign, fits=unknown>(read<u16>(%5)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(read<i16>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<u16, signature=fn(u16, u16) -> u16>(%4, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(3))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
