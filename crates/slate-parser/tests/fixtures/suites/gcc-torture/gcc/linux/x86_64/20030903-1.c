// SLATE-FILECHECK-DEFINES DEFAULT

/* Derived from PR optimization/11700.  */
/* The compiler used to ICE during reload for m68k targets.  */

/* { dg-skip-if "exceeds eBPF stack limit" { bpf-*-* } } */

void check_complex (__complex__ double, __complex__ double,
                    __complex__ double, __complex__ int);
void check_float (double, double, double, int);
extern double _Complex conj (double _Complex);
extern double carg (double _Complex __z);

static double minus_zero;

void
conj_test (void)
{
  check_complex (conj (({ __complex__ double __retval;
			  __real__ __retval = (0.0);
			  __imag__ __retval = (0.0);
			  __retval; })),
		 ({ __complex__ double __retval;
		    __real__ __retval = (0.0);
		    __imag__ __retval = (minus_zero);
		    __retval; }), 0, 0);
}

void
carg_test (void)
{
  check_float (carg (({ __complex__ double __retval;
			__real__ __retval = (2.0);
			__imag__ __retval = (0);
			__retval; })), 0, 0, 0);
}

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
// DEFAULT-NEXT:     global %4 minus_zero: f64 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @check_complex(%10 <unnamed>: complex<f64>, %11 <unnamed>: complex<f64>, %12 <unnamed>: complex<f64>, %13 <unnamed>: complex<i32>) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c, native_c) -> void];
// DEFAULT-NEXT:     fn %1 @check_float(%14 <unnamed>: f64, %15 <unnamed>: f64, %16 <unnamed>: f64, %17 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @conj(%18 <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [memory=none] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %3 @carg(%19 __z: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %5 @conj_test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20: complex<f64> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %6 __retval: complex<f64> [storage=automatic];
// DEFAULT-NEXT:             write<f64>(real(%6), const<f64>(0.0));
// DEFAULT-NEXT:             write<f64>(imag(%6), const<f64>(0.0));
// DEFAULT-NEXT:             write<complex<f64>>(%20, read<complex<f64>>(%6));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %21: complex<f64> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %7 __retval: complex<f64> [storage=automatic];
// DEFAULT-NEXT:             write<f64>(real(%7), const<f64>(0.0));
// DEFAULT-NEXT:             write<f64>(imag(%7), read<f64>(%4));
// DEFAULT-NEXT:             write<complex<f64>>(%21, read<complex<f64>>(%7));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn(complex<f64>, complex<f64>, complex<f64>, complex<i32>) -> void, abi=sysv64(native_c, native_c, native_c, native_c) -> void>(%0, call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%2, read<complex<f64>>(%20)), read<complex<f64>>(%21), real_to_complex<complex<f64>, reason=arg>(int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))), real_to_complex<complex<i32>, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @carg_test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %22: complex<f64> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %9 __retval: complex<f64> [storage=automatic];
// DEFAULT-NEXT:             write<f64>(real(%9), const<f64>(2.0));
// DEFAULT-NEXT:             write<f64>(imag(%9), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:             write<complex<f64>>(%22, read<complex<f64>>(%9));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64, i32) -> void>(%1, call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%3, read<complex<f64>>(%22)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
