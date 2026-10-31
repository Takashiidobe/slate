// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef unsigned long uLong;
typedef long offset;

struct S {
  uLong a;
  unsigned long b;
  uLong *c;
};

uLong global = 3;

int compared(uLong a, uLong b) { return a < b; }

uLong plain(unsigned long a) { return a + 1; }

uLong named(uLong a, offset n, struct S *s, _Complex double *z) {
  uLong sum = a % 65521;
  unsigned rem = (unsigned)(n % 65521);
  sum = (uLong)rem * sum + 1UL;
  static uLong total;
  total += sum;
  uLong old = total++;
  s->a = old;
  s->b = sum;
  *s->c = sum;
  uLong *p = &sum;
  p[1] = *p;
  __real__ *z = 1.0;
  return sum;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_uLong:[0-9]+]] uLong = u64;
// IR-NEXT:     type @type[[TYPE_offset:[0-9]+]] offset = i64;
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 a: u64 [c="uLong"];
// IR-NEXT:         field1 b: u64;
// IR-NEXT:         field2 c: ptr<u64> [c="uLong *"];
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     global %[[VALUE_global:[0-9]+]] global: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))) [c="uLong"] [linkage=external] [c="uLong"];
// IR-NEXT:     global %[[VALUE_total:[0-9]+]] total: u64 [storage=static] [linkage=internal] [c="uLong"];
// IR-NEXT:     fn %[[VALUE_compared:[0-9]+]] @compared(%[[VALUE_a:[0-9]+]] a: u64 [c="uLong"], %[[VALUE_b:[0-9]+]] b: u64 [c="uLong"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c="int(uLong, uLong)"] {
// IR-NEXT:         return from_bool<i32, reason=return>(lt<u64>(read<u64>(%[[VALUE_a]]) [c="uLong"], read<u64>(%[[VALUE_b]]) [c="uLong"]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_a_2:[0-9]+]] a: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] [c="uLong(unsigned long)"] {
// IR-NEXT:         return add<u64, overflow=wrap>(read<u64>(%[[VALUE_a_2]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))) [c="uLong"];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_named:[0-9]+]] @named(%[[VALUE_a_3:[0-9]+]] a: u64 [c="uLong"], %[[VALUE_n:[0-9]+]] n: i64 [c="offset"], %[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_S]]>, %[[VALUE_z:[0-9]+]] z: ptr<complex<f64>>) -> u64 [linkage=external] [fallthrough=ub_if_used] [c="uLong(uLong, offset, struct S *, _Complex double *)"] {
// IR-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: u64 [storage=automatic] = rem<u64, by_zero=ub>(read<u64>(%[[VALUE_a_3]]) [c="uLong"], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(65521))) [c="uLong"]) [c="uLong"] [c="uLong"];
// IR-NEXT:         let %[[VALUE_rem:[0-9]+]] rem: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(truncate<i32, reason=explicit, fits=unknown>(rem<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_n]]) [c="offset"], widen<i64, reason=usual_arith>(const<i32>(65521)) [c="offset"]) [c="offset"]));
// IR-NEXT:         write<u64>(%[[VALUE_sum]], add<u64, overflow=wrap>(mul<u64, overflow=wrap>(widen<u64, reason=explicit>(read<u32>(%[[VALUE_rem]])) [c="uLong"], read<u64>(%[[VALUE_sum]]) [c="uLong"]) [c="uLong"], const<u64>(1) [c="uLong"]) [c="uLong"]) [c="uLong"];
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total]]) [c="uLong"] [c="uLong"];
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE0]]) [c="uLong"], read<u64>(%[[VALUE_sum]]) [c="uLong"]) [c="uLong"] [c="uLong"];
// IR-NEXT:         write<u64>(%[[VALUE_total]], read<u64>(%[[VALUE1]]) [c="uLong"]) [c="uLong"] [c="uLong"];
// IR-NEXT:         let %[[VALUE_old:[0-9]+]] old: u64 [storage=automatic] [c="uLong"];
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total]]) [c="uLong"] [c="uLong"];
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE2]]) [c="uLong"], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))) [c="uLong"]) [c="uLong"] [c="uLong"];
// IR-NEXT:         write<u64>(%[[VALUE_total]], read<u64>(%[[VALUE3]]) [c="uLong"]) [c="uLong"] [c="uLong"];
// IR-NEXT:         write<u64>(%[[VALUE_old]], read<u64>(%[[VALUE2]]) [c="uLong"]) [c="uLong"];
// IR-NEXT:         write<u64>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]))), read<u64>(%[[VALUE_old]]) [c="uLong"]) [c="uLong"];
// IR-NEXT:         write<u64>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]))), read<u64>(%[[VALUE_sum]]));
// IR-NEXT:         write<u64>(deref(read<ptr<u64>>(field2(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]))))), read<u64>(%[[VALUE_sum]]) [c="uLong"]) [c="uLong"];
// IR-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<u64> [storage=automatic] = addr_of<ptr<u64>>(%[[VALUE_sum]]) [c="uLong *"] [c="uLong *"];
// IR-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%[[VALUE_p]]), const<i32>(1))), read<u64>(deref(read<ptr<u64>>(%[[VALUE_p]]))) [c="uLong"]) [c="uLong"];
// IR-NEXT:         write<f64>(real(deref(read<ptr<complex<f64>>>(%[[VALUE_z]]))), const<f64>(1.0));
// IR-NEXT:         return read<u64>(%[[VALUE_sum]]) [c="uLong"];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
