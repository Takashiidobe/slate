/* { dg-do run }
 * { dg-options "-std=c23 -O2" }
 */


/* Here we check that structs with flexible array
 * members can alias a compatible redefinition.  */

struct bar { int x; int f[]; };

int test_bar1(struct bar* a, void* b)
{
	a->x = 1;

	struct bar { int x; int f[]; }* p = b;
	struct bar* q = a;
	p->x = 2;

	return a->x;
}

int main()
{
	struct bar z;

	if (2 != test_bar1(&z, &z))
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
// DEFAULT-NEXT:     type @type0 bar = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: array<i32, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 bar = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 f: array<i32, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %1 @test_bar1(%2 a: ptr<@type0>, %3 b: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%2))), const<i32>(1));
// DEFAULT-NEXT:         let %5 p: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(read<ptr<void>>(%3));
// DEFAULT-NEXT:         let %6 q: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=assign>(read<ptr<@type0>>(%2));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type1>>(%5))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 z: @type0 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type0>, ptr<void>) -> i32>(%1, addr_of<ptr<@type0>>(%8), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
