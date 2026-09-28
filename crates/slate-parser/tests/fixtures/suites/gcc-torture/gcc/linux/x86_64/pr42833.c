typedef __INT_LEAST8_TYPE__   int8_t;
typedef __UINT_LEAST32_TYPE__ uint32_t;
typedef int                   ssize_t;
typedef struct {
  int8_t v1;
  int8_t v2;
  int8_t v3;
  int8_t v4;
} neon_s8;

uint32_t helper_neon_rshl_s8(uint32_t arg1, uint32_t arg2);

uint32_t helper_neon_rshl_s8(uint32_t arg1, uint32_t arg2) {
  uint32_t res;
  neon_s8  vsrc1;
  neon_s8  vsrc2;
  neon_s8  vdest;
  do {
    union {
      neon_s8  v;
      uint32_t i;
    } conv_u;
    conv_u.i = (arg1);
    vsrc1    = conv_u.v;
  } while (0);
  do {
    union {
      neon_s8  v;
      uint32_t i;
    } conv_u;
    conv_u.i = (arg2);
    vsrc2    = conv_u.v;
  } while (0);
  do {
    int8_t tmp;
    tmp = (int8_t)vsrc2.v1;
    if (tmp >= (ssize_t)sizeof(vsrc1.v1) * 8) {
      vdest.v1 = 0;
    } else if (tmp < -(ssize_t)sizeof(vsrc1.v1) * 8) {
      vdest.v1 = vsrc1.v1 >> (sizeof(vsrc1.v1) * 8 - 1);
    } else if (tmp == -(ssize_t)sizeof(vsrc1.v1) * 8) {
      vdest.v1 = vsrc1.v1 >> (tmp - 1);
      vdest.v1++;
      vdest.v1 >>= 1;
    } else if (tmp < 0) {
      vdest.v1 = (vsrc1.v1 + (1 << (-1 - tmp))) >> -tmp;
    } else {
      vdest.v1 = vsrc1.v1 << tmp;
    }
  } while (0);
  do {
    int8_t tmp;
    tmp = (int8_t)vsrc2.v2;
    if (tmp >= (ssize_t)sizeof(vsrc1.v2) * 8) {
      vdest.v2 = 0;
    } else if (tmp < -(ssize_t)sizeof(vsrc1.v2) * 8) {
      vdest.v2 = vsrc1.v2 >> (sizeof(vsrc1.v2) * 8 - 1);
    } else if (tmp == -(ssize_t)sizeof(vsrc1.v2) * 8) {
      vdest.v2 = vsrc1.v2 >> (tmp - 1);
      vdest.v2++;
      vdest.v2 >>= 1;
    } else if (tmp < 0) {
      vdest.v2 = (vsrc1.v2 + (1 << (-1 - tmp))) >> -tmp;
    } else {
      vdest.v2 = vsrc1.v2 << tmp;
    }
  } while (0);
  do {
    int8_t tmp;
    tmp = (int8_t)vsrc2.v3;
    if (tmp >= (ssize_t)sizeof(vsrc1.v3) * 8) {
      vdest.v3 = 0;
    } else if (tmp < -(ssize_t)sizeof(vsrc1.v3) * 8) {
      vdest.v3 = vsrc1.v3 >> (sizeof(vsrc1.v3) * 8 - 1);
    } else if (tmp == -(ssize_t)sizeof(vsrc1.v3) * 8) {
      vdest.v3 = vsrc1.v3 >> (tmp - 1);
      vdest.v3++;
      vdest.v3 >>= 1;
    } else if (tmp < 0) {
      vdest.v3 = (vsrc1.v3 + (1 << (-1 - tmp))) >> -tmp;
    } else {
      vdest.v3 = vsrc1.v3 << tmp;
    }
  } while (0);
  do {
    int8_t tmp;
    tmp = (int8_t)vsrc2.v4;
    if (tmp >= (ssize_t)sizeof(vsrc1.v4) * 8) {
      vdest.v4 = 0;
    } else if (tmp < -(ssize_t)sizeof(vsrc1.v4) * 8) {
      vdest.v4 = vsrc1.v4 >> (sizeof(vsrc1.v4) * 8 - 1);
    } else if (tmp == -(ssize_t)sizeof(vsrc1.v4) * 8) {
      vdest.v4 = vsrc1.v4 >> (tmp - 1);
      vdest.v4++;
      vdest.v4 >>= 1;
    } else if (tmp < 0) {
      vdest.v4 = (vsrc1.v4 + (1 << (-1 - tmp))) >> -tmp;
    } else {
      vdest.v4 = vsrc1.v4 << tmp;
    }
  } while (0);
  ;
  do {
    union {
      neon_s8  v;
      uint32_t i;
    } conv_u;
    conv_u.v = (vdest);
    res      = conv_u.i;
  } while (0);
  return res;
}

