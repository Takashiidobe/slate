/* { dg-do run } */
/* { dg-options "-std=c2y -O2" } */

struct f2 {
	struct f {
		_Alignas(int) char buf[sizeof(int)];
	} x[2];
	int i;
};

[[gnu::noinline]]
int foo2(struct f2 *p, int *q)
{
	*q = 1;
	*p = (struct f2){ };
	return *q;
}

struct g2 {
	union g {
		_Alignas(int) char buf[sizeof(int)];
	} x[2];
	int i;
};

[[gnu::noinline]]
int bar2(struct g2 *p, int *q)
{
	*q = 1;
	*p = (struct g2){ };
	return *q;
}

int main()
{
	struct f2 p2;
	if (0 != foo2(&p2, (void*)&p2.x[0].buf))
		__builtin_abort();

	struct g2 q2;
	if (0 != bar2(&q2, (void*)&q2.x[0].buf))
		__builtin_abort();
}

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:     type @type[[TYPE_f2:[0-9]+]] f2 = struct {
// DEFAULT-NEXT:         field0 x: array<@type[[TYPE_f:[0-9]+]], 2>;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_f]] f = struct {
// DEFAULT-NEXT:         field0 buf: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_g2:[0-9]+]] g2 = struct {
// DEFAULT-NEXT:         field0 x: array<@type[[TYPE_g:[0-9]+]], 2>;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_g]] g = union {
// DEFAULT-NEXT:         field0 buf: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_f2]]>, %[[VALUE_q:[0-9]+]] q: ptr<i32>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_q]])), const<i32>(1));
// DEFAULT-NEXT:         write<@type[[TYPE_f2]]>(deref(read<ptr<@type[[TYPE_f2]]>>(%[[VALUE_p]])), copy<@type[[TYPE_f2]], reason=assign>(read<@type[[TYPE_f2]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_f2]], zero_fill=true>())));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%[[VALUE_q]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar2:[0-9]+]] @bar2(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_g2]]>, %[[VALUE_q_2:[0-9]+]] q: ptr<i32>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_q_2]])), const<i32>(1));
// DEFAULT-NEXT:         write<@type[[TYPE_g2]]>(deref(read<ptr<@type[[TYPE_g2]]>>(%[[VALUE_p_2]])), copy<@type[[TYPE_g2]], reason=assign>(read<@type[[TYPE_g2]]>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_g2]], zero_fill=true>())));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%[[VALUE_q_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p2:[0-9]+]] p2: @type[[TYPE_f2]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), call<i32, signature=fn(ptr<@type[[TYPE_f2]]>, ptr<i32>) -> i32>(%[[VALUE_foo2]], addr_of<ptr<@type[[TYPE_f2]]>>(%[[VALUE_p2]]), pointer_cast<ptr<i32>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<array<i8, 4>>>(field0(deref(ptr_offset<ptr<@type[[TYPE_f]]>, subtract=false, element=@type[[TYPE_f]], overflow=ub>(array_decay<ptr<@type[[TYPE_f]]>, length=Some(2)>(field0(%[[VALUE_p2]])), const<i32>(0)))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_q2:[0-9]+]] q2: @type[[TYPE_g2]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), call<i32, signature=fn(ptr<@type[[TYPE_g2]]>, ptr<i32>) -> i32>(%[[VALUE_bar2]], addr_of<ptr<@type[[TYPE_g2]]>>(%[[VALUE_q2]]), pointer_cast<ptr<i32>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<array<i8, 4>>>(field0(deref(ptr_offset<ptr<@type[[TYPE_g]]>, subtract=false, element=@type[[TYPE_g]], overflow=ub>(array_decay<ptr<@type[[TYPE_g]]>, length=Some(2)>(field0(%[[VALUE_q2]])), const<i32>(0)))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
