/* PR rtl-optimization/84169 */

#ifdef __SIZEOF_INT128__
typedef unsigned __int128 T;
#else
typedef unsigned long long T;
#endif

T b;

static __attribute__((noipa)) T foo(T c, T d, T e, T f, T g, T h) {
  __builtin_mul_overflow((unsigned char)h, -16, &h);
  return b + h;
}

int main() {
  T x = foo(0, 0, 0, 0, 0, 4);
  if (x != -64)
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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = u128;
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_c:[0-9]+]] c: u128, %[[VALUE_d:[0-9]+]] d: u128, %[[VALUE_e:[0-9]+]] e: u128, %[[VALUE_f:[0-9]+]] f: u128, %[[VALUE_g:[0-9]+]] g: u128, %[[VALUE_h:[0-9]+]] h: u128) -> u128 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         overflow_mul<bool>(truncate<u8, reason=explicit, fits=unknown>(read<u128>(%[[VALUE_h]])), neg<i32, overflow=ub>(const<i32>(16)), deref(addr_of<ptr<u128>>(%[[VALUE_h]])));
// DEFAULT-NEXT:         return add<u128, overflow=wrap>(read<u128>(%[[VALUE_b]]), read<u128>(%[[VALUE_h]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: u128 [storage=automatic] = call<u128, signature=fn(u128, u128, u128, u128, u128, u128) -> u128>(%[[VALUE_foo]], reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(0))), reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(0))), reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(0))), reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(0))), reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(0))), reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         if ne<u128>(read<u128>(%[[VALUE_x]]), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(64)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
