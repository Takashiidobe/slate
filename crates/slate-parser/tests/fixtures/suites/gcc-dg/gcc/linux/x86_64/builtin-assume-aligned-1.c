/* { dg-do compile } */
/* { dg-options "-O3 -fno-tree-vectorize -fdump-tree-optimized-alias" } */

void
test1 (double *out1, double *out2, double *out3, double *in1,
       double *in2, int len)
{
  int i;
  double *__restrict o1 = __builtin_assume_aligned (out1, 16);
  double *__restrict o2 = __builtin_assume_aligned (out2, 16);
  double *__restrict o3 = __builtin_assume_aligned (out3, 16);
  double *__restrict i1 = __builtin_assume_aligned (in1, 16);
  double *__restrict i2 = __builtin_assume_aligned (in2, 16);
  for (i = 0; i < len; ++i)
    {
      o1[i] = i1[i] * i2[i];
      o2[i] = i1[i] + i2[i];
      o3[i] = i1[i] - i2[i];
    }
}

/* { dg-final { scan-tree-dump-times " ALIGN = 16, MISALIGN = 0" 5 "optimized" } } */

void
test2 (double *out1, double *out2, double *out3, double *in1,
       double *in2, int len)
{
  int i, align = 32, misalign = 16;
  out1 = __builtin_assume_aligned (out1, align, misalign);
  out2 = __builtin_assume_aligned (out2, align, 16);
  out3 = __builtin_assume_aligned (out3, 32, misalign);
  in1 = __builtin_assume_aligned (in1, 32, 16);
  in2 = __builtin_assume_aligned (in2, 32, 0);
  for (i = 0; i < len; ++i)
    {
      out1[i] = in1[i] * in2[i];
      out2[i] = in1[i] + in2[i];
      out3[i] = in1[i] - in2[i];
    }
}


/* { dg-final { scan-tree-dump-times " ALIGN = 32" 5 "optimized" } } */
/* { dg-final { scan-tree-dump-times " ALIGN = 32, MISALIGN = 16" 4 "optimized" } } */
/* { dg-final { scan-tree-dump-times " ALIGN = 32, MISALIGN = 0" 1 "optimized" } } */


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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_assume_aligned:[0-9]+]] @__builtin_assume_aligned(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: u64, ...) -> ptr<void> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_out1:[0-9]+]] out1: ptr<f64>, %[[VALUE_out2:[0-9]+]] out2: ptr<f64>, %[[VALUE_out3:[0-9]+]] out3: ptr<f64>, %[[VALUE_in1:[0-9]+]] in1: ptr<f64>, %[[VALUE_in2:[0-9]+]] in2: ptr<f64>, %[[VALUE_len:[0-9]+]] len: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_o1:[0-9]+]] o1: ptr<f64> [storage=automatic] [restrict] = pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%[[VALUE_out1]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         let %[[VALUE_o2:[0-9]+]] o2: ptr<f64> [storage=automatic] [restrict] = pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%[[VALUE_out2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         let %[[VALUE_o3:[0-9]+]] o3: ptr<f64> [storage=automatic] [restrict] = pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%[[VALUE_out3]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         let %[[VALUE_i1:[0-9]+]] i1: ptr<f64> [storage=automatic] [restrict] = pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%[[VALUE_in1]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         let %[[VALUE_i2:[0-9]+]] i2: ptr<f64> [storage=automatic] [restrict] = pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%[[VALUE_in2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_len]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_o1]]), read<i32>(%[[VALUE_i]]))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i1]]), read<i32>(%[[VALUE_i]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i2]]), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_o2]]), read<i32>(%[[VALUE_i]]))), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i1]]), read<i32>(%[[VALUE_i]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i2]]), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_o3]]), read<i32>(%[[VALUE_i]]))), sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i1]]), read<i32>(%[[VALUE_i]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i2]]), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_out1_2:[0-9]+]] out1: ptr<f64>, %[[VALUE_out2_2:[0-9]+]] out2: ptr<f64>, %[[VALUE_out3_2:[0-9]+]] out3: ptr<f64>, %[[VALUE_in1_2:[0-9]+]] in1: ptr<f64>, %[[VALUE_in2_2:[0-9]+]] in2: ptr<f64>, %[[VALUE_len_2:[0-9]+]] len: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_align:[0-9]+]] align: i32 [storage=automatic] = const<i32>(32);
// DEFAULT-NEXT:         let %[[VALUE_misalign:[0-9]+]] misalign: i32 [storage=automatic] = const<i32>(16);
// DEFAULT-NEXT:         write<ptr<f64>>(%[[VALUE_out1_2]], pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%[[VALUE_out1_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_align]]))), read<i32>(%[[VALUE_misalign]]))));
// DEFAULT-NEXT:         write<ptr<f64>>(%[[VALUE_out2_2]], pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%[[VALUE_out2_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_align]]))), const<i32>(16))));
// DEFAULT-NEXT:         write<ptr<f64>>(%[[VALUE_out3_2]], pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%[[VALUE_out3_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))), read<i32>(%[[VALUE_misalign]]))));
// DEFAULT-NEXT:         write<ptr<f64>>(%[[VALUE_in1_2]], pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%[[VALUE_in1_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))), const<i32>(16))));
// DEFAULT-NEXT:         write<ptr<f64>>(%[[VALUE_in2_2]], pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%[[VALUE___builtin_assume_aligned]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%[[VALUE_in2_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))), const<i32>(0))));
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_len_2]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_out1_2]]), read<i32>(%[[VALUE_i_2]]))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_in1_2]]), read<i32>(%[[VALUE_i_2]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_in2_2]]), read<i32>(%[[VALUE_i_2]]))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_out2_2]]), read<i32>(%[[VALUE_i_2]]))), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_in1_2]]), read<i32>(%[[VALUE_i_2]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_in2_2]]), read<i32>(%[[VALUE_i_2]]))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_out3_2]]), read<i32>(%[[VALUE_i_2]]))), sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_in1_2]]), read<i32>(%[[VALUE_i_2]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_in2_2]]), read<i32>(%[[VALUE_i_2]]))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
