#include <limits.h>
#include <stdio.h>

typedef unsigned __int128 U128;
typedef _BitInt(200) S200;
typedef unsigned _BitInt(200) U200;
typedef int v4si __attribute__((vector_size(16)));

int main(void) {
  int add_i = __builtin_elementwise_add_sat(INT_MAX - 5, 10);
  printf("%d\n", add_i);

  int add_i_no_sat = __builtin_elementwise_add_sat(2, 3);
  printf("%d\n", add_i_no_sat);

  short sub_s = __builtin_elementwise_sub_sat((short)(SHRT_MIN + 5), (short)10);
  printf("%d\n", sub_s);

  unsigned sub_u = __builtin_elementwise_sub_sat(5u, 10u);
  printf("%u\n", sub_u);

  U128 u128a    = (U128)0 - 1;
  U128 add_u128 = __builtin_elementwise_add_sat(u128a, (U128)5);
  printf("%d\n", add_u128 == (U128)0 - 1);

  S200 s200a    = -1;
  S200 add_s200 = __builtin_elementwise_add_sat(s200a, (S200)5);
  printf("%d\n", (int)add_s200);

  U200 u200a    = 3;
  U200 sub_u200 = __builtin_elementwise_sub_sat(u200a, (U200)10);
  printf("%d\n", sub_u200 == 0);

  v4si va = {INT_MAX, 1, INT_MIN, 0};
  v4si vb = {10, 1, -10, 0};
  v4si vr = __builtin_elementwise_add_sat(va, vb);
  printf("%d %d %d %d\n", vr[0], vr[1], vr[2], vr[3]);

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
// DEFAULT-NEXT:     type @type0 U128 = u128;
// DEFAULT-NEXT:     type @type1 S200 = i200b;
// DEFAULT-NEXT:     type @type2 U200 = u200b;
// DEFAULT-NEXT:     type @type3 v4si = vector<i32, 4>;
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%19 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 add_i: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(__builtin_elementwise_add_sat, sub<i32, overflow=ub>(const<i32>(2147483647), const<i32>(5)), const<i32>(10));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%20)), read<i32>(%6));
// DEFAULT-NEXT:         let %7 add_i_no_sat: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(__builtin_elementwise_add_sat, const<i32>(2), const<i32>(3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%21)), read<i32>(%7));
// DEFAULT-NEXT:         let %8 sub_s: i16 [storage=automatic] = call<i16, signature=fn(i16, i16) -> i16>(__builtin_elementwise_sub_sat, truncate<i16, reason=explicit, fits=unknown>(add<i32, overflow=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(32767)), const<i32>(1)), const<i32>(5))), truncate<i16, reason=explicit, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%22)), widen<i32, reason=vararg>(read<i16>(%8)));
// DEFAULT-NEXT:         let %9 sub_u: u32 [storage=automatic] = call<u32, signature=fn(u32, u32) -> u32>(__builtin_elementwise_sub_sat, const<u32>(5), const<u32>(10));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%23)), read<u32>(%9));
// DEFAULT-NEXT:         let %10 u128a: u128 [storage=automatic] = sub<u128, overflow=wrap>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %11 add_u128: u128 [storage=automatic] = call<u128, signature=fn(u128, u128) -> u128>(__builtin_elementwise_add_sat, read<u128>(%10), reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(5))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%24)), from_bool<i32, reason=vararg>(eq<u128>(read<u128>(%11), sub<u128, overflow=wrap>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         let %12 s200a: i200b [storage=automatic] = widen<i200b, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         let %13 add_s200: i200b [storage=automatic] = call<i200b, signature=fn(i200b, i200b) -> i200b>(__builtin_elementwise_add_sat, read<i200b>(%12), widen<i200b, reason=explicit>(const<i32>(5)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%25)), truncate<i32, reason=explicit, fits=unknown>(read<i200b>(%13)));
// DEFAULT-NEXT:         let %14 u200a: u200b [storage=automatic] = reinterpret<u200b, reason=assign, fits=unknown>(widen<i200b, reason=assign>(const<i32>(3)));
// DEFAULT-NEXT:         let %15 sub_u200: u200b [storage=automatic] = call<u200b, signature=fn(u200b, u200b) -> u200b>(__builtin_elementwise_sub_sat, read<u200b>(%14), reinterpret<u200b, reason=explicit, fits=unknown>(widen<i200b, reason=explicit>(const<i32>(10))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%26)), from_bool<i32, reason=vararg>(eq<u200b>(read<u200b>(%15), reinterpret<u200b, reason=usual_arith, fits=unknown>(widen<i200b, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         let %16 va: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(2147483647), index1 = const<i32>(1), index2 = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), index3 = const<i32>(0));
// DEFAULT-NEXT:         let %17 vb: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(1), index2 = neg<i32, overflow=ub>(const<i32>(10)), index3 = const<i32>(0));
// DEFAULT-NEXT:         let %18 vr: vector<i32, 4> [storage=automatic] = call<vector<i32, 4>, signature=fn(vector<i32, 4>, vector<i32, 4>) -> vector<i32, 4>, abi=sysv64(direct, direct) -> direct>(__builtin_elementwise_add_sat, read<vector<i32, 4>>(%16), read<vector<i32, 4>>(%17));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%27)), read<i32>(lane(%18, const<i32>(0))), read<i32>(lane(%18, const<i32>(1))), read<i32>(lane(%18, const<i32>(2))), read<i32>(lane(%18, const<i32>(3))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
