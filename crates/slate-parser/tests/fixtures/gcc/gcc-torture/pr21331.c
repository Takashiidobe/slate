void abort(void);

int bar(void) { return -1; }

unsigned long foo() {
  unsigned long retval;
  retval = bar();
  if (retval == -1)
    return 0;
  return 3;
}

int main(void) {
  if (foo() != 0)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @foo() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 retval: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%3, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%1))));
// DEFAULT-NEXT:         reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%1)));
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%3), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(0)));
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(const<i32>(3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn() -> u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
