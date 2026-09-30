/* { dg-do run }
 * { dg-options "-std=gnu23 -O2" }
 */


/* This test fails when the bitfield is not marked
   nonaddressable in the composite type.  */

struct foo { int x :3; } x;

[[gnu::noinline,gnu::noipa]]
int test_foo1(struct foo* a, void* b)
{
	a->x = 1;

	struct foo { int x :3; } y;
	typeof(*(1 ? &x : &y)) *z = b;

	z->x = 2;

	return a->x;
}

int main()
{
	struct foo y;

	if (2 != test_foo1(&y, &y))
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
// DEFAULT-NEXT:         field0 x: i32 : 3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_2:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 x: i32 : 3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE_foo]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_foo1:[0-9]+]] @test_foo1(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_foo]]>, %[[VALUE_b:[0-9]+]] b: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_a]]))), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: @type[[TYPE_foo_2]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: ptr<@type[[TYPE_foo]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_foo]]>, reason=assign>(read<ptr<void>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_z]]))), const<i32>(2));
// DEFAULT-NEXT:         return read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_a]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: @type[[TYPE_foo]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(2), call<i32, signature=fn(ptr<@type[[TYPE_foo]]>, ptr<void>) -> i32>(%[[VALUE_test_foo1]], addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_y_2]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_y_2]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
