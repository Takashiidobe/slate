/* { dg-do run } 
 * { dg-options "-std=c23 -O2" } */

[[gnu::noinline]]
void *alias(void *ap, void *bp, void *x, void *y)
{
	struct foo { struct bar *f; } *a = ap;
	struct bar { long x; };

	a->f = x;

	{
		struct bar;
		struct foo { struct bar *f; } *b = bp;
		struct bar { long x; };

		// after completing bar, the two struct foo should be compatible 

		b->f = y;
	}


	return a->f;
}

int main()
{
	struct bar { long x; };
	struct foo { struct bar *f; } a;
	struct bar x, y;
	if (&y != alias(&a, &a, &x, &y))
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
// DEFAULT-NEXT:         field0 f: ptr<@type[[TYPE_bar:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar]] bar = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar_2:[0-9]+]] bar = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_2:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 f: ptr<@type[[TYPE_bar_2]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar_3:[0-9]+]] bar = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_3:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 f: ptr<@type[[TYPE_bar_3]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_alias:[0-9]+]] @alias(%[[VALUE_ap:[0-9]+]] ap: ptr<void>, %[[VALUE_bp:[0-9]+]] bp: ptr<void>, %[[VALUE_x:[0-9]+]] x: ptr<void>, %[[VALUE_y:[0-9]+]] y: ptr<void>) -> ptr<void> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_foo]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_foo]]>, reason=assign>(read<ptr<void>>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_bar]]>>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_a]]))), pointer_cast<ptr<@type[[TYPE_bar]]>, reason=assign>(read<ptr<void>>(%[[VALUE_x]])));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_foo_2]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_foo_2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_bp]]));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_bar_2]]>>(field0(deref(read<ptr<@type[[TYPE_foo_2]]>>(%[[VALUE_b]]))), pointer_cast<ptr<@type[[TYPE_bar_2]]>, reason=assign>(read<ptr<void>>(%[[VALUE_y]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(read<ptr<@type[[TYPE_bar]]>>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_a]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_foo_3]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: @type[[TYPE_bar_3]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: @type[[TYPE_bar_3]] [storage=automatic];
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_bar_3]]>>(addr_of<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_y_2]]), pointer_cast<ptr<@type[[TYPE_bar_3]]>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<void>, ptr<void>, ptr<void>, ptr<void>) -> ptr<void>>(%[[VALUE_alias]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_foo_3]]>>(%[[VALUE_a_2]])), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_foo_3]]>>(%[[VALUE_a_2]])), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_x_2]])), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_bar_3]]>>(%[[VALUE_y_2]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
