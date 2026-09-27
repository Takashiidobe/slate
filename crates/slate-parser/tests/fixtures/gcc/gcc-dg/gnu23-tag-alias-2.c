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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 foo = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 fox = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 bar = enum : u32 {
// DEFAULT-NEXT:         %0 A = const<i32>(1);
// DEFAULT-NEXT:         %1 B = const<i32>(3);
// DEFAULT-NEXT:         %2 C = const<i32>(5);
// DEFAULT-NEXT:         %3 D = const<i32>(9);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 bar = enum : u32 {
// DEFAULT-NEXT:         %0 A = const<i32>(1);
// DEFAULT-NEXT:         %1 B = const<i32>(3);
// DEFAULT-NEXT:         %2 C = const<i32>(6);
// DEFAULT-NEXT:         %3 D = const<i32>(9);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type5 baX = enum : u32 {
// DEFAULT-NEXT:         %0 A = const<i32>(1);
// DEFAULT-NEXT:         %1 B = const<i32>(3);
// DEFAULT-NEXT:         %2 C = const<i32>(5);
// DEFAULT-NEXT:         %3 D = const<i32>(9);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %1 @test_foo1(%2 a: ptr<@type0>, %3 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%2))), const<i32>(1));
// DEFAULT-NEXT:         let %5 p: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(read<ptr<void>>(%3));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type1>>(%5))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_foo2(%7 a: ptr<@type0>, %8 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%7))), const<i32>(1));
// DEFAULT-NEXT:         let %10 p: ptr<@type2> [storage=automatic] = pointer_cast<ptr<@type2>, reason=assign>(read<ptr<void>>(%8));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type2>>(%10))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type0>>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test_bar1(%17 a: ptr<@type3>, %18 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<@type3>(deref(read<ptr<@type3>>(%17)), int_to_enum<@type3, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %24 p: ptr<@type4> [storage=automatic] = pointer_cast<ptr<@type4>, reason=assign>(read<ptr<void>>(%18));
// DEFAULT-NEXT:         write<@type4>(deref(read<ptr<@type4>>(%24)), int_to_enum<@type4, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type3>(deref(read<ptr<@type3>>(%17)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test_bar2(%26 a: ptr<@type3>, %27 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<@type3>(deref(read<ptr<@type3>>(%26)), int_to_enum<@type3, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %33 p: ptr<@type5> [storage=automatic] = pointer_cast<ptr<@type5>, reason=assign>(read<ptr<void>>(%27));
// DEFAULT-NEXT:         write<@type5>(deref(read<ptr<@type5>>(%33)), int_to_enum<@type5, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type3>(deref(read<ptr<@type3>>(%26)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %34 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %35 y: @type0 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn(ptr<@type0>, ptr<void>) -> i32>(%1, addr_of<ptr<@type0>>(%35), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%35))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%37);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn(ptr<@type0>, ptr<void>) -> i32>(%6, addr_of<ptr<@type0>>(%35), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%35))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%37);
// DEFAULT-NEXT:         let %36 z: @type3 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(3), call<i32, signature=fn(ptr<@type3>, ptr<void>) -> i32>(%16, addr_of<ptr<@type3>>(%36), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type3>>(%36))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%37);
// DEFAULT-NEXT:         if ne<i32>(const<i32>(3), call<i32, signature=fn(ptr<@type3>, ptr<void>) -> i32>(%25, addr_of<ptr<@type3>>(%36), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type3>>(%36))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%37);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
