/* PR rtl-optimization/58726 */

int a, c;
union {
  int f1;
  int f2 : 1;
} b;

short foo(short p) { return p < 0 ? p : a; }

int main() {
  if (sizeof(short) * __CHAR_BIT__ != 16 || sizeof(int) * __CHAR_BIT__ != 32)
    return 0;
  b.f1 = 56374;
  unsigned short d;
  int            e = b.f2;
  d                = e == 0 ? b.f1 : 0;
  c                = foo(d);
  if (c != (short)56374)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 f1: i32;
// DEFAULT-NEXT:         field1 f2: i32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[None, Some(0)], bit_units=[(0, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(conditional<i32>(lt<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_p]])), const<i32>(0)), widen<i32, reason=promotion>(read<i16>(%[[VALUE_p]])), read<i32>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16)))), ne<u64>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32)))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_b]]), const<i32>(56374));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: u16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic] = read<i32>(bitfield1<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_d]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(conditional<i32>(eq<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0)), read<i32>(field0(%[[VALUE_b]])), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], widen<i32, reason=assign>(call<i16, signature=fn(i16) -> i16>(%[[VALUE_foo]], reinterpret<i16, reason=arg, fits=unknown>(read<u16>(%[[VALUE_d]])))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_c]]), widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(56374))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
