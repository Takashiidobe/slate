// SLATE-FILECHECK-DEFINES DEFAULT

static int minimum(int a, int b)
{
	if(a < b)
		return a;
	else
		return b;
}
static int a, b;
static inline int foo(void)
{
	a = minimum (a, b);
	return 0;
}
static int bar(void)
{
	return foo();
}

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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_minimum:[0-9]+]] @minimum(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]))
// DEFAULT-NEXT:             return read<i32>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return read<i32>(%[[VALUE_b_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_minimum]], read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(%[[VALUE_foo]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
