void abort(void);
void exit(int);

void f(double *ty) { *ty = -1.0; }

int main(void) {
  double foo[6];
  double tx = 0.0, ty, d;

  f(&ty);

  if (ty < 0)
    ty = -ty;
  d = (tx > ty) ? tx : ty;
  if (ty != d)
    abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_ty:[0-9]+]] ty: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(deref(read<ptr<f64>>(%[[VALUE_ty]])), neg<f64>(const<f64>(1.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_foo:[0-9]+]] foo: array<f64, 6> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_tx:[0-9]+]] tx: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %[[VALUE_ty_2:[0-9]+]] ty: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: f64 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f64>) -> void>(%[[VALUE_f]], addr_of<ptr<f64>>(%[[VALUE_ty_2]]));
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(read<f64>(%[[VALUE_ty_2]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:             write<f64>(%[[VALUE_ty_2]], neg<f64>(read<f64>(%[[VALUE_ty_2]])));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_d]], conditional<f64>(gt<f64, exceptions=ignore>(read<f64>(%[[VALUE_tx]]), read<f64>(%[[VALUE_ty_2]])), read<f64>(%[[VALUE_tx]]), read<f64>(%[[VALUE_ty_2]])));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_ty_2]]), read<f64>(%[[VALUE_d]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
