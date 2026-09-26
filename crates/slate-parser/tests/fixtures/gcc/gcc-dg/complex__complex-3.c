/* Verify that rtl expansion cleanup doesn't get too aggressive about
   code dealing with complex CONCATs.  */
/* { dg-do run } */
/* { dg-options "-O -fno-tree-sra" } */

extern void abort(void);
extern void exit(int);

__complex__ float foo(void) {
  __complex__ float f[1];
  __real__ f[0] = 1;
  __imag__ f[0] = 1;
  f[0]          = __builtin_conjf(f[0]);
  return f[0];
}

int main(void) {
  __complex__ double d[1];
  d[0] = foo();
  if (__real__ d[0] != 1 || __imag__ d[0] != -1)
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
// DEFAULT-NEXT:     fn %1 @exit(%6 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @__builtin_conjf(%7 <unnamed>: complex<f32>) -> complex<f32> [linkage=external] [memory=none] [abi=sysv64(coerce<pair<f32>>) -> coerce<pair<f32>>];
// DEFAULT-NEXT:     fn %2 @foo() -> complex<f32> [linkage=external] [abi=sysv64() -> coerce<pair<f32>>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 f: array<complex<f32>, 1> [storage=automatic];
// DEFAULT-NEXT:         write<f32>(real(deref(ptr_offset<ptr<complex<f32>>, subtract=false, element=complex<f32>, overflow=ub>(array_decay<ptr<complex<f32>>, length=Some(1)>(%3), const<i32>(0)))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<f32>(imag(deref(ptr_offset<ptr<complex<f32>>, subtract=false, element=complex<f32>, overflow=ub>(array_decay<ptr<complex<f32>>, length=Some(1)>(%3), const<i32>(0)))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(deref(ptr_offset<ptr<complex<f32>>, subtract=false, element=complex<f32>, overflow=ub>(array_decay<ptr<complex<f32>>, length=Some(1)>(%3), const<i32>(0))), call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(coerce<pair<f32>>) -> coerce<pair<f32>>>(%8, read<complex<f32>>(deref(ptr_offset<ptr<complex<f32>>, subtract=false, element=complex<f32>, overflow=ub>(array_decay<ptr<complex<f32>>, length=Some(1)>(%3), const<i32>(0))))));
// DEFAULT-NEXT:         return read<complex<f32>>(deref(ptr_offset<ptr<complex<f32>>, subtract=false, element=complex<f32>, overflow=ub>(array_decay<ptr<complex<f32>>, length=Some(1)>(%3), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 d: array<complex<f64>, 1> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<complex<f64>>(deref(ptr_offset<ptr<complex<f64>>, subtract=false, element=complex<f64>, overflow=ub>(array_decay<ptr<complex<f64>>, length=Some(1)>(%5), const<i32>(0))), complex_convert<complex<f64>, reason=assign>(call<complex<f32>, signature=fn() -> complex<f32>, abi=sysv64() -> coerce<pair<f32>>>(%2)));
// DEFAULT-NEXT:         complex_convert<complex<f64>, reason=assign>(call<complex<f32>, signature=fn() -> complex<f32>, abi=sysv64() -> coerce<pair<f32>>>(%2));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(real(deref(ptr_offset<ptr<complex<f64>>, subtract=false, element=complex<f64>, overflow=ub>(array_decay<ptr<complex<f64>>, length=Some(1)>(%5), const<i32>(0))))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<f64, exceptions=ignore>(read<f64>(imag(deref(ptr_offset<ptr<complex<f64>>, subtract=false, element=complex<f64>, overflow=ub>(array_decay<ptr<complex<f64>>, length=Some(1)>(%5), const<i32>(0))))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
