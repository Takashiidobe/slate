void abort(void);
void exit(int);

long foo(long a, long b, long c) {
  if (a != 12 || b != 1 || c != 11)
    abort();
  return 0;
}
long bar(long a, long b) { return b; }
void baz(long a, long b, void *c) {
  long d;
  d = (long)c;
  foo(d, bar(a, 1), b);
}
int main() {
  baz(10, 11, (void *)12);
  exit(0);
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
// DEFAULT-NEXT:     fn %2 @foo(%3 a: i64, %4 b: i64, %5 c: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i64>(read<i64>(%3), widen<i64, reason=usual_arith>(const<i32>(12))), ne<i64>(read<i64>(%4), widen<i64, reason=usual_arith>(const<i32>(1)))), ne<i64>(read<i64>(%5), widen<i64, reason=usual_arith>(const<i32>(11))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return widen<i64, reason=return>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar(%7 a: i64, %8 b: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i64>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @baz(%10 a: i64, %11 b: i64, %12 c: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 d: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%13, ptr_to_int<i64, reason=explicit>(read<ptr<void>>(%12)));
// DEFAULT-NEXT:         call<i64, signature=fn(i64, i64, i64) -> i64>(%2, read<i64>(%13), call<i64, signature=fn(i64, i64) -> i64>(%6, read<i64>(%10), widen<i64, reason=arg>(const<i32>(1))), read<i64>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i64, i64, ptr<void>) -> void>(%9, widen<i64, reason=arg>(const<i32>(10)), widen<i64, reason=arg>(const<i32>(11)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(12)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
