/* { dg-do compile } */
/* { dg-options "-O3" } */

double *my_alloc1 (int len) __attribute__((__assume_aligned__ (16)));
double *my_alloc2 (int len) __attribute__((__assume_aligned__ (32, 16)));

void
test1 (int len)
{
  int i;
  double *__restrict o1 = my_alloc1 (len);
  double *__restrict o2 = my_alloc1 (len);
  double *__restrict o3 = my_alloc1 (len);
  double *__restrict i1 = my_alloc1 (len);
  double *__restrict i2 = my_alloc1 (len);
  for (i = 0; i < len; ++i)
    {
      o1[i] = i1[i] * i2[i];
      o2[i] = i1[i] + i2[i];
      o3[i] = i1[i] - i2[i];
    }
}

void
test2 (int len)
{
  int i;
  double *__restrict o1 = my_alloc2 (len);
  double *__restrict o2 = my_alloc2 (len);
  double *__restrict o3 = my_alloc2 (len);
  double *__restrict i1 = my_alloc2 (len);
  double *__restrict i2 = my_alloc2 (len);
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
// DEFAULT-NEXT:     fn %1 @my_alloc1(%20 len: i32) -> ptr<f64> [linkage=external];
// DEFAULT-NEXT:     fn %3 @my_alloc2(%21 len: i32) -> ptr<f64> [linkage=external];
// DEFAULT-NEXT:     fn %4 @test1(%5 len: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 o1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32) -> ptr<f64>>(%1, read<i32>(%5));
// DEFAULT-NEXT:         let %8 o2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32) -> ptr<f64>>(%1, read<i32>(%5));
// DEFAULT-NEXT:         let %9 o3: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32) -> ptr<f64>>(%1, read<i32>(%5));
// DEFAULT-NEXT:         let %10 i1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32) -> ptr<f64>>(%1, read<i32>(%5));
// DEFAULT-NEXT:         let %11 i2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32) -> ptr<f64>>(%1, read<i32>(%5));
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), read<i32>(%5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%7), read<i32>(%6))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%10), read<i32>(%6)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%11), read<i32>(%6))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%8), read<i32>(%6))), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%10), read<i32>(%6)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%11), read<i32>(%6))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%9), read<i32>(%6))), sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%10), read<i32>(%6)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%11), read<i32>(%6))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test2(%13 len: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 o1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32) -> ptr<f64>>(%3, read<i32>(%13));
// DEFAULT-NEXT:         let %16 o2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32) -> ptr<f64>>(%3, read<i32>(%13));
// DEFAULT-NEXT:         let %17 o3: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32) -> ptr<f64>>(%3, read<i32>(%13));
// DEFAULT-NEXT:         let %18 i1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32) -> ptr<f64>>(%3, read<i32>(%13));
// DEFAULT-NEXT:         let %19 i2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32) -> ptr<f64>>(%3, read<i32>(%13));
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), read<i32>(%13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%15), read<i32>(%14))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%18), read<i32>(%14)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%19), read<i32>(%14))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%16), read<i32>(%14))), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%18), read<i32>(%14)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%19), read<i32>(%14))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%17), read<i32>(%14))), sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%18), read<i32>(%14)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%19), read<i32>(%14))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
