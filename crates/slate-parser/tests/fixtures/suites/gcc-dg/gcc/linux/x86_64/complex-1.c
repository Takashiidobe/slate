/* { dg-do run } */
/* { dg-options "-O" } */

/* Verify that the 6th complex floating-point argument is
   correctly passed as unnamed argument on SPARC64.  */

extern void abort(void);   

void foo(long arg1, long arg2, long arg3, long arg4, long arg5, ...)
{
  __builtin_va_list ap;
  _Complex float cf;

  __builtin_va_start(ap, arg5);
  cf = __builtin_va_arg(ap, _Complex float);
  __builtin_va_end(ap);

  if (__imag__ cf != 2.0f)
    abort();
}

int bar(long arg1, long arg2, long arg3, long arg4, long arg5, _Complex float arg6)
{
  foo(arg1, arg2, arg3, arg4, arg5, arg6);
  return 0;
}

int main(void)
{
  return bar(0, 0, 0, 0, 0, 2.0fi);
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %1 @foo(%2 arg1: i64, %3 arg2: i64, %4 arg3: i64, %5 arg4: i64, %6 arg5: i64, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %8 cf: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         va_start(%7);
// DEFAULT-NEXT:         write<complex<f32>>(%8, va_arg<complex<f32>>(%7));
// DEFAULT-NEXT:         va_arg<complex<f32>>(%7);
// DEFAULT-NEXT:         va_end(%7);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(imag(%8)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @bar(%10 arg1: i64, %11 arg2: i64, %12 arg3: i64, %13 arg4: i64, %14 arg5: i64, %15 arg6: complex<f32>) -> i32 [linkage=external] [abi=sysv64(scalar, scalar, scalar, scalar, scalar, coerce<pair<f32>>) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(i64, i64, i64, i64, i64, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, coerce<pair<f32>>) -> void>(%1, read<i64>(%10), read<i64>(%11), read<i64>(%12), read<i64>(%13), read<i64>(%14), read<complex<f32>>(%15));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i64, i64, i64, i64, i64, complex<f32>) -> i32, abi=sysv64(scalar, scalar, scalar, scalar, scalar, coerce<pair<f32>>) -> scalar>(%9, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(2.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
