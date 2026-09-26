/* { dg-require-effective-target stdint_types } */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void f883b(int8_t *result, int16_t *__restrict arg1, uint32_t *__restrict arg2,
           uint64_t *__restrict arg3, uint8_t *__restrict arg4) {
  int idx;
  for (idx = 0; idx < 96; idx += 1) {
    result[idx] = (((((((((((-27 + 2 + 1) >> 1) || arg4[idx]) < arg1[idx])
                             ? (((-27 + 2 + 1) >> 1) || arg4[idx])
                             : arg1[idx]) >>
                        (arg2[idx] & 31)) ^
                       1) -
                      -32) >>
                     7) |
                    -5) &
                   arg3[idx]);
  }
}

int8_t   result[96];
int16_t  arg1[96];
uint32_t arg2[96];
uint64_t arg3[96];
uint8_t  arg4[96];

int main(void) {
  int i;
  int correct[] = {
      0x0,  0x1,  0x2,  0x3,  0x0,  0x1,  0x2,  0x3,  0x8,  0x9,  0xa,  0xb,
      0x8,  0x9,  0xa,  0xb,  0x10, 0x11, 0x12, 0x13, 0x10, 0x11, 0x12, 0x13,
      0x18, 0x19, 0x1a, 0x1b, 0x18, 0x19, 0x1a, 0x1b, 0x20, 0x21, 0x22, 0x23,
      0x20, 0x21, 0x22, 0x23, 0x28, 0x29, 0x2a, 0x2b, 0x28, 0x29, 0x2a, 0x2b,
      0x30, 0x31, 0x32, 0x33, 0x30, 0x31, 0x32, 0x33, 0x38, 0x39, 0x3a, 0x3b,
      0x38, 0x39, 0x3a, 0x3b, 0x40, 0x41, 0x42, 0x43, 0x40, 0x41, 0x42, 0x43,
      0x48, 0x49, 0x4a, 0x4b, 0x48, 0x49, 0x4a, 0x4b, 0x50, 0x51, 0x52, 0x53,
      0x50, 0x51, 0x52, 0x53, 0x58, 0x59, 0x5a, 0x5b, 0x58, 0x59, 0x5a, 0x5b};

  for (i = 0; i < 96; i++)
    arg3[i] = arg2[i] = arg1[i] = arg4[i] = i;

  f883b(result, arg1, arg2, arg3, arg4);

  for (i = 0; i < 96; i++)
    if (result[i] != correct[i])
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
// DEFAULT-NEXT:     type @type0 __int8_t = i8;
// DEFAULT-NEXT:     type @type1 __uint8_t = u8;
// DEFAULT-NEXT:     type @type2 __int16_t = i16;
// DEFAULT-NEXT:     type @type3 __uint32_t = u32;
// DEFAULT-NEXT:     type @type4 __uint64_t = u64;
// DEFAULT-NEXT:     type @type5 int8_t = i8;
// DEFAULT-NEXT:     type @type6 int16_t = i16;
// DEFAULT-NEXT:     type @type7 uint8_t = u8;
// DEFAULT-NEXT:     type @type8 uint32_t = u32;
// DEFAULT-NEXT:     type @type9 uint64_t = u64;
// DEFAULT-NEXT:     global %18 result: array<i8, 96> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %19 arg1: array<i16, 96> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %20 arg2: array<u32, 96> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %21 arg3: array<u64, 96> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %22 arg4: array<u8, 96> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %10 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @f883b(%12 result: ptr<i8>, %13 arg1: ptr<i16> [restrict], %14 arg2: ptr<u32> [restrict], %15 arg3: ptr<u64> [restrict], %16 arg4: ptr<u8> [restrict]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %17 idx: i32 [storage=automatic];
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%17, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%17), const<i32>(96))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%17);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%17, read<i32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%12), read<i32>(%17))), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(and<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(sub<i32, overflow=ub>(xor<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(conditional<i32>(lt<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(27)), const<i32>(2)), const<i32>(1)), const<i32>(1)), const<i32>(0)), ne<u8>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%16), read<i32>(%17)))), const<u8>(0)))), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%13), read<i32>(%17)))))), from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(add<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(27)), const<i32>(2)), const<i32>(1)), const<i32>(1)), const<i32>(0)), ne<u8>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%16), read<i32>(%17)))), const<u8>(0)))), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%13), read<i32>(%17)))))), and<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%14), read<i32>(%17)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(31)))), const<i32>(1)), neg<i32, overflow=ub>(const<i32>(32))), const<i32>(7)), neg<i32, overflow=ub>(const<i32>(5))))), read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%15), read<i32>(%17))))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %24 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %25 correct: array<i32, 96> [storage=automatic] [align=16] = aggregate<array<i32, 96>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1), index2 = const<i32>(2), index3 = const<i32>(3), index4 = const<i32>(0), index5 = const<i32>(1), index6 = const<i32>(2), index7 = const<i32>(3), index8 = const<i32>(8), index9 = const<i32>(9), index10 = const<i32>(10), index11 = const<i32>(11), index12 = const<i32>(8), index13 = const<i32>(9), index14 = const<i32>(10), index15 = const<i32>(11), index16 = const<i32>(16), index17 = const<i32>(17), index18 = const<i32>(18), index19 = const<i32>(19), index20 = const<i32>(16), index21 = const<i32>(17), index22 = const<i32>(18), index23 = const<i32>(19), index24 = const<i32>(24), index25 = const<i32>(25), index26 = const<i32>(26), index27 = const<i32>(27), index28 = const<i32>(24), index29 = const<i32>(25), index30 = const<i32>(26), index31 = const<i32>(27), index32 = const<i32>(32), index33 = const<i32>(33), index34 = const<i32>(34), index35 = const<i32>(35), index36 = const<i32>(32), index37 = const<i32>(33), index38 = const<i32>(34), index39 = const<i32>(35), index40 = const<i32>(40), index41 = const<i32>(41), index42 = const<i32>(42), index43 = const<i32>(43), index44 = const<i32>(40), index45 = const<i32>(41), index46 = const<i32>(42), index47 = const<i32>(43), index48 = const<i32>(48), index49 = const<i32>(49), index50 = const<i32>(50), index51 = const<i32>(51), index52 = const<i32>(48), index53 = const<i32>(49), index54 = const<i32>(50), index55 = const<i32>(51), index56 = const<i32>(56), index57 = const<i32>(57), index58 = const<i32>(58), index59 = const<i32>(59), index60 = const<i32>(56), index61 = const<i32>(57), index62 = const<i32>(58), index63 = const<i32>(59), index64 = const<i32>(64), index65 = const<i32>(65), index66 = const<i32>(66), index67 = const<i32>(67), index68 = const<i32>(64), index69 = const<i32>(65), index70 = const<i32>(66), index71 = const<i32>(67), index72 = const<i32>(72), index73 = const<i32>(73), index74 = const<i32>(74), index75 = const<i32>(75), index76 = const<i32>(72), index77 = const<i32>(73), index78 = const<i32>(74), index79 = const<i32>(75), index80 = const<i32>(80), index81 = const<i32>(81), index82 = const<i32>(82), index83 = const<i32>(83), index84 = const<i32>(80), index85 = const<i32>(81), index86 = const<i32>(82), index87 = const<i32>(83), index88 = const<i32>(88), index89 = const<i32>(89), index90 = const<i32>(90), index91 = const<i32>(91), index92 = const<i32>(88), index93 = const<i32>(89), index94 = const<i32>(90), index95 = const<i32>(91));
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%24, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%24), const<i32>(96))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%24, read<i32>(%32));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(96)>(%22), read<i32>(%24))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%24))));
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(96)>(%19), read<i32>(%24))), reinterpret<i16, reason=assign, fits=unknown>(widen<u16, reason=assign>(reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%24))))));
// DEFAULT-NEXT:                 write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(96)>(%20), read<i32>(%24))), reinterpret<u32, reason=assign, fits=unknown>(widen<i32, reason=assign>(reinterpret<i16, reason=assign, fits=unknown>(widen<u16, reason=assign>(reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%24))))))));
// DEFAULT-NEXT:                 write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(96)>(%21), read<i32>(%24))), widen<u64, reason=assign>(reinterpret<u32, reason=assign, fits=unknown>(widen<i32, reason=assign>(reinterpret<i16, reason=assign, fits=unknown>(widen<u16, reason=assign>(reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%24)))))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<i16>, ptr<u32>, ptr<u64>, ptr<u8>) -> void>(%11, array_decay<ptr<i8>, length=Some(96)>(%18), array_decay<ptr<i16>, length=Some(96)>(%19), array_decay<ptr<u32>, length=Some(96)>(%20), array_decay<ptr<u64>, length=Some(96)>(%21), array_decay<ptr<u8>, length=Some(96)>(%22));
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%24, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%24), const<i32>(96))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%24, read<i32>(%34));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(96)>(%18), read<i32>(%24))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(96)>(%25), read<i32>(%24)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
