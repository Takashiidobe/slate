/* The bit-field below would have a problem if __INT_MAX__ is too
   small.  */
void abort(void);
void exit(int);

#if __INT_MAX__ < 2147483647
int main(void) { exit(0); }
#else
/* Failed on powerpc due to bad extzvsi pattern.  */

struct ieee {
  unsigned int negative  : 1;
  unsigned int exponent  : 11;
  unsigned int mantissa0 : 20;
  unsigned int mantissa1 : 32;
} x;

unsigned int foo(void) {
  unsigned int exponent;

  exponent = x.exponent;
  if (exponent == 0)
    return 1;
  else if (exponent > 1)
    return 2;
  return 0;
}

int main(void) {
  x.exponent = 1;
  if (foo() != 0)
    abort();
  return 0;
}
#endif


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
// DEFAULT-NEXT:     type @type[[TYPE_ieee:[0-9]+]] ieee = struct {
// DEFAULT-NEXT:         field0 negative: u32 : 1;
// DEFAULT-NEXT:         field1 exponent: u32 : 11;
// DEFAULT-NEXT:         field2 mantissa0: u32 : 20;
// DEFAULT-NEXT:         field3 mantissa1: u32 : 32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0, 1, 4], bit_offsets=[Some(0), Some(1), Some(12), Some(32)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_ieee]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_exponent:[0-9]+]] exponent: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%[[VALUE_exponent]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..8, bits=1..12>(%[[VALUE_x]])))));
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE_exponent]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             return reinterpret<u32, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if gt<u32>(read<u32>(%[[VALUE_exponent]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:                 return reinterpret<u32, reason=return, fits=always>(const<i32>(2));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..8, bits=1..12>(%[[VALUE_x]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%[[VALUE_foo]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
