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
// DEFAULT-NEXT:     type @type[[TYPE_p:[0-9]+]] p = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_pd_t:[0-9]+]] pd_t = @type[[TYPE_p]];
// DEFAULT-NEXT:     type @type[[TYPE_p_2:[0-9]+]] p = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_p_3:[0-9]+]] p = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_p_4:[0-9]+]] p = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_p2_t:[0-9]+]] p2_t = @type[[TYPE_p]];
// DEFAULT-NEXT:     type @type[[TYPE_q:[0-9]+]] q = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_y0:[0-9]+]] y0: @type[[TYPE_p]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: @type[[TYPE_p_2]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE_p]]>(%[[VALUE_y0]], copy<@type[[TYPE_p]], reason=assign>(read<@type[[TYPE_p_2]]>(%[[VALUE_x]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: @type[[TYPE_p_3]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y0_2:[0-9]+]] y0: @type[[TYPE_p_3]] [storage=automatic] = copy<@type[[TYPE_p_3]], reason=assign>(read<@type[[TYPE_p_3]]>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_3:[0-9]+]] x: @type[[TYPE_p_4]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y0_3:[0-9]+]] y0: @type[[TYPE_p]] [storage=automatic] = copy<@type[[TYPE_p]], reason=assign>(read<@type[[TYPE_p_4]]>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x_4:[0-9]+]] x: @type[[TYPE_p]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y0_4:[0-9]+]] y0: @type[[TYPE_p]] [storage=automatic] = copy<@type[[TYPE_p]], reason=assign>(read<@type[[TYPE_p]]>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_q]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_q]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE_q]]>(%[[VALUE_a]], copy<@type[[TYPE_q]], reason=assign>(read<@type[[TYPE_q]]>(%[[VALUE_b]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
