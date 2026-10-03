/* { dg-do compile } */
/* { dg-options "-std=gnu2y" } */

static int z[0];
static int y[_Countof (z)];

_Static_assert(_Countof (y) == 0);

void
completed (void)
{
  int z[] = {};

  static_assert (_Countof (z) == 0);
}

void zro_fix (int i,
	      char (*a)[0][5],
	      int (*x)[_Countof (*a)],
	      short (*)[_Generic(x, int (*)[0]: 1)]);
void zro_var (int i,
	      char (*a)[0][i], /* dg-warn "variable" */
	      int (*x)[_Countof (*a)],
	      short (*)[_Generic(x, int (*)[0]: 1)]);
void zro_uns (int i,
	      char (*a)[0][*],
	      int (*x)[_Countof (*a)],
	      short (*)[_Generic(x, int (*)[0]: 1)]);

void
const_expr(void)
{
  int n = 7;

  _Static_assert (_Countof (int [0][3]) == 0);
  _Static_assert (_Countof (int [0]) == 0);
  _Static_assert (_Countof (int [0][n]) == 0);
}

// SLATE-FILECHECK-STD DEFAULT gnu2y
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
// DEFAULT-NEXT:     global %[[VALUE_z:[0-9]+]] z: array<i32, 0> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: array<i32, 0> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_completed:[0-9]+]] @completed() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_z_2:[0-9]+]] z: array<i32, 0> [storage=automatic] = aggregate<array<i32, 0>, zero_fill=false>();
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_zro_fix:[0-9]+]] @zro_fix(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_a:[0-9]+]] a: ptr<array<array<i8, 5>, 0>>, %[[VALUE_x:[0-9]+]] x: ptr<vla<i32, *>>, %[[VALUE0:[0-9]+]] <unnamed>: ptr<vla<i16, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_zro_var:[0-9]+]] @zro_var(%[[VALUE_i_2:[0-9]+]] i: i32, %[[VALUE_a_2:[0-9]+]] a: ptr<array<vla<i8, *>, 0>>, %[[VALUE_x_2:[0-9]+]] x: ptr<vla<i32, *>>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<vla<i16, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_zro_uns:[0-9]+]] @zro_uns(%[[VALUE_i_3:[0-9]+]] i: i32, %[[VALUE_a_3:[0-9]+]] a: ptr<array<vla<i8, *>, 0>>, %[[VALUE_x_3:[0-9]+]] x: ptr<vla<i32, *>>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<vla<i16, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_const_expr:[0-9]+]] @const_expr() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
