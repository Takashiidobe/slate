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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: i64, %[[VALUE_b:[0-9]+]] b: i64, %[[VALUE_c:[0-9]+]] c: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i64>(read<i64>(%[[VALUE_a]]), widen<i64, reason=usual_arith>(const<i32>(12))), ne<i64>(read<i64>(%[[VALUE_b]]), widen<i64, reason=usual_arith>(const<i32>(1)))), ne<i64>(read<i64>(%[[VALUE_c]]), widen<i64, reason=usual_arith>(const<i32>(11))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return widen<i64, reason=return>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_a_2:[0-9]+]] a: i64, %[[VALUE_b_2:[0-9]+]] b: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_b_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_a_3:[0-9]+]] a: i64, %[[VALUE_b_3:[0-9]+]] b: i64, %[[VALUE_c_2:[0-9]+]] c: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%[[VALUE_d]], ptr_to_int<i64, reason=explicit>(read<ptr<void>>(%[[VALUE_c_2]])));
// DEFAULT-NEXT:         call<i64, signature=fn(i64, i64, i64) -> i64>(%[[VALUE_foo]], read<i64>(%[[VALUE_d]]), call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE_bar]], read<i64>(%[[VALUE_a_3]]), widen<i64, reason=arg>(const<i32>(1))), read<i64>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i64, i64, ptr<void>) -> void>(%[[VALUE_baz]], widen<i64, reason=arg>(const<i32>(10)), widen<i64, reason=arg>(const<i32>(11)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(12)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
