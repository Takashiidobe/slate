/* { dg-do compile }
 * { dg-options "-std=c23" }
 */

// compatibility of structs in assignment

typedef struct p { int a; } pd_t;

void test1(void)
{
  pd_t y0;
  struct p { int a; } x;
  y0 = x;
}

void test2(void)
{
  struct p { int a; } x;
  struct p y0 = x;
}

void test3(void)
{
  struct p { int a; } x;
  pd_t y0 = x;
}

typedef struct p { int a; } p2_t;

void test4(void)
{
  p2_t x;
  pd_t y0 = x;
}

void test5(void)
{
  struct q { int a; } a;
  struct q { int a; } b;
  a = b;
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
// DEFAULT-NEXT:     type @type0 p = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 pd_t = @type0;
// DEFAULT-NEXT:     type @type2 p = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 p = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type4 p = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type5 p2_t = @type0;
// DEFAULT-NEXT:     type @type6 q = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %2 @test1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 y0: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %5 x: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<@type0>(%3, copy<@type0, reason=assign>(read<@type2>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 x: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %9 y0: @type3 [storage=automatic] = copy<@type3, reason=assign>(read<@type3>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 x: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %13 y0: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type4>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %17 y0: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20 a: @type6 [storage=automatic];
// DEFAULT-NEXT:         let %21 b: @type6 [storage=automatic];
// DEFAULT-NEXT:         write<@type6>(%20, copy<@type6, reason=assign>(read<@type6>(%21)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
