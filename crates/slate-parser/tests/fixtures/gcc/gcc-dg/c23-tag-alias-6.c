/* { dg-do run }
 * { dg-options "-std=c23 -O2" }
 */


/* These tests check that a composite type for a struct
 * can alias the original definition.  */

struct foo { int (*y)[]; int x; } s;

int test_foo(struct foo* a, void* b)
{
	a->x = 1;

	struct foo { int (*y)[1]; int x; } t;
	typeof(*(1 ? &s: &t)) *p = b;
	p->x = 2;

	return a->x;
}


int main()
{
	struct foo y;

	if (2 != test_foo(&y, &y))
		__builtin_abort();

	return 0;
}


// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:         field0 y: ptr<array<i32, incomplete>>;
// DEFAULT-NEXT:         field1 x: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 foo = struct {
// DEFAULT-NEXT:         field0 y: ptr<array<i32, 1>>;
// DEFAULT-NEXT:         field1 x: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %1 s: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @test_foo(%3 a: ptr<@type0>, %4 b: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type0>>(%3))), const<i32>(1));
// DEFAULT-NEXT:         let %6 t: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %7 p: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=assign>(read<ptr<void>>(%4));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type0>>(%7))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field1(deref(read<ptr<@type0>>(%3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 y: @type0 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type0>, ptr<void>) -> i32>(%2, addr_of<ptr<@type0>>(%9), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
