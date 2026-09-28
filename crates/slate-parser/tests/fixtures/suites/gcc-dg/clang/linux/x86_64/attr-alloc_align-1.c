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
// DEFAULT-NEXT:     fn %2 @my_alloc1(%24 len: i32, %25 align: i32) -> ptr<f64> [linkage=external];
// DEFAULT-NEXT:     fn %5 @my_alloc2(%26 align: i32, %27 len: i32) -> ptr<f64> [linkage=external];
// DEFAULT-NEXT:     fn %6 @test1(%7 len: i32, %8 align: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 o1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%2, read<i32>(%7), const<i32>(32));
// DEFAULT-NEXT:         let %11 o2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%2, read<i32>(%7), const<i32>(32));
// DEFAULT-NEXT:         let %12 o3: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%2, read<i32>(%7), const<i32>(32));
// DEFAULT-NEXT:         let %13 i1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%2, read<i32>(%7), const<i32>(32));
// DEFAULT-NEXT:         let %14 i2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%2, read<i32>(%7), read<i32>(%8));
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), read<i32>(%7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%31));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%10), read<i32>(%9))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%13), read<i32>(%9)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%14), read<i32>(%9))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%11), read<i32>(%9))), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%13), read<i32>(%9)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%14), read<i32>(%9))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%12), read<i32>(%9))), sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%13), read<i32>(%9)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%14), read<i32>(%9))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test2(%16 len: i32, %17 align: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %18 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %19 o1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%5, const<i32>(32), read<i32>(%16));
// DEFAULT-NEXT:         let %20 o2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%5, const<i32>(32), read<i32>(%16));
// DEFAULT-NEXT:         let %21 o3: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%5, const<i32>(32), read<i32>(%16));
// DEFAULT-NEXT:         let %22 i1: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%5, const<i32>(32), read<i32>(%16));
// DEFAULT-NEXT:         let %23 i2: ptr<f64> [storage=automatic] [restrict] = call<ptr<f64>, signature=fn(i32, i32) -> ptr<f64>>(%5, read<i32>(%17), read<i32>(%16));
// DEFAULT-NEXT:         for %29
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%18, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%18), read<i32>(%16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %32: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%18, read<i32>(%33));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%19), read<i32>(%18))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%22), read<i32>(%18)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%23), read<i32>(%18))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%20), read<i32>(%18))), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%22), read<i32>(%18)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%23), read<i32>(%18))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%21), read<i32>(%18))), sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%22), read<i32>(%18)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%23), read<i32>(%18))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
