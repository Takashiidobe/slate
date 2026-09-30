/* PR tree-optimization/56051 */

extern void abort(void);

int main() {
  unsigned char x1[1] = {0};
  unsigned int  s1    = __CHAR_BIT__;
  int           a1    = x1[0] < (unsigned char)(1 << s1);
  unsigned char y1    = (unsigned char)(1 << s1);
  int           b1    = x1[0] < y1;
  if (a1 != b1)
    abort();
#if __SIZEOF_LONG_LONG__ > __SIZEOF_INT__
  unsigned long long x2[1] = {2ULL << (sizeof(int) * __CHAR_BIT__)};
  unsigned int       s2    = sizeof(int) * __CHAR_BIT__ - 1;
  int                a2    = x2[0] >= (unsigned long long)(1 << s2);
  unsigned long long y2    = 1 << s2;
  int                b2    = x2[0] >= y2;
  if (a2 != b2)
    abort();
  unsigned long long x3[1] = {2ULL << (sizeof(int) * __CHAR_BIT__)};
  unsigned int       s3    = sizeof(int) * __CHAR_BIT__ - 1;
  int                a3    = x3[0] >= (unsigned long long)(1U << s3);
  unsigned long long y3    = 1U << s3;
  int                b3    = x3[0] >= y3;
  if (a3 != b3)
    abort();
#endif
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x1:[0-9]+]] x1: array<u8, 1> [storage=automatic] = aggregate<array<u8, 1>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_s1:[0-9]+]] s1: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(8));
// DEFAULT-NEXT:         let %[[VALUE_a1:[0-9]+]] a1: i32 [storage=automatic] = from_bool<i32, reason=assign>(lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1)>(%[[VALUE_x1]]), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u32>(%[[VALUE_s1]]))))))));
// DEFAULT-NEXT:         let %[[VALUE_y1:[0-9]+]] y1: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u32>(%[[VALUE_s1]]))));
// DEFAULT-NEXT:         let %[[VALUE_b1:[0-9]+]] b1: i32 [storage=automatic] = from_bool<i32, reason=assign>(lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1)>(%[[VALUE_x1]]), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_y1]])))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a1]]), read<i32>(%[[VALUE_b1]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_x2:[0-9]+]] x2: array<u64, 1> [storage=automatic] = aggregate<array<u64, 1>, zero_fill=false>(index0 = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(2), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         let %[[VALUE_s2:[0-9]+]] s2: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE_a2:[0-9]+]] a2: i32 [storage=automatic] = from_bool<i32, reason=assign>(ge<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1)>(%[[VALUE_x2]]), const<i32>(0)))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u32>(%[[VALUE_s2]]))))));
// DEFAULT-NEXT:         let %[[VALUE_y2:[0-9]+]] y2: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<u32>(%[[VALUE_s2]]))));
// DEFAULT-NEXT:         let %[[VALUE_b2:[0-9]+]] b2: i32 [storage=automatic] = from_bool<i32, reason=assign>(ge<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1)>(%[[VALUE_x2]]), const<i32>(0)))), read<u64>(%[[VALUE_y2]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a2]]), read<i32>(%[[VALUE_b2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_x3:[0-9]+]] x3: array<u64, 1> [storage=automatic] = aggregate<array<u64, 1>, zero_fill=false>(index0 = shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(2), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         let %[[VALUE_s3:[0-9]+]] s3: u32 [storage=automatic] = truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE_a3:[0-9]+]] a3: i32 [storage=automatic] = from_bool<i32, reason=assign>(ge<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1)>(%[[VALUE_x3]]), const<i32>(0)))), widen<u64, reason=explicit>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), read<u32>(%[[VALUE_s3]])))));
// DEFAULT-NEXT:         let %[[VALUE_y3:[0-9]+]] y3: u64 [storage=automatic] = widen<u64, reason=assign>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), read<u32>(%[[VALUE_s3]])));
// DEFAULT-NEXT:         let %[[VALUE_b3:[0-9]+]] b3: i32 [storage=automatic] = from_bool<i32, reason=assign>(ge<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(1)>(%[[VALUE_x3]]), const<i32>(0)))), read<u64>(%[[VALUE_y3]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a3]]), read<i32>(%[[VALUE_b3]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
