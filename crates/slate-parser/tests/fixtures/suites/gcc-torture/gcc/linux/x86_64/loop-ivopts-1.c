/* From PR 18977.  */
void foo(float *x);

int main() {
  float x[4];
  foo(x);
  return 0;
}

void foo(float *x) {
  int          i, j, k;
  float        temp;
  static float t16[16] = {1., 2.,  3.,  4.,  5.,  6.,  7.,  8.,
                          9., 10., 11., 12., 13., 14., 15., 16.};
  static float tmp[4]  = {0., 0., 0., 0.};

  for (i = 0; i < 4; i++) {
    k    = 3 - i;
    temp = t16[5 * k];
    for (j = k + 1; j < 4; j++) {
      tmp[k] = t16[k + j * 4] * temp;
    }
  }
  x[0] = tmp[0];
  x[1] = tmp[1];
  x[2] = tmp[2];
  x[3] = tmp[3];
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
// DEFAULT-NEXT:     global %[[VALUE_t16:[0-9]+]] t16: array<f32, 16> [storage=static] [align=16] = aggregate<array<f32, 16>, zero_fill=false>(index0 = float_narrow<f32,
// DEFAULT-SAME: reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(3.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(4.0)), index4 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(5.0)), index5 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(6.0)), index6 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(7.0)), index7 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(8.0)), index8 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(9.0)), index9 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(10.0)), index10 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(11.0)), index11 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(12.0)), index12 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(13.0)), index13 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(14.0)), index14 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(15.0)), index15 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(16.0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_tmp:[0-9]+]] tmp: array<f32, 4> [storage=static] [align=16] = aggregate<array<f32, 4>, zero_fill=false>(index0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), index1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), index2 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), index3 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_temp:[0-9]+]] temp: f32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_k]], sub<i32, overflow=ub>(const<i32>(3), read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:                     write<f32>(%[[VALUE_temp]], read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_t16]]), mul<i32, overflow=ub>(const<i32>(5), read<i32>(%[[VALUE_k]]))))));
// DEFAULT-NEXT:                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], add<i32, overflow=ub>(read<i32>(%[[VALUE_k]]), const<i32>(1)));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(4))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(%[[VALUE_tmp]]), read<i32>(%[[VALUE_k]]))), mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(16)>(%[[VALUE_t16]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_k]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_j]]), const<i32>(4)))))), read<f32>(%[[VALUE_temp]])));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_x]]), const<i32>(0))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(%[[VALUE_tmp]]), const<i32>(0)))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_x]]), const<i32>(1))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(%[[VALUE_tmp]]), const<i32>(1)))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_x]]), const<i32>(2))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(%[[VALUE_tmp]]), const<i32>(2)))));
// DEFAULT-NEXT:         write<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(read<ptr<f32>>(%[[VALUE_x]]), const<i32>(3))), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(%[[VALUE_tmp]]), const<i32>(3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: array<f32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f32>) -> void>(%[[VALUE_foo]], array_decay<ptr<f32>, length=Some(4)>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
