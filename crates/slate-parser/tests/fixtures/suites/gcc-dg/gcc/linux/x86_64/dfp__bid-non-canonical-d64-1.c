/* Test non-canonical BID significands: _Decimal64.  Bug 91226.  */
/* { dg-require-effective-target dfp_bid } */
/* { dg-options "-std=gnu23 -O2" } */

extern void abort (void);
extern void exit (int);

union u
{
  _Decimal64 d64;
  unsigned long long int u64;
};

int
main (void)
{
  union u x;
  _Decimal64 d64;
  x.u64 = 0x6c7386f26fc10001ULL;
  d64 = x.d64;
  volatile double d = d64;
  if (d == 0)
    exit (0);
  else
    abort ();
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
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 d64: d64;
// DEFAULT-NEXT:         field1 u64: u64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: @type[[TYPE_u]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d64:[0-9]+]] d64: d64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(field1(%[[VALUE_x]]), const<u64>(7814738154233069569));
// DEFAULT-NEXT:         write<d64>(%[[VALUE_d64]], read<d64>(field0(%[[VALUE_x]])));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: volatile f64 [storage=automatic] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(read<d64>(%[[VALUE_d64]]));
// DEFAULT-NEXT:         if eq<f64, exceptions=observable>(read<f64, volatile>(%[[VALUE_d]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
