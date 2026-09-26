/* PR tree-optimization/56962 */

extern void abort(void);
long long   v[144];

__attribute__((noinline, noclone)) void bar(long long *x) {
  if (x != &v[29])
    abort();
}

__attribute__((noinline, noclone)) void foo(long long *x, long y, long z) {
  long long a, b, c;
  a        = x[z * 4 + y * 3];
  b        = x[z * 5 + y * 3];
  c        = x[z * 5 + y * 4];
  x[y * 4] = a;
  bar(&x[z * 5 + y]);
  x[z * 5 + y * 5] = b + c;
}

int main() {
  foo(v, 24, 1);
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
// DEFAULT-NEXT:     global %1 v: array<i64, 144> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @bar(%3 x: ptr<i64>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<i64>>(read<ptr<i64>>(%3), addr_of<ptr<i64>>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(144)>(%1), const<i32>(29)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo(%5 x: ptr<i64>, %6 y: i64, %7 z: i64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 a: i64 [storage=automatic];
// DEFAULT-NEXT:         let %9 b: i64 [storage=automatic];
// DEFAULT-NEXT:         let %10 c: i64 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%8, read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%5), add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(4))), mul<i64, overflow=ub>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(3))))))));
// DEFAULT-NEXT:         write<i64>(%9, read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%5), add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(5))), mul<i64, overflow=ub>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(3))))))));
// DEFAULT-NEXT:         write<i64>(%10, read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%5), add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(5))), mul<i64, overflow=ub>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(4))))))));
// DEFAULT-NEXT:         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%5), mul<i64, overflow=ub>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(4))))), read<i64>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>) -> void>(%2, addr_of<ptr<i64>>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%5), add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(5))), read<i64>(%6))))));
// DEFAULT-NEXT:         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%5), add<i64, overflow=ub>(mul<i64, overflow=ub>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(5))), mul<i64, overflow=ub>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(5)))))), add<i64, overflow=ub>(read<i64>(%9), read<i64>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>, i64, i64) -> void>(%4, array_decay<ptr<i64>, length=Some(144)>(%1), widen<i64, reason=arg>(const<i32>(24)), widen<i64, reason=arg>(const<i32>(1)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
