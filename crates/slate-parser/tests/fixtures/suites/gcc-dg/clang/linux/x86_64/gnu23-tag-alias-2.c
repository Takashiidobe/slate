/* { dg-do run }
 * { dg-options "-std=gnu23 -O2" }
 */


/* These tests check that incompatible definitions of
   tagged types can be assumed not to alias and that
   this is exploited during optimization.  */

struct foo { int x; };

[[gnu::noinline,gnu::noipa]]
int test_foo1(struct foo* a, void* b)
{
	a->x = 1;

	struct foo { int x; int y; }* p = b;
	p->x = 2;

	return a->x;
}

[[gnu::noinline,gnu::noipa]]
int test_foo2(struct foo* a, void* b)
{
	a->x = 1;

	struct fox { int x; }* p = b;
	p->x = 2;

	return a->x;
}



/* While these tests check that incompatible enums can still
 * alias, although this is not required.   */

enum bar { A = 1, B = 3, C = 5, D = 9 };

[[gnu::noinline,gnu::noipa]]
int test_bar1(enum bar* a, void* b)
{
	*a = A;

	enum bar { A = 1, B = 3, C = 6, D = 9 }* p = b;
	*p = B;

	return *a;
}

[[gnu::noinline,gnu::noipa]]
int test_bar2(enum bar* a, void* b)
{
	*a = A;

	enum baX { A = 1, B = 3, C = 5, D = 9 }* p = b;
	*p = B;

	return *a;
}


int main()
{
	struct foo y;

	if (1 != test_foo1(&y, &y))
		__builtin_abort();

	if (1 != test_foo2(&y, &y))
		__builtin_abort();

	enum bar z;

	if (B != test_bar1(&z, &z))
		__builtin_abort();

	if (B != test_bar2(&z, &z))
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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_2:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_fox:[0-9]+]] fox = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar:[0-9]+]] bar = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_B:[0-9]+]] B = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_C:[0-9]+]] C = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE_D:[0-9]+]] D = const<i32>(9);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_bar_2:[0-9]+]] bar = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] A = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_B]] B = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_C]] C = const<i32>(6);
// DEFAULT-NEXT:         %[[VALUE_D]] D = const<i32>(9);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_baX:[0-9]+]] baX = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] A = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_B]] B = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_C]] C = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE_D]] D = const<i32>(9);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %[[VALUE_B]] @test_foo1(%[[VALUE_C]] a: ptr<@type[[TYPE_foo]]>, %[[VALUE_D]] b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_C]]))), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_foo_2]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_foo_2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_D]]));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_foo_2]]>>(%[[VALUE_p]]))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_C]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_foo2:[0-9]+]] @test_foo2(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_foo]]>, %[[VALUE_b:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_a]]))), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_fox]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_fox]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_fox]]>>(%[[VALUE_p_2]]))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_a]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_bar1:[0-9]+]] @test_bar1(%[[VALUE_a_2:[0-9]+]] a: ptr<@type[[TYPE_bar]]>, %[[VALUE_b_2:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<@type[[TYPE_bar]]>(deref(read<ptr<@type[[TYPE_bar]]>>(%[[VALUE_a_2]])), int_to_enum<@type[[TYPE_bar]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_bar_2]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_bar_2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         write<@type[[TYPE_bar_2]]>(deref(read<ptr<@type[[TYPE_bar_2]]>>(%[[VALUE_p_3]])), int_to_enum<@type[[TYPE_bar_2]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_bar]]>(deref(read<ptr<@type[[TYPE_bar]]>>(%[[VALUE_a_2]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_bar2:[0-9]+]] @test_bar2(%[[VALUE_a_3:[0-9]+]] a: ptr<@type[[TYPE_bar]]>, %[[VALUE_b_3:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<@type[[TYPE_bar]]>(deref(read<ptr<@type[[TYPE_bar]]>>(%[[VALUE_a_3]])), int_to_enum<@type[[TYPE_bar]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_p_4:[0-9]+]] p: ptr<@type[[TYPE_baX]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_baX]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:         write<@type[[TYPE_baX]]>(deref(read<ptr<@type[[TYPE_baX]]>>(%[[VALUE_p_4]])), int_to_enum<@type[[TYPE_baX]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_bar]]>(deref(read<ptr<@type[[TYPE_bar]]>>(%[[VALUE_a_3]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: @type[[TYPE_foo]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn(ptr<@type[[TYPE_foo]]>, ptr<void>) -> i32>(%[[VALUE_B]], addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_y]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_y]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn(ptr<@type[[TYPE_foo]]>, ptr<void>) -> i32>(%[[VALUE_test_foo2]], addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_y]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_y]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: @type[[TYPE_bar]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(3), call<i32, signature=fn(ptr<@type[[TYPE_bar]]>, ptr<void>) -> i32>(%[[VALUE_test_bar1]], addr_of<ptr<@type[[TYPE_bar]]>>(%[[VALUE_z]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bar]]>>(%[[VALUE_z]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(3), call<i32, signature=fn(ptr<@type[[TYPE_bar]]>, ptr<void>) -> i32>(%[[VALUE_test_bar2]], addr_of<ptr<@type[[TYPE_bar]]>>(%[[VALUE_z]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bar]]>>(%[[VALUE_z]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
