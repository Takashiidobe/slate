/* PR tree-optimization/71631 */

volatile char v;
int           a = 1, b = 1, c = 1;

void foo(const char *s) {
  while (*s++)
    v = *s;
}

int main() {
  volatile int d = 1;
  volatile int e = 1;
  int          f = 1 / a;
  int          g = 1U < f;
  int          h = 2 + g;
  int          i = 3 % h;
  int          j = e && b;
  int          k = 1 == c;
  int          l = d != 0;
  short        m = (short)(-1 * i * l);
  short        x = j * (k * m);
  if (i == 1)
    foo("AB");
  if (x != -1)
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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: volatile i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([65, 66, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_s:[0-9]+]] s: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_s]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<const i8>>(%[[VALUE_s]], read<ptr<const i8>>(%[[VALUE2]]));
// DEFAULT-NEXT:             yield ne<i8>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE1]]))), const<i8>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<i8, volatile>(%[[VALUE_v]], read<i8>(deref(read<ptr<const i8>>(%[[VALUE_s]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: volatile i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: volatile i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic] = div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: i32 [storage=automatic] = from_bool<i32, reason=assign>(lt<u32>(const<u32>(1), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE_f]]))));
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: i32 [storage=automatic] = add<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_g]]));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(3), read<i32>(%[[VALUE_h]]));
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(read<i32, volatile>(%[[VALUE_e]]), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(const<i32>(1), read<i32>(%[[VALUE_c]])));
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i32 [storage=automatic] = from_bool<i32, reason=assign>(ne<i32>(read<i32, volatile>(%[[VALUE_d]]), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: i16 [storage=automatic] = truncate<i16, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(mul<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), read<i32>(%[[VALUE_i]])), read<i32>(%[[VALUE_l]])));
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i16 [storage=automatic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_j]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_k]]), widen<i32, reason=promotion>(read<i16>(%[[VALUE_m]])))));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<const i8>) -> void>(%[[VALUE_foo]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_x]])), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
