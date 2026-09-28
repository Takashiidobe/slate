void abort(void);
void exit(int);

__complex__ double f() {
  int                a[40];
  __complex__ double c;

  a[9] = 0;
  c    = a[9];
  return c;
}

int main(void) {
  __complex__ double c;

  if (c = f())
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @f() -> complex<f64> [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 a: array<i32, 40> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %4 c: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(40)>(%3), const<i32>(9))), const<i32>(0));
// DEFAULT-NEXT:         write<complex<f64>>(%4, real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(40)>(%3), const<i32>(9)))))));
// DEFAULT-NEXT:         return read<complex<f64>>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 c: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f64>>(%6, call<complex<f64>, signature=fn() -> complex<f64>, abi=sysv64() -> native_c>(%2));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=ignore>(call<complex<f64>, signature=fn() -> complex<f64>, abi=sysv64() -> native_c>(%2), real_to_complex<complex<f64>, reason=usual_arith>(const<f64>(0.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
