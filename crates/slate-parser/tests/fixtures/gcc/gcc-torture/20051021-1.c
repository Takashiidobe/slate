/* Verify that TRUTH_AND_EXPR is not wrongly changed to TRUTH_ANDIF_EXPR.  */

extern void abort(void);

int count = 0;

int foo1(void) {
  count++;
  return 0;
}

int foo2(void) {
  count++;
  return 0;
}

int main(void) {
  if ((foo1() == 1) & (foo2() == 1))
    abort();

  if (count != 2)
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
// DEFAULT-NEXT:     global %1 count: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %6: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%5), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:         let %8: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%8));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(and<i32>(from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(1))), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn() -> i32>(%3), const<i32>(1)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
