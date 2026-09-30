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
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_o:[0-9]+]] @o() -> i8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i8 [storage=automatic];
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i64>(read<i64>(%[[VALUE_f]]), const<i64>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         write<i8>(%[[VALUE_l]], read<i8>(deref(read<ptr<i8>>(%[[VALUE_g]]))));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE_l]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gg:[0-9]+]] @gg(%[[VALUE_a:[0-9]+]] a: u16, %[[VALUE_b:[0-9]+]] b: u16) -> u16 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i16 [storage=automatic];
// DEFAULT-NEXT:         if gt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_a]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_b]]))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_d]], reinterpret<i16, reason=assign, fits=unknown>(read<u16>(%[[VALUE_b]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i8, signature=fn() -> i8>(%[[VALUE_o]]);
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_d]], reinterpret<i16, reason=assign, fits=unknown>(read<u16>(%[[VALUE_a]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(read<i16>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<u16, signature=fn(u16, u16) -> u16>(%[[VALUE_gg]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(3))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
