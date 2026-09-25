/* { dg-do run } */
/* Tail call optimizations would reverse the order of additions in func().  */

void abort(void);
void exit(int);

double func(const double *array) {
  double d = *array;
  if (d == 0.0)
    return d;
  else
    return d + func(array + 1);
}

int main() {
  double values[] = {0.1e-100, 1.0, -1.0, 0.0};
  if (func(values) != 0.1e-100)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @func(%3 array: ptr<const f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 d: f64 [storage=automatic] = read<f64>(deref(read<ptr<const f64>>(%3)));
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%4), const<f64>(0.0))
// DEFAULT-NEXT:             return read<f64>(%4);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%4), call<f64, signature=fn(ptr<const f64>) -> f64>(%2, ptr_offset<ptr<const f64>, subtract=false, element=f64, overflow=ub>(read<ptr<const f64>>(%3), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 values: array<f64, 4> [storage=automatic] = aggregate<array<f64, 4>, zero_fill=false>(index0 = const<f64>(1e-101), index1 = const<f64>(1.0), index2 = neg<f64>(const<f64>(1.0)), index3 = const<f64>(0.0));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(ptr<const f64>) -> f64>(%2, pointer_cast<ptr<const f64>, reason=arg>(array_decay<ptr<f64>, length=Some(4)>(%6))), const<f64>(1e-101))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
