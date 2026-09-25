void abort(void);
void exit(int);

int test_endianness() {
  union doubleword {
    double        d;
    unsigned long u[2];
  } dw;
  dw.d = 10;
  return dw.u[0] != 0 ? 1 : 0;
}

int test_endianness_vol() {
  union doubleword {
    volatile double d;
    volatile long   u[2];
  } dw;
  dw.d = 10;
  return dw.u[0] != 0 ? 1 : 0;
}

int main(void) {
  if (test_endianness() != test_endianness_vol())
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
// DEFAULT-NEXT:     type @type0 doubleword = union {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 u: array<u64, 2>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type1 doubleword = union {
// DEFAULT-NEXT:         field0 d: volatile f64;
// DEFAULT-NEXT:         field1 u: volatile array<i64, 2>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @test_endianness() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 dw: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(field0(%4), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(10)));
// DEFAULT-NEXT:         return conditional<i32>(ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(field1(%4)), const<i32>(0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test_endianness_vol() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 dw: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<f64, volatile>(field0(%7), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(10)));
// DEFAULT-NEXT:         return conditional<i32>(ne<i64>(read<i64, volatile>(deref(ptr_offset<ptr<volatile i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<volatile i64>, length=Some(2)>(field1(%7)), const<i32>(0)))), widen<i64, reason=usual_arith>(const<i32>(0))), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%2), call<i32, signature=fn() -> i32>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
