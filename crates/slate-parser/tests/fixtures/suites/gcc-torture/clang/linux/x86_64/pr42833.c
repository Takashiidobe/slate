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
// DEFAULT-NEXT:     type @type[[TYPE_int8_t:[0-9]+]] int8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE_uint32_t:[0-9]+]] uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_ssize_t:[0-9]+]] ssize_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 v1: i8;
// DEFAULT-NEXT:         field1 v2: i8;
// DEFAULT-NEXT:         field2 v3: i8;
// DEFAULT-NEXT:         field3 v4: i8;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 1, 2, 3]];
// DEFAULT-NEXT:     type @type[[TYPE_neon_s8:[0-9]+]] neon_s8 = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 v: @type[[TYPE0]];
// DEFAULT-NEXT:         field1 i: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 v: @type[[TYPE0]];
// DEFAULT-NEXT:         field1 i: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 v: @type[[TYPE0]];
// DEFAULT-NEXT:         field1 i: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_helper_neon_rshl_s8:[0-9]+]] @helper_neon_rshl_s8(%[[VALUE_arg1:[0-9]+]] arg1: u32, %[[VALUE_arg2:[0-9]+]] arg2: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res:[0-9]+]] res: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_vsrc1:[0-9]+]] vsrc1: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_vsrc2:[0-9]+]] vsrc2: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_vdest:[0-9]+]] vdest: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_conv_u:[0-9]+]] conv_u: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:                 write<u32>(field1(%[[VALUE_conv_u]]), read<u32>(%[[VALUE_arg1]]));
// DEFAULT-NEXT:                 write<@type[[TYPE0]]>(%[[VALUE_vsrc1]], copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(field0(%[[VALUE_conv_u]]))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_conv_u_2:[0-9]+]] conv_u: @type[[TYPE2]] [storage=automatic];
// DEFAULT-NEXT:                 write<u32>(field1(%[[VALUE_conv_u_2]]), read<u32>(%[[VALUE_arg2]]));
// DEFAULT-NEXT:                 write<@type[[TYPE0]]>(%[[VALUE_vsrc2]], copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(field0(%[[VALUE_conv_u_2]]))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_tmp:[0-9]+]] tmp: i8 [storage=automatic];
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_tmp]], read<i8>(field0(%[[VALUE_vsrc2]])));
// DEFAULT-NEXT:                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp]])), mul<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), const<i32>(8)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(field0(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp]])), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i8>(field0(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_vsrc1]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp]])), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i8>(field0(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_vsrc1]]))), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp]])), const<i32>(1)))));
// DEFAULT-NEXT:                                 let %[[VALUE3:[0-9]+]]: i8 [synthetic] = read<i8>(field0(%[[VALUE_vdest]]));
// DEFAULT-NEXT:                                 let %[[VALUE4:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE3]])), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field0(%[[VALUE_vdest]]), read<i8>(%[[VALUE4]]));
// DEFAULT-NEXT:                                 let %[[VALUE5:[0-9]+]]: i8 [synthetic] = read<i8>(field0(%[[VALUE_vdest]]));
// DEFAULT-NEXT:                                 let %[[VALUE6:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE5]])), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field0(%[[VALUE_vdest]]), read<i8>(%[[VALUE6]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp]])), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field0(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_vsrc1]]))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp]]))))), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp]]))))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field0(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_vsrc1]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp]])))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_tmp_2:[0-9]+]] tmp: i8 [storage=automatic];
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_tmp_2]], read<i8>(field1(%[[VALUE_vsrc2]])));
// DEFAULT-NEXT:                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_2]])), mul<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), const<i32>(8)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(field1(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_2]])), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i8>(field1(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_vsrc1]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_2]])), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i8>(field1(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_vsrc1]]))), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_2]])), const<i32>(1)))));
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: i8 [synthetic] = read<i8>(field1(%[[VALUE_vdest]]));
// DEFAULT-NEXT:                                 let %[[VALUE9:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE8]])), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field1(%[[VALUE_vdest]]), read<i8>(%[[VALUE9]]));
// DEFAULT-NEXT:                                 let %[[VALUE10:[0-9]+]]: i8 [synthetic] = read<i8>(field1(%[[VALUE_vdest]]));
// DEFAULT-NEXT:                                 let %[[VALUE11:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE10]])), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field1(%[[VALUE_vdest]]), read<i8>(%[[VALUE11]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_2]])), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field1(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_vsrc1]]))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_2]]))))), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_2]]))))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field1(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(field1(%[[VALUE_vsrc1]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_2]])))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_tmp_3:[0-9]+]] tmp: i8 [storage=automatic];
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_tmp_3]], read<i8>(field2(%[[VALUE_vsrc2]])));
// DEFAULT-NEXT:                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_3]])), mul<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), const<i32>(8)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(field2(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_3]])), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i8>(field2(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field2(%[[VALUE_vsrc1]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_3]])), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i8>(field2(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field2(%[[VALUE_vsrc1]]))), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_3]])), const<i32>(1)))));
// DEFAULT-NEXT:                                 let %[[VALUE13:[0-9]+]]: i8 [synthetic] = read<i8>(field2(%[[VALUE_vdest]]));
// DEFAULT-NEXT:                                 let %[[VALUE14:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE13]])), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field2(%[[VALUE_vdest]]), read<i8>(%[[VALUE14]]));
// DEFAULT-NEXT:                                 let %[[VALUE15:[0-9]+]]: i8 [synthetic] = read<i8>(field2(%[[VALUE_vdest]]));
// DEFAULT-NEXT:                                 let %[[VALUE16:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE15]])), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field2(%[[VALUE_vdest]]), read<i8>(%[[VALUE16]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_3]])), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field2(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field2(%[[VALUE_vsrc1]]))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_3]]))))), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_3]]))))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field2(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(field2(%[[VALUE_vsrc1]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_3]])))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_tmp_4:[0-9]+]] tmp: i8 [storage=automatic];
// DEFAULT-NEXT:                 write<i8>(%[[VALUE_tmp_4]], read<i8>(field3(%[[VALUE_vsrc2]])));
// DEFAULT-NEXT:                 if ge<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_4]])), mul<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1))), const<i32>(8)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<i8>(field3(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_4]])), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i8>(field3(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field3(%[[VALUE_vsrc1]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_4]])), mul<i32, overflow=ub>(neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(1)))), const<i32>(8)))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i8>(field3(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(field3(%[[VALUE_vsrc1]]))), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_4]])), const<i32>(1)))));
// DEFAULT-NEXT:                                 let %[[VALUE18:[0-9]+]]: i8 [synthetic] = read<i8>(field3(%[[VALUE_vdest]]));
// DEFAULT-NEXT:                                 let %[[VALUE19:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE18]])), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field3(%[[VALUE_vdest]]), read<i8>(%[[VALUE19]]));
// DEFAULT-NEXT:                                 let %[[VALUE20:[0-9]+]]: i8 [synthetic] = read<i8>(field3(%[[VALUE_vdest]]));
// DEFAULT-NEXT:                                 let %[[VALUE21:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE20]])), const<i32>(1)));
// DEFAULT-NEXT:                                 write<i8>(field3(%[[VALUE_vdest]]), read<i8>(%[[VALUE21]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if lt<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_4]])), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field3(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field3(%[[VALUE_vsrc1]]))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_4]]))))), neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_4]]))))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<i8>(field3(%[[VALUE_vdest]]), truncate<i8, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(field3(%[[VALUE_vsrc1]]))), widen<i32, reason=promotion>(read<i8>(%[[VALUE_tmp_4]])))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         do %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_conv_u_3:[0-9]+]] conv_u: @type[[TYPE3]] [storage=automatic];
// DEFAULT-NEXT:                 write<@type[[TYPE0]]>(field0(%[[VALUE_conv_u_3]]), copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(%[[VALUE_vdest]])));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_res]], read<u32>(field1(%[[VALUE_conv_u_3]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_res]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: u32 [storage=automatic] = call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE_helper_neon_rshl_s8]], reinterpret<u32, reason=arg, fits=always>(const<i32>(84215045)), reinterpret<u32, reason=arg, fits=always>(const<i32>(16843009)));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%[[VALUE_r]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(168430090)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
