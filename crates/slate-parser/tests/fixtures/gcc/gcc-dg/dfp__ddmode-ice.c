/* { dg-do compile } */
/* { dg-options "-O1" } */

/* This used to result in an ICE.  */

_Decimal64 y[258][258];
_Decimal64 dd[258][258];
_Decimal64 ry[258][258];
_Decimal64
foo (void)
{
  int i;
  int j;
  int m;
  int im;
  int jm;
  int ip;
  int jp;
  int i2m;
  int i1p;
  _Decimal64 a;
  _Decimal64 b;
  _Decimal64 c;
  _Decimal64 qi;
  _Decimal64 qj;
  _Decimal64 xx;
  _Decimal64 yx;
  _Decimal64 xy;
  _Decimal64 yy;
  _Decimal64 rel;
  _Decimal64 qxx;
  _Decimal64 qyy;
  _Decimal64 qxy;
  do
    {
      jp = j + 1;
      for (i = i1p; i <= i2m; i++)
	{
	  ip = i + 1;
	  yx = y[ip][j] - y[im][j];
	  yy = y[i][jp] - y[i][jm];
	  a = 0.25dd * (xy * xy + yy * yy);
	  b = 0.25dd * (xx * xx + yx * yx);
	  c = 0.125dd * (xx * xy + yx * yy);
	  qj = 0.0dd;
	  dd[i][m] = b + a * rel + b;
	  qxx = y[ip][j] - 2.0dd * y[i][j] + y[im][j];
	  qyy = y[i][jp] - 2.0dd * y[i][j] + y[i][jm];
	  qxy = y[ip][jp] - y[ip][jm] - y[im][jp] + y[im][jm];
	  ry[i][m] = a * qxx + b * qyy - c * qxy + yx * qi + yy * qj;
	}
    }
  while (1);
}

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
// DEFAULT-NEXT:     global %0 y: array<array<d64, 258>, 258> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %1 dd: array<array<d64, 258>, 258> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %2 ry: array<array<d64, 258>, 258> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo() -> d64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 m: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 im: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 jm: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 ip: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 jp: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 i2m: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 i1p: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 a: d64 [storage=automatic];
// DEFAULT-NEXT:         let %14 b: d64 [storage=automatic];
// DEFAULT-NEXT:         let %15 c: d64 [storage=automatic];
// DEFAULT-NEXT:         let %16 qi: d64 [storage=automatic];
// DEFAULT-NEXT:         let %17 qj: d64 [storage=automatic];
// DEFAULT-NEXT:         let %18 xx: d64 [storage=automatic];
// DEFAULT-NEXT:         let %19 yx: d64 [storage=automatic];
// DEFAULT-NEXT:         let %20 xy: d64 [storage=automatic];
// DEFAULT-NEXT:         let %21 yy: d64 [storage=automatic];
// DEFAULT-NEXT:         let %22 rel: d64 [storage=automatic];
// DEFAULT-NEXT:         let %23 qxx: d64 [storage=automatic];
// DEFAULT-NEXT:         let %24 qyy: d64 [storage=automatic];
// DEFAULT-NEXT:         let %25 qxy: d64 [storage=automatic];
// DEFAULT-NEXT:         do %26
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%10, add<i32, overflow=ub>(read<i32>(%5), const<i32>(1)));
// DEFAULT-NEXT:                 for %27
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%4, read<i32>(%12));
// DEFAULT-NEXT:                     condition: le<i32>(read<i32>(%4), read<i32>(%11))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %28: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%4, read<i32>(%29));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%9, add<i32, overflow=ub>(read<i32>(%4), const<i32>(1)));
// DEFAULT-NEXT:                             write<d64>(%19, sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%9)))), read<i32>(%5)))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%7)))), read<i32>(%5))))));
// DEFAULT-NEXT:                             write<d64>(%21, sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%4)))), read<i32>(%10)))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%4)))), read<i32>(%8))))));
// DEFAULT-NEXT:                             write<d64>(%13, mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(0.25), add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%20), read<d64>(%20)), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%21), read<d64>(%21)))));
// DEFAULT-NEXT:                             write<d64>(%14, mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(0.25), add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%18), read<d64>(%18)), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%19), read<d64>(%19)))));
// DEFAULT-NEXT:                             write<d64>(%15, mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(0.125), add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%18), read<d64>(%20)), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%19), read<d64>(%21)))));
// DEFAULT-NEXT:                             write<d64>(%17, const<d64>(0.0));
// DEFAULT-NEXT:                             write<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%1), read<i32>(%4)))), read<i32>(%6))), add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%14), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%13), read<d64>(%22))), read<d64>(%14)));
// DEFAULT-NEXT:                             write<d64>(%23, add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%9)))), read<i32>(%5)))), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(2.0), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%4)))), read<i32>(%5)))))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%7)))), read<i32>(%5))))));
// DEFAULT-NEXT:                             write<d64>(%24, add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%4)))), read<i32>(%10)))), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(2.0), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%4)))), read<i32>(%5)))))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%4)))), read<i32>(%8))))));
// DEFAULT-NEXT:                             write<d64>(%25, add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%9)))), read<i32>(%10)))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%9)))), read<i32>(%8))))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%7)))), read<i32>(%10))))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%0), read<i32>(%7)))), read<i32>(%8))))));
// DEFAULT-NEXT:                             write<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%2), read<i32>(%4)))), read<i32>(%6))), add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%13), read<d64>(%23)), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%14), read<d64>(%24))), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%15), read<d64>(%25))), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%19), read<d64>(%16))), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%21), read<d64>(%17))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
