/* AArch64 wrong code at -O3.  An oversized vector
   (V16DI, 128 bytes, no register mode) is expanded into two distinct
   stack slots that shared the same MEM_EXPR.  The load/store pair fusion
   pass then fused stores belonging to different slots, leaving part of a
   slot uninitialized.  Self-checking: the checksum is target-independent.  */

#include <stdint.h>

#define BS_VEC(type, num) type __attribute__((vector_size(num * sizeof(type))))
uint64_t BS_CHECKSUM, g_284;
struct U0 {
  int16_t f0;
  int64_t f2;
} g_205;
int16_t            g_8, func_2_BS_COND_1;
uint8_t            g_9[2][1];
volatile struct U0 g_121[1];
int64_t           *g_565 = &g_284;

int main(void) {
  BS_VEC(int64_t, 16) BS_VAR_3 = {1, 8096386231136, 9039249955151};
  uint64_t LOCAL_CHECKSUM      = 0;
  switch (func_2_BS_COND_1) {
  case 2:
    goto BS_LABEL_0;
  case 4:
    goto BS_LABEL_0;
  }
  for (g_8 = 0; g_8 <= 0; g_8 += 1)
    for (g_205.f2 = 0; g_205.f2 <= 1; g_205.f2 += 1) {
      if (g_9[g_205.f2][0])
      BS_LABEL_0:
        for (;;)
          ;
      for (uint32_t BS_TEMP_371 = 0; BS_TEMP_371 < 16; BS_TEMP_371++)
        LOCAL_CHECKSUM ^= BS_VAR_3[BS_TEMP_371] + 9 + (LOCAL_CHECKSUM << 6) +
                              LOCAL_CHECKSUM >>
                          (*g_565 |= g_121[0].f0);
      BS_VAR_3 = (BS_VEC(int64_t, 16)){};
    }
  BS_CHECKSUM = LOCAL_CHECKSUM;
  if (BS_CHECKSUM != 0x7237237237237237ULL)
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
// DEFAULT-NEXT:     type @type0 __uint8_t = u8;
// DEFAULT-NEXT:     type @type1 __int16_t = i16;
// DEFAULT-NEXT:     type @type2 __uint32_t = u32;
// DEFAULT-NEXT:     type @type3 __int64_t = i64;
// DEFAULT-NEXT:     type @type4 __uint64_t = u64;
// DEFAULT-NEXT:     type @type5 int16_t = i16;
// DEFAULT-NEXT:     type @type6 int64_t = i64;
// DEFAULT-NEXT:     type @type7 uint8_t = u8;
// DEFAULT-NEXT:     type @type8 uint32_t = u32;
// DEFAULT-NEXT:     type @type9 uint64_t = u64;
// DEFAULT-NEXT:     type @type10 U0 = struct {
// DEFAULT-NEXT:         field0 f0: i16;
// DEFAULT-NEXT:         field1 f2: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %10 BS_CHECKSUM: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 g_284: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 g_205: @type10 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 g_8: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 func_2_BS_COND_1: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 g_9: array<array<u8, 1>, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 g_121: volatile array<@type10, 1> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %18 g_565: ptr<i64> [storage=static] = pointer_cast<ptr<i64>, reason=assign>(addr_of<ptr<u64>>(%11)) [linkage=external];
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %21 BS_VAR_3: vector<i64, 16> [storage=automatic] = aggregate<vector<i64, 16>, zero_fill=true>(index0 = widen<i64, reason=assign>(const<i32>(1)), index1 = const<i64>(8096386231136), index2 = const<i64>(9039249955151));
// DEFAULT-NEXT:         let %22 LOCAL_CHECKSUM: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         switch %24 widen<i32, reason=promotion>(read<i16>(%15))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %24 const<i32>(2):
// DEFAULT-NEXT:                     goto %20;
// DEFAULT-NEXT:                 case %24 const<i32>(4):
// DEFAULT-NEXT:                     goto %20;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         for %25
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i16>(%14, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: le<i32>(widen<i32, reason=promotion>(read<i16>(%14)), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %30: i16 [synthetic] = read<i16>(%14);
// DEFAULT-NEXT:                 let %31: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%30)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i16>(%14, read<i16>(%31));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %26
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i64>(field1(%13), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:                     condition: le<i64>(read<i64>(field1(%13)), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %32: i64 [synthetic] = read<i64>(field1(%13));
// DEFAULT-NEXT:                         let %33: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%32), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                         write<i64>(field1(%13), read<i64>(%33));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if ne<u8>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1)>(deref(ptr_offset<ptr<array<u8, 1>>, subtract=false, element=array<u8, 1>, overflow=ub>(array_decay<ptr<array<u8, 1>>, length=Some(2)>(%16), read<i64>(field1(%13))))), const<i32>(0)))), const<u8>(0))
// DEFAULT-NEXT:                                 label %20 BS_LABEL_0:
// DEFAULT-NEXT:                                     for %27
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                         condition: omitted
// DEFAULT-NEXT:                                         increment: omitted
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             ;
// DEFAULT-NEXT:                             for %28
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     let %23 BS_TEMP_371: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<u32>(read<u32>(%23), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16)))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %34: u32 [synthetic] = read<u32>(%23);
// DEFAULT-NEXT:                                     let %35: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%34), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                                     write<u32>(%23, read<u32>(%35));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     let %36: u64 [synthetic] = read<u64>(%22);
// DEFAULT-NEXT:                                     let %37: ptr<i64> [synthetic] = read<ptr<i64>>(%18);
// DEFAULT-NEXT:                                     let %38: i64 [synthetic] = read<i64>(deref(read<ptr<i64>>(%37)));
// DEFAULT-NEXT:                                     let %39: i64 [synthetic] = or<i64>(read<i64>(%38), widen<i64, reason=usual_arith>(widen<i32, reason=promotion>(read<i16, volatile>(field0(deref(ptr_offset<ptr<volatile @type10>, subtract=false, element=@type10, overflow=ub>(array_decay<ptr<volatile @type10>, length=Some(1)>(%17), const<i32>(0))))))));
// DEFAULT-NEXT:                                     write<i64>(deref(read<ptr<i64>>(%37)), read<i64>(%39));
// DEFAULT-NEXT:                                     let %40: u64 [synthetic] = xor<u64>(read<u64>(%36), shr<u64, amount_out_of_range=ub, fill=zero_extend>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(add<i64, overflow=ub>(read<i64>(lane(%21, read<u32>(%23))), widen<i64, reason=usual_arith>(const<i32>(9)))), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%22), const<i32>(6))), read<u64>(%22)), read<i64>(%39)));
// DEFAULT-NEXT:                                     write<u64>(%22, read<u64>(%40));
// DEFAULT-NEXT:                             write<vector<i64, 16>>(%21, read<vector<i64, 16>>(compound_literal %29 [storage=automatic] = aggregate<vector<i64, 16>, zero_fill=true>()));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:         write<u64>(%10, read<u64>(%22));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%10), const<u64>(8230085817501184567))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
