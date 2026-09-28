/* PR target/60017 */

extern void abort(void);

struct S0 {
  short m0;
  short m1;
};

struct S1 {
  unsigned  m0 : 1;
  char      m1[2][2];
  struct S0 m2[2];
};

struct S1 x = {1, {{2, 3}, {4, 5}}, {{6, 7}, {8, 9}}};

struct S1 func(void) { return x; }

int main(void) {
  struct S1 ret = func();

  if (ret.m2[1].m1 != 9)
    abort();

  return 0;
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
// DEFAULT-NEXT:     type @type0 S0 = struct {
// DEFAULT-NEXT:         field0 m0: i16;
// DEFAULT-NEXT:         field1 m1: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type1 S1 = struct {
// DEFAULT-NEXT:         field0 m0: u32 : 1;
// DEFAULT-NEXT:         field1 m1: array<array<i8, 2>, 2>;
// DEFAULT-NEXT:         field2 m2: array<@type0, 2>;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 1, 6], bit_offsets=[Some(0), None, None], bit_units=[(0, 1)], field_units=[Some(0), None, None]];
// DEFAULT-NEXT:     global %3 x: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field1 = aggregate<array<array<i8, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(3))), index1 = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(5)))), field2 = aggregate<array<@type0, 2>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(6)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(7))), index1 = aggregate<@type0, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(8)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(9))))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @func() -> @type1 [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 ret: @type1 [storage=automatic] = copy<@type1, reason=assign>(call<@type1, signature=fn() -> @type1, abi=sysv64() -> native_c>(%4));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(field2(%6)), const<i32>(1)))))), const<i32>(9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
