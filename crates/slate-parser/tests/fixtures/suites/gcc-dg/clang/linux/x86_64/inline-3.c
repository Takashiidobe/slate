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
// DEFAULT-NEXT:     fn %0 @t() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @big_function_2() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %20 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %21 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %22 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %23 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %24 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %25 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %26 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %27 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %28 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %29 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %30 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %31 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %32 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %33 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %34 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %35 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %36 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @big_function_1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %3 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %4 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %5 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %6 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %7 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %8 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %9 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %10 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %11 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %12 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %13 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %14 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %15 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %16 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %17 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %18 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         while %19 ne<i32>(call<i32, signature=fn() -> i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
