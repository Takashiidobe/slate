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
// DEFAULT-NEXT:     type @type[[TYPE_bar0:[0-9]+]] bar0 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: array<i32, 3>;
// DEFAULT-NEXT:         field2 y: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_bar0_2:[0-9]+]] bar0 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: array<i32, 4>;
// DEFAULT-NEXT:         field2 y: i32;
// DEFAULT-NEXT:     } [size=24, align=4, offsets=[0, 4, 20]];
// DEFAULT-NEXT:     type @type[[TYPE_bar1:[0-9]+]] bar1 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, 3>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_bar1_2:[0-9]+]] bar1 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, 3>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_bar2:[0-9]+]] bar2 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, 3>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_bar2_2:[0-9]+]] bar2 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, 4>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_bar3:[0-9]+]] bar3 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, 3>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_bar3_2:[0-9]+]] bar3 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: ptr<array<i32, incomplete>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_test_bar0:[0-9]+]] @test_bar0(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_bar0]]>, %[[VALUE_b:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_bar0]]>>(%[[VALUE_a]]))), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_bar0_2]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_bar0_2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_bar0_2]]>>(%[[VALUE_p]]))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type[[TYPE_bar0]]>>(%[[VALUE_a]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_bar1:[0-9]+]] @test_bar1(%[[VALUE_a_2:[0-9]+]] a: ptr<@type[[TYPE_bar1]]>, %[[VALUE_b_2:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_bar1]]>>(%[[VALUE_a_2]]))), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_bar1_2]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_bar1_2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_bar1_2]]>>(%[[VALUE_p_2]]))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type[[TYPE_bar1]]>>(%[[VALUE_a_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_bar2:[0-9]+]] @test_bar2(%[[VALUE_a_3:[0-9]+]] a: ptr<@type[[TYPE_bar2]]>, %[[VALUE_b_3:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_bar2]]>>(%[[VALUE_a_3]]))), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_bar2_2]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_bar2_2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_bar2_2]]>>(%[[VALUE_p_3]]))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type[[TYPE_bar2]]>>(%[[VALUE_a_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_bar3:[0-9]+]] @test_bar3(%[[VALUE_a_4:[0-9]+]] a: ptr<@type[[TYPE_bar3]]>, %[[VALUE_b_4:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_bar3]]>>(%[[VALUE_a_4]]))), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_p_4:[0-9]+]] p: ptr<@type[[TYPE_bar3_2]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_bar3_2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_bar3_2]]>>(%[[VALUE_p_4]]))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type[[TYPE_bar3]]>>(%[[VALUE_a_4]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_z0:[0-9]+]] z0: @type[[TYPE_bar0]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn(ptr<@type[[TYPE_bar0]]>, ptr<void>) -> i32>(%[[VALUE_test_bar0]], addr_of<ptr<@type[[TYPE_bar0]]>>(%[[VALUE_z0]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bar0]]>>(%[[VALUE_z0]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_z1:[0-9]+]] z1: @type[[TYPE_bar1]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type[[TYPE_bar1]]>, ptr<void>) -> i32>(%[[VALUE_test_bar1]], addr_of<ptr<@type[[TYPE_bar1]]>>(%[[VALUE_z1]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bar1]]>>(%[[VALUE_z1]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_z2:[0-9]+]] z2: @type[[TYPE_bar2]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type[[TYPE_bar2]]>, ptr<void>) -> i32>(%[[VALUE_test_bar2]], addr_of<ptr<@type[[TYPE_bar2]]>>(%[[VALUE_z2]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bar2]]>>(%[[VALUE_z2]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_z3:[0-9]+]] z3: @type[[TYPE_bar3]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type[[TYPE_bar3]]>, ptr<void>) -> i32>(%[[VALUE_test_bar3]], addr_of<ptr<@type[[TYPE_bar3]]>>(%[[VALUE_z3]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bar3]]>>(%[[VALUE_z3]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
