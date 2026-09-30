/* { dg-do run }
 * { dg-options "-std=c23 -O2" }
 */


/* These tests check that redefinitions of tagged
   types can alias the original definitions.  */

struct foo { int x; };

int test_foo(struct foo* a, void* b)
{
	a->x = 1;

	struct foo { int x; }* p = b;
	p->x = 2;

	return a->x;
}


enum bar { A = 1, B = 3 };

int test_bar(enum bar* a, void* b)
{
	*a = A;

	enum bar { A = 1, B = 3 }* p = b;
	*p = B;

	return *a;
}


int main()
{
	struct foo y;

	if (2 != test_foo(&y, &y))
		__builtin_abort();

	enum bar z;

	if (B != test_bar(&z, &z))
		__builtin_abort();

	return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_2:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar:[0-9]+]] bar = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_B:[0-9]+]] B = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_bar_2:[0-9]+]] bar = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A]] A = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_B]] B = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %[[VALUE_B]] @test_foo(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_foo]]>, %[[VALUE_b:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_a]]))), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_foo_2]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_foo_2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_foo_2]]>>(%[[VALUE_p]]))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_a]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_bar:[0-9]+]] @test_bar(%[[VALUE_a_2:[0-9]+]] a: ptr<@type[[TYPE_bar]]>, %[[VALUE_b_2:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<@type[[TYPE_bar]]>(deref(read<ptr<@type[[TYPE_bar]]>>(%[[VALUE_a_2]])), int_to_enum<@type[[TYPE_bar]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_bar_2]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_bar_2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         write<@type[[TYPE_bar_2]]>(deref(read<ptr<@type[[TYPE_bar_2]]>>(%[[VALUE_p_2]])), int_to_enum<@type[[TYPE_bar_2]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_bar]]>(deref(read<ptr<@type[[TYPE_bar]]>>(%[[VALUE_a_2]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: @type[[TYPE_foo]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type[[TYPE_foo]]>, ptr<void>) -> i32>(%[[VALUE_B]], addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_y]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_y]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: @type[[TYPE_bar]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(3), call<i32, signature=fn(ptr<@type[[TYPE_bar]]>, ptr<void>) -> i32>(%[[VALUE_test_bar]], addr_of<ptr<@type[[TYPE_bar]]>>(%[[VALUE_z]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bar]]>>(%[[VALUE_z]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
