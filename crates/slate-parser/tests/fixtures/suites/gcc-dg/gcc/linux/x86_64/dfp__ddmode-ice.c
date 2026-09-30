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
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: array<array<d64, 258>, 258> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_dd:[0-9]+]] dd: array<array<d64, 258>, 258> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ry:[0-9]+]] ry: array<array<d64, 258>, 258> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> d64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_im:[0-9]+]] im: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_jm:[0-9]+]] jm: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ip:[0-9]+]] ip: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_jp:[0-9]+]] jp: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i2m:[0-9]+]] i2m: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i1p:[0-9]+]] i1p: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_qi:[0-9]+]] qi: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_qj:[0-9]+]] qj: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xx:[0-9]+]] xx: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_yx:[0-9]+]] yx: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xy:[0-9]+]] xy: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_yy:[0-9]+]] yy: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_rel:[0-9]+]] rel: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_qxx:[0-9]+]] qxx: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_qyy:[0-9]+]] qyy: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_qxy:[0-9]+]] qxy: d64 [storage=automatic];
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_jp]], add<i32, overflow=ub>(read<i32>(%[[VALUE_j]]), const<i32>(1)));
// DEFAULT-NEXT:                 for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE_i1p]]));
// DEFAULT-NEXT:                     condition: le<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_i2m]]))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_ip]], add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1)));
// DEFAULT-NEXT:                             write<d64>(%[[VALUE_yx]], sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_ip]])))), read<i32>(%[[VALUE_j]])))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_im]])))), read<i32>(%[[VALUE_j]]))))));
// DEFAULT-NEXT:                             write<d64>(%[[VALUE_yy]], sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_jp]])))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_jm]]))))));
// DEFAULT-NEXT:                             write<d64>(%[[VALUE_a]], mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(0.25), add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_xy]]), read<d64>(%[[VALUE_xy]])), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_yy]]), read<d64>(%[[VALUE_yy]])))));
// DEFAULT-NEXT:                             write<d64>(%[[VALUE_b]], mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(0.25), add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_xx]]), read<d64>(%[[VALUE_xx]])), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_yx]]), read<d64>(%[[VALUE_yx]])))));
// DEFAULT-NEXT:                             write<d64>(%[[VALUE_c]], mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(0.125), add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_xx]]), read<d64>(%[[VALUE_xy]])), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_yx]]), read<d64>(%[[VALUE_yy]])))));
// DEFAULT-NEXT:                             write<d64>(%[[VALUE_qj]], const<d64>(0.0));
// DEFAULT-NEXT:                             write<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_dd]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_m]]))), add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_b]]), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_a]]), read<d64>(%[[VALUE_rel]]))), read<d64>(%[[VALUE_b]])));
// DEFAULT-NEXT:                             write<d64>(%[[VALUE_qxx]], add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_ip]])))), read<i32>(%[[VALUE_j]])))), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(2.0), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]])))))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_im]])))), read<i32>(%[[VALUE_j]]))))));
// DEFAULT-NEXT:                             write<d64>(%[[VALUE_qyy]], add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_jp]])))), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(2.0), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]])))))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_jm]]))))));
// DEFAULT-NEXT:                             write<d64>(%[[VALUE_qxy]], add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_ip]])))), read<i32>(%[[VALUE_jp]])))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_ip]])))), read<i32>(%[[VALUE_jm]]))))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_im]])))), read<i32>(%[[VALUE_jp]]))))), read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_y]]), read<i32>(%[[VALUE_im]])))), read<i32>(%[[VALUE_jm]]))))));
// DEFAULT-NEXT:                             write<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(array_decay<ptr<d64>, length=Some(258)>(deref(ptr_offset<ptr<array<d64, 258>>, subtract=false, element=array<d64, 258>, overflow=ub>(array_decay<ptr<array<d64, 258>>, length=Some(258)>(%[[VALUE_ry]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_m]]))), add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<d64, rounding=nearest_even, exceptions=observable, contract=fast>(add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_a]]), read<d64>(%[[VALUE_qxx]])), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_b]]), read<d64>(%[[VALUE_qyy]]))), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_c]]), read<d64>(%[[VALUE_qxy]]))), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_yx]]), read<d64>(%[[VALUE_qi]]))), mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(read<d64>(%[[VALUE_yy]]), read<d64>(%[[VALUE_qj]]))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
