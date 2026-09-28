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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 foo = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 bar = enum : u32 {
// DEFAULT-NEXT:         %0 A = const<i32>(1);
// DEFAULT-NEXT:         %1 B = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type3 bar = enum : u32 {
// DEFAULT-NEXT:         %0 A = const<i32>(1);
// DEFAULT-NEXT:         %1 B = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %1 @test_foo(%2 a: ptr<@type0>, %3 b: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%2))), const<i32>(1));
// DEFAULT-NEXT:         let %5 p: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(read<ptr<void>>(%3));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type1>>(%5))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test_bar(%10 a: ptr<@type2>, %11 b: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<@type2>(deref(read<ptr<@type2>>(%10)), int_to_enum<@type2, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %15 p: ptr<@type3> [storage=automatic] = pointer_cast<ptr<@type3>, reason=assign>(read<ptr<void>>(%11));
// DEFAULT-NEXT:         write<@type3>(deref(read<ptr<@type3>>(%15)), int_to_enum<@type3, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type2>(deref(read<ptr<@type2>>(%10)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 y: @type0 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type0>, ptr<void>) -> i32>(%1, addr_of<ptr<@type0>>(%17), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%17))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%19);
// DEFAULT-NEXT:         let %18 z: @type2 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(3), call<i32, signature=fn(ptr<@type2>, ptr<void>) -> i32>(%9, addr_of<ptr<@type2>>(%18), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%18))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%19);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
