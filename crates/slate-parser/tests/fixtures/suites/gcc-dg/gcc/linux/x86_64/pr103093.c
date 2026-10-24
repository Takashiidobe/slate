/* { dg-do compile } */
/* { dg-options "-fpermissive -O2" } */

int i_0, c_4, uc_7, func_2_c_11;

short *func_2_ptr_10;

void func_2() {
  uc_7 = 7;
  for (; uc_7 <= 60; uc_7 += 1) {
    c_4 = 5;
    for (; c_4 <= 76; c_4 += 1) {
      func_2_ptr_10 = &i_0;	/* { dg-warning "assignment to .*" } */
      if ((i_0 |= 5) > 0 ?: (60 && uc_7) | *func_2_ptr_10)
        if (func_2_c_11)
          for (;;)
            ;
    }
  }
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %[[VALUE_i_0:[0-9]+]] i_0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c_4:[0-9]+]] c_4: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_uc_7:[0-9]+]] uc_7: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_func_2_c_11:[0-9]+]] func_2_c_11: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_func_2_ptr_10:[0-9]+]] func_2_ptr_10: ptr<i16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func_2:[0-9]+]] @func_2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_uc_7]], const<i32>(7));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_uc_7]]), const<i32>(60))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_uc_7]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_uc_7]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_c_4]], const<i32>(5));
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: le<i32>(read<i32>(%[[VALUE_c_4]]), const<i32>(76))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c_4]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_c_4]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<ptr<i16>>(%[[VALUE_func_2_ptr_10]], pointer_cast<ptr<i16>, reason=assign>(addr_of<ptr<i32>>(%[[VALUE_i_0]])));
// DEFAULT-NEXT:                                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_0]]);
// DEFAULT-NEXT:                                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE6]]), const<i32>(5));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_i_0]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = from_bool<i32, reason=promotion>(gt<i32>(read<i32>(%[[VALUE7]]), const<i32>(0)));
// DEFAULT-NEXT:                                 if ne<i32>(conditional<i32>(ne<i32>(read<i32>(%[[VALUE8]]), const<i32>(0)), read<i32>(%[[VALUE8]]), or<i32>(from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(const<i32>(60), const<i32>(0)), ne<i32>(read<i32>(%[[VALUE_uc_7]]), const<i32>(0)))), widen<i32, reason=promotion>(read<i16>(deref(read<ptr<i16>>(%[[VALUE_func_2_ptr_10]])))))), const<i32>(0))
// DEFAULT-NEXT:                                     if ne<i32>(read<i32>(%[[VALUE_func_2_c_11]]), const<i32>(0))
// DEFAULT-NEXT:                                         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                                             init:
// DEFAULT-NEXT:                                             condition: omitted
// DEFAULT-NEXT:                                             increment: omitted
// DEFAULT-NEXT:                                             body:
// DEFAULT-NEXT:                                                 ;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
