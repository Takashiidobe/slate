/* { dg-do compile } */
/* { dg-options "-std=c2y -pedantic-errors -Wvla-parameter" } */

void fix_fix (int i,
	      char (*a)[3][5],
	      int (*x)[_Countof (*a)],
	      short (*)[_Generic(x, int (*)[3]: 1)]);
void fix_var (int i,
	      char (*a)[3][i], /* dg-warn "variable" */
	      int (*x)[_Countof (*a)],
	      short (*)[_Generic(x, int (*)[3]: 1)]);
void fix_uns (int i,
	      char (*a)[3][*],
	      int (*x)[_Countof (*a)],
	      short (*)[_Generic(x, int (*)[3]: 1)]);

void var_fix (int i,
	      char (*a)[i][5], /* dg-warn "variable" */
	      int (*x)[_Countof (*a)]); /* dg-warn "variable" */
void var_var (int i,
	      char (*a)[i][i], /* dg-warn "variable" */
	      int (*x)[_Countof (*a)]); /* dg-warn "variable" */
void var_uns (int i,
	      char (*a)[i][*], /* dg-warn "variable" */
	      int (*x)[_Countof (*a)]); /* dg-warn "variable" */

void uns_fix (int i,
	      char (*a)[*][5],
	      int (*x)[_Countof (*a)]);
void uns_var (int i,
	      char (*a)[*][i], /* dg-warn "variable" */
	      int (*x)[_Countof (*a)]);
void uns_uns (int i,
	      char (*a)[*][*],
	      int (*x)[_Countof (*a)]);

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:     fn %[[VALUE_fix_fix:[0-9]+]] @fix_fix(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_a:[0-9]+]] a: ptr<array<array<i8, 5>, 3>>, %[[VALUE_x:[0-9]+]] x: ptr<vla<i32, *>>, %[[VALUE0:[0-9]+]] <unnamed>: ptr<vla<i16, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fix_var:[0-9]+]] @fix_var(%[[VALUE_i_2:[0-9]+]] i: i32, %[[VALUE_a_2:[0-9]+]] a: ptr<array<vla<i8, *>, 3>>, %[[VALUE_x_2:[0-9]+]] x: ptr<vla<i32, *>>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<vla<i16, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fix_uns:[0-9]+]] @fix_uns(%[[VALUE_i_3:[0-9]+]] i: i32, %[[VALUE_a_3:[0-9]+]] a: ptr<array<vla<i8, *>, 3>>, %[[VALUE_x_3:[0-9]+]] x: ptr<vla<i32, *>>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<vla<i16, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_var_fix:[0-9]+]] @var_fix(%[[VALUE_i_4:[0-9]+]] i: i32, %[[VALUE_a_4:[0-9]+]] a: ptr<vla<array<i8, 5>, *>>, %[[VALUE_x_4:[0-9]+]] x: ptr<vla<i32, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_var_var:[0-9]+]] @var_var(%[[VALUE_i_5:[0-9]+]] i: i32, %[[VALUE_a_5:[0-9]+]] a: ptr<vla<vla<i8, *>, *>>, %[[VALUE_x_5:[0-9]+]] x: ptr<vla<i32, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_var_uns:[0-9]+]] @var_uns(%[[VALUE_i_6:[0-9]+]] i: i32, %[[VALUE_a_6:[0-9]+]] a: ptr<vla<vla<i8, *>, *>>, %[[VALUE_x_6:[0-9]+]] x: ptr<vla<i32, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_uns_fix:[0-9]+]] @uns_fix(%[[VALUE_i_7:[0-9]+]] i: i32, %[[VALUE_a_7:[0-9]+]] a: ptr<vla<array<i8, 5>, *>>, %[[VALUE_x_7:[0-9]+]] x: ptr<vla<i32, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_uns_var:[0-9]+]] @uns_var(%[[VALUE_i_8:[0-9]+]] i: i32, %[[VALUE_a_8:[0-9]+]] a: ptr<vla<vla<i8, *>, *>>, %[[VALUE_x_8:[0-9]+]] x: ptr<vla<i32, *>>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_uns_uns:[0-9]+]] @uns_uns(%[[VALUE_i_9:[0-9]+]] i: i32, %[[VALUE_a_9:[0-9]+]] a: ptr<vla<vla<i8, *>, *>>, %[[VALUE_x_9:[0-9]+]] x: ptr<vla<i32, *>>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
