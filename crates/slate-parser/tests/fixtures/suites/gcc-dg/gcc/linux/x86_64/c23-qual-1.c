/* Tests related to qualifiers and pointers to arrays in C23, PR98397 */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

/* test that qualifiers are preserved in tertiary operator for pointers to arrays in C23 */

void f(void)
{
	const int (*u)[1];
	void *v;
	_Static_assert(_Generic(1 ? u : v, const void*: 1, void*: 0), "lost qualifier");
	_Static_assert(_Generic(1 ? v : u, const void*: 1, void*: 0), "lost qualifier");
}

/* test that assignment of unqualified to qualified pointers works as expected */

void g(void)
{
	int (*x)[3];
	const int (*p)[3] = x;
}

/* test that assignment of qualified void pointers works as expected */

void h(void)
{
	const void* x;
	const int (*p)[3] = x;
}


// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: ptr<const array<i32, 1>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: ptr<void> [storage=automatic];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: ptr<array<i32, 3>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<const array<i32, 3>> [storage=automatic] = pointer_cast<ptr<const array<i32, 3>>, reason=assign>(read<ptr<array<i32, 3>>>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h:[0-9]+]] @h() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: ptr<const void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<const array<i32, 3>> [storage=automatic] = pointer_cast<ptr<const array<i32, 3>>, reason=assign>(read<ptr<const void>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
