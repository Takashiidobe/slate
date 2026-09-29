/* { dg-do compile } */
/* { dg-options "-O3" } */

double *my_alloc1 (int len, int align) __attribute__((__alloc_align__ (2)));
double *my_alloc2 (int align, int len) __attribute__((alloc_align (1)));

void
test1 (int len, int align)
{
  int i;
  double *__restrict o1 = my_alloc1 (len, 32);
  double *__restrict o2 = my_alloc1 (len, 32);
  double *__restrict o3 = my_alloc1 (len, 32);
  double *__restrict i1 = my_alloc1 (len, 32);
  double *__restrict i2 = my_alloc1 (len, align);
  for (i = 0; i < len; ++i)
    {
      o1[i] = i1[i] * i2[i];
      o2[i] = i1[i] + i2[i];
      o3[i] = i1[i] - i2[i];
    }
}

void
test2 (int len, int align)
{
  int i;
  double *__restrict o1 = my_alloc2 (32, len);
  double *__restrict o2 = my_alloc2 (32, len);
  double *__restrict o3 = my_alloc2 (32, len);
  double *__restrict i1 = my_alloc2 (32, len);
  double *__restrict i2 = my_alloc2 (align, len);
  for (i = 0; i < len; ++i)
    {
      o1[i] = i1[i] * i2[i];
      o2[i] = i1[i] + i2[i];
      o3[i] = i1[i] - i2[i];
    }
}

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
// DEFAULT-NEXT:     fn %[[VALUE_my_alloc1:[0-9]+]] @my_alloc1(%[[VALUE_len:[0-9]+]] len: i32, %[[VALUE_align:[0-9]+]] align: i32) -> ptr<f64> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_my_alloc2:[0-9]+]] @my_alloc2(%[[VALUE_align_2:[0-9]+]] align: i32, %[[VALUE_len_2:[0-9]+]] len: i32) -> ptr<f64> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_len_3:[0-9]+]] len: i32, %[[VALUE_align_3:[0-9]+]] align: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_o1:[0-9]+]] o1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%[[VALUE_my_alloc1]], read<i32>(%[[VALUE_len_3]]), const<i32>(32));
// DEFAULT-NEXT:         let %[[VALUE_o2:[0-9]+]] o2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%[[VALUE_my_alloc1]], read<i32>(%[[VALUE_len_3]]), const<i32>(32));
// DEFAULT-NEXT:         let %[[VALUE_o3:[0-9]+]] o3: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%[[VALUE_my_alloc1]], read<i32>(%[[VALUE_len_3]]), const<i32>(32));
// DEFAULT-NEXT:         let %[[VALUE_i1:[0-9]+]] i1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%[[VALUE_my_alloc1]], read<i32>(%[[VALUE_len_3]]), const<i32>(32));
// DEFAULT-NEXT:         let %[[VALUE_i2:[0-9]+]] i2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%[[VALUE_my_alloc1]], read<i32>(%[[VALUE_len_3]]), read<i32>(%[[VALUE_align_3]]));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_len_3]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_o1]]), read<i32>(%[[VALUE_i]]))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i1]]), read<i32>(%[[VALUE_i]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i2]]), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_o2]]), read<i32>(%[[VALUE_i]]))), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i1]]), read<i32>(%[[VALUE_i]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i2]]), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_o3]]), read<i32>(%[[VALUE_i]]))), sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i1]]), read<i32>(%[[VALUE_i]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i2]]), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_len_4:[0-9]+]] len: i32, %[[VALUE_align_4:[0-9]+]] align: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_o1_2:[0-9]+]] o1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%[[VALUE_my_alloc2]], const<i32>(32), read<i32>(%[[VALUE_len_4]]));
// DEFAULT-NEXT:         let %[[VALUE_o2_2:[0-9]+]] o2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%[[VALUE_my_alloc2]], const<i32>(32), read<i32>(%[[VALUE_len_4]]));
// DEFAULT-NEXT:         let %[[VALUE_o3_2:[0-9]+]] o3: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%[[VALUE_my_alloc2]], const<i32>(32), read<i32>(%[[VALUE_len_4]]));
// DEFAULT-NEXT:         let %[[VALUE_i1_2:[0-9]+]] i1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%[[VALUE_my_alloc2]], const<i32>(32), read<i32>(%[[VALUE_len_4]]));
// DEFAULT-NEXT:         let %[[VALUE_i2_2:[0-9]+]] i2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%[[VALUE_my_alloc2]], read<i32>(%[[VALUE_align_4]]), read<i32>(%[[VALUE_len_4]]));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_len_4]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_o1_2]]), read<i32>(%[[VALUE_i_2]]))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i1_2]]), read<i32>(%[[VALUE_i_2]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i2_2]]), read<i32>(%[[VALUE_i_2]]))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_o2_2]]), read<i32>(%[[VALUE_i_2]]))), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i1_2]]), read<i32>(%[[VALUE_i_2]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i2_2]]), read<i32>(%[[VALUE_i_2]]))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_o3_2]]), read<i32>(%[[VALUE_i_2]]))), sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i1_2]]), read<i32>(%[[VALUE_i_2]])))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_i2_2]]), read<i32>(%[[VALUE_i_2]]))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
