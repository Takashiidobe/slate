/* { dg-do run }
 * { dg-options "-std=gnu23 -O2" }
 */


/* We check that the incompatible enums as fields lead to
 * incompatible types that can be assumed not to alias
 * and that this is exploited during optimization.  */

struct bar1 { int x; enum A1 { X1 = 1 } f; };

[[gnu::noinline,gnu::noipa]]
int test_bar1(struct bar1* a, void* b)
{
	a->x = 1;

	struct bar1 { int x; enum A1 { X1 = 2 } f; }* p = b;
	p->x = 2;

	return a->x;
}


struct bar2 { int x; enum A2 { X2 = 1 } f; };

[[gnu::noinline,gnu::noipa]]
int test_bar2(struct bar2* a, void* b)
{
	a->x = 1;

	struct bar2 { int x; enum B2 { X2 = 1 } f; }* p = b;
	p->x = 2;

	return a->x;
}



struct bar3 { int x; enum A3 { X3 = 1 } f; };

[[gnu::noinline,gnu::noipa]]
int test_bar3(struct bar3* a, void* b)
{
	a->x = 1;

	struct bar3 { int x; enum A3 { Y3 = 1 } f; }* p = b;
	p->x = 2;

	return a->x;
}


struct bar4 { int x; enum { Z4 = 1 } f; };

[[gnu::noinline,gnu::noipa]]
int test_bar4(struct bar4* a, void* b)
{
	a->x = 1;

	struct bar4 { int x; enum { Z4 = 1 } f; }* p = b;
	p->x = 2;

	return a->x;
}



int main()
{
	struct bar1 z1;

	if (1 != test_bar1(&z1, &z1))
		__builtin_abort();

	struct bar2 z2;

	if (1 != test_bar2(&z2, &z2))
		__builtin_abort();

	struct bar3 z3;

	if (1 != test_bar3(&z3, &z3))
		__builtin_abort();

	struct bar4 z4;
#if 0
	// we used to test this, but this would be incorrect
	// if there is a declaration in another TU cf. PR117490
	if (1 != test_bar4(&z4, &z4))
		__builtin_abort();
#endif
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
// DEFAULT-NEXT:     type @type0 bar1 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: @type1;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 A1 = enum : u32 {
// DEFAULT-NEXT:         %0 X1 = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type2 bar1 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: @type3;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type3 A1 = enum : u32 {
// DEFAULT-NEXT:         %0 X1 = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 bar2 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: @type5;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type5 A2 = enum : u32 {
// DEFAULT-NEXT:         %0 X2 = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type6 bar2 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: @type7;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type7 B2 = enum : u32 {
// DEFAULT-NEXT:         %0 X2 = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type8 bar3 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: @type9;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type9 A3 = enum : u32 {
// DEFAULT-NEXT:         %0 X3 = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type10 bar3 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: @type11;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type11 A3 = enum : u32 {
// DEFAULT-NEXT:         %0 Y3 = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type12 bar4 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: @type13;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type13 = enum : u32 {
// DEFAULT-NEXT:         %0 Z4 = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type14 bar4 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: @type15;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type15 = enum : u32 {
// DEFAULT-NEXT:         %0 Z4 = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %3 @test_bar1(%4 a: ptr<@type0>, %5 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%4))), const<i32>(1));
// DEFAULT-NEXT:         let %9 p: ptr<@type2> [storage=automatic] = pointer_cast<ptr<@type2>, reason=assign>(read<ptr<void>>(%5));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type2>>(%9))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type0>>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test_bar2(%14 a: ptr<@type4>, %15 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type4>>(%14))), const<i32>(1));
// DEFAULT-NEXT:         let %19 p: ptr<@type6> [storage=automatic] = pointer_cast<ptr<@type6>, reason=assign>(read<ptr<void>>(%15));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type6>>(%19))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type4>>(%14))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @test_bar3(%24 a: ptr<@type8>, %25 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type8>>(%24))), const<i32>(1));
// DEFAULT-NEXT:         let %29 p: ptr<@type10> [storage=automatic] = pointer_cast<ptr<@type10>, reason=assign>(read<ptr<void>>(%25));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type10>>(%29))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type8>>(%24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @test_bar4(%34 a: ptr<@type12>, %35 b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type12>>(%34))), const<i32>(1));
// DEFAULT-NEXT:         let %39 p: ptr<@type14> [storage=automatic] = pointer_cast<ptr<@type14>, reason=assign>(read<ptr<void>>(%35));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type14>>(%39))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type12>>(%34))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %40 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %41 z1: @type0 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn(ptr<@type0>, ptr<void>) -> i32>(%3, addr_of<ptr<@type0>>(%41), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%41))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%45);
// DEFAULT-NEXT:         let %42 z2: @type4 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn(ptr<@type4>, ptr<void>) -> i32>(%13, addr_of<ptr<@type4>>(%42), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type4>>(%42))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%45);
// DEFAULT-NEXT:         let %43 z3: @type8 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), call<i32, signature=fn(ptr<@type8>, ptr<void>) -> i32>(%23, addr_of<ptr<@type8>>(%43), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type8>>(%43))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%45);
// DEFAULT-NEXT:         let %44 z4: @type12 [storage=automatic];
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
