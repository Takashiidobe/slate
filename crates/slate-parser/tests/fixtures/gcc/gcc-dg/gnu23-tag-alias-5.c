/* { dg-do run }
 * { dg-options "-std=gnu23 -O2" }
 */

/* This test checks that different field offsets imply
 * that the types can be assumed not to alias
 * and that this is exploited during optimization.  */


struct bar0 { int x; int f[3]; int y; };

[[gnu::noinline,gnu::noipa]]
int test_bar0(struct bar0* a, void* b)
{
	a->x = 1;

	struct bar0 { int x; int f[4]; int y; }* p = b;
	p->x = 2;

	return a->x;
}


/* While these tests check that different structs with different
 * sizes in arrays pointed to by field members can alias,
 * even though the types are incompatible.  */


struct bar1 { int x; int (*f)[3]; };

[[gnu::noinline,gnu::noipa]]
int test_bar1(struct bar1* a, void* b)
{
	a->x = 1;

	struct bar1 { int x; int (*f)[3]; }* p = b;
	p->x = 2;

	return a->x;
}


struct bar2 { int x; int (*f)[3]; };

[[gnu::noinline,gnu::noipa]]
int test_bar2(struct bar2* a, void* b)
{
	a->x = 1;

	struct bar2 { int x; int (*f)[4]; }* p = b;
	p->x = 2;

	return a->x;
}



/* This test checks that different structs with pointers to
 * different compatible arrays types can alias.  */


struct bar3 { int x; int (*f)[3]; };

[[gnu::noinline,gnu::noipa]]
int test_bar3(struct bar3* a, void* b)
{
	a->x = 1;

	struct bar3 { int x; int (*f)[]; }* p = b;
	p->x = 2;

	return a->x;
}




int main()
{
	// control

	struct bar0 z0;

	if (1 != test_bar0(&z0, &z0))
		__builtin_abort();

	// this could be different
	struct bar1 z1;

	if (2 != test_bar1(&z1, &z1))
		__builtin_abort();

	struct bar2 z2;

	if (2 != test_bar2(&z2, &z2))
		__builtin_abort();

	struct bar3 z3;

	if (2 != test_bar3(&z3, &z3))
		__builtin_abort();


	return 0;
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
// DEFAULT-NEXT:     type @type0 bar0 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: array<i32, 3>;
// DEFAULT-NEXT:         field2 y: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 16]];
// DEFAULT-NEXT:     type @type1 bar0 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: array<i32, 4>;
// DEFAULT-NEXT:         field2 y: i32;
// DEFAULT-NEXT:     } [size=24, align=4, offsets=[0, 4, 20]];
// DEFAULT-NEXT:     type @type2 bar1 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, 3>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 bar1 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, 3>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type4 bar2 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, 3>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type5 bar2 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, 4>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type6 bar3 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, 3>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type7 bar3 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, incomplete>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %1 @test_bar0(%2 a: ptr<@type0>, %3 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%2))), const<i32>(1));
// DEFAULT-NEXT:         let %5 p: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(read<ptr<void>>(%3));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type1>>(%5))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test_bar1(%8 a: ptr<@type2>, %9 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type2>>(%8))), const<i32>(1));
// DEFAULT-NEXT:         let %11 p: ptr<@type3> [storage=automatic] = pointer_cast<ptr<@type3>, reason=assign>(read<ptr<void>>(%9));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type3>>(%11))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type2>>(%8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test_bar2(%14 a: ptr<@type4>, %15 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type4>>(%14))), const<i32>(1));
// DEFAULT-NEXT:         let %17 p: ptr<@type5> [storage=automatic] = pointer_cast<ptr<@type5>, reason=assign>(read<ptr<void>>(%15));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type5>>(%17))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type4>>(%14))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_bar3(%20 a: ptr<@type6>, %21 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type6>>(%20))), const<i32>(1));
// DEFAULT-NEXT:         let %23 p: ptr<@type7> [storage=automatic] = pointer_cast<ptr<@type7>, reason=assign>(read<ptr<void>>(%21));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type7>>(%23))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type6>>(%20))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %24 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %25 z0: @type0 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn(ptr<@type0>, ptr<void>) -> i32>(%1, addr_of<ptr<@type0>>(%25), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%25))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%29);
// DEFAULT-NEXT:         let %26 z1: @type2 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type2>, ptr<void>) -> i32>(%7, addr_of<ptr<@type2>>(%26), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%26))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%29);
// DEFAULT-NEXT:         let %27 z2: @type4 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type4>, ptr<void>) -> i32>(%13, addr_of<ptr<@type4>>(%27), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type4>>(%27))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%29);
// DEFAULT-NEXT:         let %28 z3: @type6 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type6>, ptr<void>) -> i32>(%19, addr_of<ptr<@type6>>(%28), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type6>>(%28))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%29);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
