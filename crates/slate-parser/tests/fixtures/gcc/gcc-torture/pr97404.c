/* PR ipa/97404 */
/* { dg-additional-options "-fno-inline" } */

char         a, b;
long         c;
short        d, e;
long        *f = &c;
int          g;
char         h(signed char i) { return 0; }
static short j(short i, int k) { return i < 0 ? 0 : i >> k; }
void         l(void);
void         m(void) {
  e  = j(d | 9766, 11);
  *f = e;
}
void l(void) {
  a = 5 | g;
  b = h(a);
}
int main() {
  m();
  if (c != 4)
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
// DEFAULT-NEXT:     global %0 a: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: ptr<i64> [storage=static] = addr_of<ptr<i64>>(%2) [linkage=external];
// DEFAULT-NEXT:     global %6 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %7 @h(%8 i: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @j(%10 i: i16, %11 k: i32) -> i16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(conditional<i32>(lt<i32>(widen<i32, reason=promotion>(read<i16>(%10)), const<i32>(0)), const<i32>(0), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%10)), read<i32>(%11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @l() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%0, truncate<i8, reason=assign, fits=unknown>(or<i32>(const<i32>(5), read<i32>(%6))));
// DEFAULT-NEXT:         write<i8>(%1, call<i8, signature=fn(i8) -> i8>(%7, read<i8>(%0)));
// DEFAULT-NEXT:         call<i8, signature=fn(i8) -> i8>(%7, read<i8>(%0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @m() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%4, call<i16, signature=fn(i16, i32) -> i16>(%9, truncate<i16, reason=arg, fits=unknown>(or<i32>(widen<i32, reason=promotion>(read<i16>(%3)), const<i32>(9766))), const<i32>(11)));
// DEFAULT-NEXT:         call<i16, signature=fn(i16, i32) -> i16>(%9, truncate<i16, reason=arg, fits=unknown>(or<i32>(widen<i32, reason=promotion>(read<i16>(%3)), const<i32>(9766))), const<i32>(11));
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%5)), widen<i64, reason=assign>(read<i16>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
