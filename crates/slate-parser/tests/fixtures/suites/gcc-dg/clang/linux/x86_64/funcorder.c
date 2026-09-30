/* { dg-do compile } */
/* { dg-options "-O2 -funit-at-a-time" } */
/* { dg-final { scan-assembler-not "link_error" } } */
/*  In unit-at-time the functions should be assembled in order
    e q t main, so we realize that they are pure.  */
 
void link_error (void);
static int mem;
static int e(void) __attribute__ ((noinline));
static int q(void) __attribute__ ((noinline));
static int t(void) __attribute__ ((noinline));
int
main()
{
	return t();
}
static int t()
{
	int r,e;
	if (mem)
		t();
	e=mem;
	r=q();
	if (e!=mem)
		link_error();
	return r;
}
static int e()
{
	return 0;
}
static int q()
{
	int t=mem,r;
	r=e();
	if (t!=mem)
		link_error();
	return r;
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
// DEFAULT-NEXT:     global %[[VALUE_mem:[0-9]+]] mem: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_link_error:[0-9]+]] @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_e:[0-9]+]] @e() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_q:[0-9]+]] @q() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: i32 [storage=automatic] = read<i32>(%[[VALUE_mem]]);
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], call<i32, signature=fn() -> i32>(%[[VALUE_e]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_t]]), read<i32>(%[[VALUE_mem]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_t_2:[0-9]+]] @t() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_e_2:[0-9]+]] e: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_mem]]), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn() -> i32>(%[[VALUE_t_2]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_e_2]], read<i32>(%[[VALUE_mem]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_2]], call<i32, signature=fn() -> i32>(%[[VALUE_q]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_e_2]]), read<i32>(%[[VALUE_mem]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return call<i32, signature=fn() -> i32>(%[[VALUE_t_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
