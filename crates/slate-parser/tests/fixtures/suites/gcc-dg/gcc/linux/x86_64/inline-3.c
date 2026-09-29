/* { dg-options "-O2 -funit-at-a-time" } */
/* { dg-final { scan-assembler-not "big_function_2" } } */

int t(void);
static void
big_function_2(void);
void
big_function_1()
{
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	big_function_2();
}
static void
big_function_2()
{
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
	while (t());
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
// DEFAULT-NEXT:     fn %[[VALUE_t:[0-9]+]] @t() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_big_function_2:[0-9]+]] @big_function_2() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE2:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE3:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE4:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE5:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE6:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE7:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE8:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE9:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE10:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE11:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE12:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE13:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE14:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE15:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE16:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_big_function_1:[0-9]+]] @big_function_1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %[[VALUE17:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE18:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE19:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE20:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE21:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE22:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE23:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE24:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE25:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE26:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE27:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE28:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE29:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE30:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE31:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE32:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %[[VALUE33:[0-9]+]] ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_big_function_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