extern void abort(void);

int main() {
  uint32_t r = helper_neon_rshl_s8(0x05050505, 0x01010101);
  if (r != 0x0a0a0a0a)
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
// DEFAULT-NEXT:     type @type0 int8_t = i8;
// DEFAULT-NEXT:     type @type1 uint32_t = u32;
// DEFAULT-NEXT:     type @type2 ssize_t = i32;
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 v1: i8;
// DEFAULT-NEXT:         field1 v2: i8;
// DEFAULT-NEXT:         field2 v3: i8;
// DEFAULT-NEXT:         field3 v4: i8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     type @type4 neon_s8 = @type3;
// DEFAULT-NEXT:     type @type5 = union {
// DEFAULT-NEXT:         field0 v: @type3;
// DEFAULT-NEXT:         field1 i: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type6 = union {
// DEFAULT-NEXT:         field0 v: @type3;
// DEFAULT-NEXT:         field1 i: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type7 = union {
// DEFAULT-NEXT:         field0 v: @type3;
// DEFAULT-NEXT:         field1 i: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %7 @helper_neon_rshl_s8(%8 arg1: u32, %9 arg2: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 res: u32 [storage=automatic];
// DEFAULT-NEXT:         let %11 vsrc1: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %12 vsrc2: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %13 vdest: @type3 [storage=automatic];
// DEFAULT-NEXT:         do %29
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %15 conv_u: @type5 [storage=automatic];
// DEFAULT-NEXT:                 write<u32>(field1(%15), read<u32>(%8));
// DEFAULT-NEXT:                 write<@type3>(%11, copy<@type3, reason=assign>(read<@type3>(field0(%15))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %30
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %17 conv_u: @type6 [storage=automatic];
// DEFAULT-NEXT:                 write<u32>(field1(%17), read<u32>(%9));
// DEFAULT-NEXT:                 write<@type3>(%12, copy<@type3, reason=assign>(read<@type3>(field0(%17))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %31
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %18 tmp: i8 [storage=automatic];
// DEFAULT-NEXT:                 write<i8>(%18, read<i8>(field0(%12)));
// DEFAULT-NEXT:                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%18)), mul<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), const<i32>(8)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(field0(%13), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i8>(%18)), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i8>(field0(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field0(%11))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%18)), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i8>(field0(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field0(%11))), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%18)), const<i32>(1)))));
// DEFAULT-NEXT:                                 let %36: i8 [synthetic] = read<i8>(field0(%13));
// DEFAULT-NEXT:                                 let %37: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%36)), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field0(%13), read<i8>(%37));
// DEFAULT-NEXT:                                 let %38: i8 [synthetic] = read<i8>(field0(%13));
// DEFAULT-NEXT:                                 let %39: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%38)), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field0(%13), read<i8>(%39));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(widen<i32, reason=promotion>(read<i8>(%18)), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field0(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field0(%11))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%18))))), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%18))))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field0(%13), truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(field0(%11))), widen<i32, reason=promotion>(read<i8>(%18)))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %32
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %19 tmp: i8 [storage=automatic];
// DEFAULT-NEXT:                 write<i8>(%19, read<i8>(field1(%12)));
// DEFAULT-NEXT:                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%19)), mul<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), const<i32>(8)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(field1(%13), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i8>(%19)), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i8>(field1(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field1(%11))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%19)), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i8>(field1(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field1(%11))), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%19)), const<i32>(1)))));
// DEFAULT-NEXT:                                 let %40: i8 [synthetic] = read<i8>(field1(%13));
// DEFAULT-NEXT:                                 let %41: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%40)), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field1(%13), read<i8>(%41));
// DEFAULT-NEXT:                                 let %42: i8 [synthetic] = read<i8>(field1(%13));
// DEFAULT-NEXT:                                 let %43: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%42)), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field1(%13), read<i8>(%43));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(widen<i32, reason=promotion>(read<i8>(%19)), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field1(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field1(%11))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%19))))), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%19))))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field1(%13), truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(field1(%11))), widen<i32, reason=promotion>(read<i8>(%19)))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %33
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %20 tmp: i8 [storage=automatic];
// DEFAULT-NEXT:                 write<i8>(%20, read<i8>(field2(%12)));
// DEFAULT-NEXT:                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%20)), mul<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), const<i32>(8)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(field2(%13), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i8>(%20)), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i8>(field2(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field2(%11))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%20)), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i8>(field2(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field2(%11))), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%20)), const<i32>(1)))));
// DEFAULT-NEXT:                                 let %44: i8 [synthetic] = read<i8>(field2(%13));
// DEFAULT-NEXT:                                 let %45: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%44)), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field2(%13), read<i8>(%45));
// DEFAULT-NEXT:                                 let %46: i8 [synthetic] = read<i8>(field2(%13));
// DEFAULT-NEXT:                                 let %47: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%46)), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field2(%13), read<i8>(%47));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(widen<i32, reason=promotion>(read<i8>(%20)), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field2(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field2(%11))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%20))))), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%20))))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field2(%13), truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(field2(%11))), widen<i32, reason=promotion>(read<i8>(%20)))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %34
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 tmp: i8 [storage=automatic];
// DEFAULT-NEXT:                 write<i8>(%21, read<i8>(field3(%12)));
// DEFAULT-NEXT:                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%21)), mul<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), const<i32>(8)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(field3(%13), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i8>(%21)), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i8>(field3(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field3(%11))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%21)), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i8>(field3(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field3(%11))), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%21)), const<i32>(1)))));
// DEFAULT-NEXT:                                 let %48: i8 [synthetic] = read<i8>(field3(%13));
// DEFAULT-NEXT:                                 let %49: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%48)), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field3(%13), read<i8>(%49));
// DEFAULT-NEXT:                                 let %50: i8 [synthetic] = read<i8>(field3(%13));
// DEFAULT-NEXT:                                 let %51: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%50)), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field3(%13), read<i8>(%51));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(widen<i32, reason=promotion>(read<i8>(%21)), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field3(%13), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field3(%11))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%21))))), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%21))))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field3(%13), truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(field3(%11))), widen<i32, reason=promotion>(read<i8>(%21)))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         do %35
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %23 conv_u: @type7 [storage=automatic];
// DEFAULT-NEXT:                 write<@type3>(field0(%23), copy<@type3, reason=assign>(read<@type3>(%13)));
// DEFAULT-NEXT:                 write<u32>(%10, read<u32>(field1(%23)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return read<u32>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %26 r: u32 [storage=automatic] = call<u32, signature=fn(u32, u32) -> u32>(%7, reinterpret<u32, reason=arg, fits=always>(const<i32>(84215045)), reinterpret<u32, reason=arg, fits=always>(const<i32>(16843009)));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%26), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(168430090)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%24);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
