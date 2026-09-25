/* { dg-do run { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-skip-if "" { ! run_expensive_tests }  { "*" } { "-O0" "-O2" } } */
/* { dg-skip-if "" { ! run_expensive_tests } { "-flto" } { "" } } */

#if __BITINT_MAXWIDTH__ >= 1024
constexpr _BitInt(1024) d =
    -541140097068598424394740839221562143161511518875518765552323978870598341733206554363735813878577506997168480201818027232521wb;
int c;

static inline void foo(_BitInt(1024) b, _BitInt(1024) * r) {
  if (c)
    b = 0;
  *r = b;
}

[[gnu::noipa]] void bar(_BitInt(1024) y) {
  if (y != d)
    __builtin_abort();
}
#endif

int main() {
#if __BITINT_MAXWIDTH__ >= 1024
  _BitInt(1024) x;
  foo(d, &x);
  bar(x);
#endif
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
// DEFAULT-NEXT:     global %0 d: i1024b [storage=static] [const] [constexpr] = widen<i1024b, reason=assign>(neg<i409b, overflow=ub>(const<i409b>(541140097068598424394740839221562143161511518875518765552323978870598341733206554363735813878577506997168480201818027232521))) [linkage=internal];
// DEFAULT-NEXT:     global %1 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 b: i1024b, %4 r: ptr<i1024b>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             write<i1024b>(%3, widen<i1024b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i1024b>(deref(read<ptr<i1024b>>(%4)), read<i1024b>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 y: i1024b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i1024b>(read<i1024b>(%6), read<i1024b>(%0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 x: i1024b [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i1024b, ptr<i1024b>) -> void>(%2, read<i1024b>(%0), addr_of<ptr<i1024b>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(i1024b) -> void>(%5, read<i1024b>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
