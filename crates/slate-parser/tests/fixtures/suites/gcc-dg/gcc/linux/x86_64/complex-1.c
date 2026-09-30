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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_arg1:[0-9]+]] arg1: i64, %[[VALUE_arg2:[0-9]+]] arg2: i64, %[[VALUE_arg3:[0-9]+]] arg3: i64, %[[VALUE_arg4:[0-9]+]] arg4: i64, %[[VALUE_arg5:[0-9]+]] arg5: i64, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_cf:[0-9]+]] cf: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_cf]], va_arg<complex<f32>>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(imag(%[[VALUE_cf]])), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_arg1_2:[0-9]+]] arg1: i64, %[[VALUE_arg2_2:[0-9]+]] arg2: i64, %[[VALUE_arg3_2:[0-9]+]] arg3: i64, %[[VALUE_arg4_2:[0-9]+]] arg4: i64, %[[VALUE_arg5_2:[0-9]+]] arg5: i64, %[[VALUE_arg6:[0-9]+]] arg6: complex<f32>) -> i32 [linkage=external] [abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(i64, i64, i64, i64, i64, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_foo]], read<i64>(%[[VALUE_arg1_2]]), read<i64>(%[[VALUE_arg2_2]]), read<i64>(%[[VALUE_arg3_2]]), read<i64>(%[[VALUE_arg4_2]]), read<i64>(%[[VALUE_arg5_2]]), read<complex<f32>>(%[[VALUE_arg6]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i64, i64, i64, i64, i64, complex<f32>) -> i32, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> scalar>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(2.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
