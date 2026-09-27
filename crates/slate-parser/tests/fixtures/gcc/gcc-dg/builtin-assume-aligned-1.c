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


// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %25 @__builtin_assume_aligned(%23 <unnamed>: ptr<const void>, %24 <unnamed>: u64, ...) -> ptr<void> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %0 @test1(%1 out1: ptr<f64>, %2 out2: ptr<f64>, %3 out3: ptr<f64>, %4 in1: ptr<f64>, %5 in2: ptr<f64>, %6 len: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 o1: ptr<f64> [storage=automatic] [restrict] = pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%25, pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%1)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         let %9 o2: ptr<f64> [storage=automatic] [restrict] = pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%25, pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%2)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         let %10 o3: ptr<f64> [storage=automatic] [restrict] = pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%25, pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%3)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         let %11 i1: ptr<f64> [storage=automatic] [restrict] = pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%25, pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         let %12 i2: ptr<f64> [storage=automatic] [restrict] = pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%25, pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%5)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         for %26
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), read<i32>(%6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%29));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%8), read<i32>(%7))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%11), read<i32>(%7)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%12), read<i32>(%7))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%9), read<i32>(%7))), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%11), read<i32>(%7)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%12), read<i32>(%7))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%10), read<i32>(%7))), sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%11), read<i32>(%7)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%12), read<i32>(%7))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test2(%14 out1: ptr<f64>, %15 out2: ptr<f64>, %16 out3: ptr<f64>, %17 in1: ptr<f64>, %18 in2: ptr<f64>, %19 len: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %21 align: i32 [storage=automatic] = const<i32>(32);
// DEFAULT-NEXT:         let %22 misalign: i32 [storage=automatic] = const<i32>(16);
// DEFAULT-NEXT:         write<ptr<f64>>(%14, pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%25, pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%14)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%21))), read<i32>(%22))));
// DEFAULT-NEXT:         write<ptr<f64>>(%15, pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%25, pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%15)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%21))), const<i32>(16))));
// DEFAULT-NEXT:         write<ptr<f64>>(%16, pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%25, pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%16)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))), read<i32>(%22))));
// DEFAULT-NEXT:         write<ptr<f64>>(%17, pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%25, pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%17)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))), const<i32>(16))));
// DEFAULT-NEXT:         write<ptr<f64>>(%18, pointer_cast<ptr<f64>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%25, pointer_cast<ptr<const void>, reason=arg>(read<ptr<f64>>(%18)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))), const<i32>(0))));
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%20, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%20), read<i32>(%19))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                 let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%20, read<i32>(%31));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%14), read<i32>(%20))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%17), read<i32>(%20)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%18), read<i32>(%20))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%15), read<i32>(%20))), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%17), read<i32>(%20)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%18), read<i32>(%20))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%16), read<i32>(%20))), sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%17), read<i32>(%20)))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%18), read<i32>(%20))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
