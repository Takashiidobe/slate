/* PR middle-end/20303 */
/* Test nesting of #pragma GCC visibility. */
/* { dg-do compile } */
/* { dg-require-visibility "" } */
/* { dg-final { scan-not-hidden "foo00" } } */
/* { dg-final { scan-hidden "foo01" } } */
/* { dg-final { scan-not-hidden "foo02" } } */
/* { dg-final { scan-hidden "foo03" } } */
/* { dg-final { scan-not-hidden "foo04" } } */
/* { dg-final { scan-not-hidden "foo05" } } */
/* { dg-final { scan-not-hidden "foo06" } } */
/* { dg-final { scan-hidden "foo07" } } */
/* { dg-final { scan-not-hidden "foo08" } } */
/* { dg-final { scan-hidden "foo09" } } */
/* { dg-final { scan-not-hidden "foo10" } } */
/* { dg-final { scan-hidden "foo11" } } */
/* { dg-final { scan-hidden "foo12" } } */
/* { dg-final { scan-hidden "foo13" } } */
/* { dg-final { scan-not-hidden "foo14" } } */
/* { dg-final { scan-hidden "foo15" } } */
/* { dg-final { scan-not-hidden "foo16" } } */
/* { dg-final { scan-hidden "foo17" } } */
/* { dg-final { scan-not-hidden "foo18" } } */
/* { dg-final { scan-hidden "foo19" } } */
/* { dg-final { scan-not-hidden "foo20" } } */
/* { dg-final { scan-hidden "foo21" } } */
/* { dg-final { scan-not-hidden "foo22" } } */
/* { dg-final { scan-hidden "foo23" } } */
/* { dg-final { scan-not-hidden "foo24" } } */
/* { dg-final { scan-hidden "foo25" } } */
/* { dg-final { scan-not-hidden "foo26" } } */
/* { dg-final { scan-hidden "foo27" } } */
/* { dg-final { scan-not-hidden "foo28" } } */
/* { dg-final { scan-hidden "foo29" } } */
/* { dg-final { scan-not-hidden "foo30" } } */
/* { dg-final { scan-hidden "foo31" } } */
/* { dg-final { scan-not-hidden "foo32" } } */
/* { dg-final { scan-hidden "foo33" } } */
/* { dg-final { scan-not-hidden "foo34" } } */
/* { dg-final { scan-hidden "foo35" } } */
/* { dg-final { scan-not-hidden "foo36" } } */
/* { dg-final { scan-hidden "foo37" } } */
/* { dg-final { scan-not-hidden "foo38" } } */
/* { dg-final { scan-hidden "foo39" } } */
/* { dg-final { scan-not-hidden "foo40" } } */
/* { dg-final { scan-hidden "foo41" } } */
/* { dg-final { scan-not-hidden "foo42" } } */
/* { dg-final { scan-hidden "foo43" } } */
/* { dg-final { scan-not-hidden "foo44" } } */
/* { dg-final { scan-hidden "foo45" } } */
/* { dg-final { scan-hidden "foo46" } } */
/* { dg-final { scan-hidden "foo47" } } */
/* { dg-final { scan-not-hidden "foo48" } } */
/* { dg-final { scan-hidden "foo49" } } */
/* { dg-final { scan-not-hidden "foo50" } } */
/* { dg-final { scan-hidden "foo51" } } */
/* { dg-final { scan-not-hidden "foo52" } } */
/* { dg-final { scan-not-hidden "foo53" } } */
/* { dg-final { scan-not-hidden "foo54" } } */
/* { dg-final { scan-hidden "foo55" } } */
/* { dg-final { scan-not-hidden "foo56" } } */
/* { dg-final { scan-hidden "foo57" } } */
/* { dg-final { scan-not-hidden "foo58" } } */
/* { dg-final { scan-hidden "foo59" } } */

#pragma GCC visibility push(default)
void foo00();
#pragma GCC visibility push(hidden)
void foo01();
#pragma GCC visibility push(default)
void foo02();
#pragma GCC visibility push(hidden)
void foo03();
#pragma GCC visibility push(default)
void foo04();
#pragma GCC visibility push(default)
void foo05();
#pragma GCC visibility push(default)
void foo06();
#pragma GCC visibility push(hidden)
void foo07();
#pragma GCC visibility push(default)
void foo08();
#pragma GCC visibility push(hidden)
void foo09();
#pragma GCC visibility push(default)
void foo10();
#pragma GCC visibility push(hidden)
void foo11();
#pragma GCC visibility push(hidden)
void foo12();
#pragma GCC visibility push(hidden)
void foo13();
#pragma GCC visibility push(default)
void foo14();
#pragma GCC visibility push(hidden)
void foo15();
#pragma GCC visibility push(default)
void foo16();
#pragma GCC visibility push(hidden)
void foo17();
#pragma GCC visibility push(default)
void foo18();
#pragma GCC visibility push(hidden)
void foo19();
#pragma GCC visibility push(default)
void foo20();
#pragma GCC visibility push(hidden)
void foo21();
#pragma GCC visibility push(default)
void foo22();
#pragma GCC visibility push(hidden)
void foo23();
#pragma GCC visibility push(default)
void foo24();
#pragma GCC visibility push(hidden)
void foo25();
#pragma GCC visibility push(default)
void foo26();
#pragma GCC visibility push(hidden)
void foo27();
#pragma GCC visibility push(default)
void foo28();
#pragma GCC visibility push(hidden)
void foo29();
#pragma GCC visibility pop
void foo30();
#pragma GCC visibility pop
void foo31();
#pragma GCC visibility pop
void foo32();
#pragma GCC visibility pop
void foo33();
#pragma GCC visibility pop
void foo34();
#pragma GCC visibility pop
void foo35();
#pragma GCC visibility pop
void foo36();
#pragma GCC visibility pop
void foo37();
#pragma GCC visibility pop
void foo38();
#pragma GCC visibility pop
void foo39();
#pragma GCC visibility pop
void foo40();
#pragma GCC visibility pop
void foo41();
#pragma GCC visibility pop
void foo42();
#pragma GCC visibility pop
void foo43();
#pragma GCC visibility pop
void foo44();
#pragma GCC visibility pop
void foo45();
#pragma GCC visibility pop
void foo46();
#pragma GCC visibility pop
void foo47();
#pragma GCC visibility pop
void foo48();
#pragma GCC visibility pop
void foo49();
#pragma GCC visibility pop
void foo50();
#pragma GCC visibility pop
void foo51();
#pragma GCC visibility pop
void foo52();
#pragma GCC visibility pop
void foo53();
#pragma GCC visibility pop
void foo54();
#pragma GCC visibility pop
void foo55();
#pragma GCC visibility pop
void foo56();
#pragma GCC visibility pop
void foo57();
#pragma GCC visibility pop
void foo58();
#pragma GCC visibility push (hidden)
void foo59();
#pragma GCC visibility pop
#pragma GCC visibility pop

#define D(N) \
void foo##N##0() { } \
void foo##N##1() { } \
void foo##N##2() { } \
void foo##N##3() { } \
void foo##N##4() { } \
void foo##N##5() { } \
void foo##N##6() { } \
void foo##N##7() { } \
void foo##N##8() { } \
void foo##N##9() { }
D(0)
D(1)
D(2)
D(3)
D(4)
D(5)

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     fn %[[VALUE_foo00:[0-9]+]] @foo00(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo01:[0-9]+]] @foo01(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo02:[0-9]+]] @foo02(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo03:[0-9]+]] @foo03(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo04:[0-9]+]] @foo04(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo05:[0-9]+]] @foo05(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo06:[0-9]+]] @foo06(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo07:[0-9]+]] @foo07(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo08:[0-9]+]] @foo08(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo09:[0-9]+]] @foo09(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo10:[0-9]+]] @foo10(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo11:[0-9]+]] @foo11(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo12:[0-9]+]] @foo12(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo13:[0-9]+]] @foo13(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo14:[0-9]+]] @foo14(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo15:[0-9]+]] @foo15(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo16:[0-9]+]] @foo16(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo17:[0-9]+]] @foo17(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo18:[0-9]+]] @foo18(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo19:[0-9]+]] @foo19(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo20:[0-9]+]] @foo20(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo21:[0-9]+]] @foo21(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo22:[0-9]+]] @foo22(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo23:[0-9]+]] @foo23(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo24:[0-9]+]] @foo24(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo25:[0-9]+]] @foo25(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo26:[0-9]+]] @foo26(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo27:[0-9]+]] @foo27(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo28:[0-9]+]] @foo28(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo29:[0-9]+]] @foo29(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo30:[0-9]+]] @foo30(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo31:[0-9]+]] @foo31(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo32:[0-9]+]] @foo32(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo33:[0-9]+]] @foo33(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo34:[0-9]+]] @foo34(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo35:[0-9]+]] @foo35(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo36:[0-9]+]] @foo36(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo37:[0-9]+]] @foo37(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo38:[0-9]+]] @foo38(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo39:[0-9]+]] @foo39(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo40:[0-9]+]] @foo40(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo41:[0-9]+]] @foo41(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo42:[0-9]+]] @foo42(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo43:[0-9]+]] @foo43(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo44:[0-9]+]] @foo44(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo45:[0-9]+]] @foo45(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo46:[0-9]+]] @foo46(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo47:[0-9]+]] @foo47(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo48:[0-9]+]] @foo48(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo49:[0-9]+]] @foo49(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo50:[0-9]+]] @foo50(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo51:[0-9]+]] @foo51(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo52:[0-9]+]] @foo52(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo53:[0-9]+]] @foo53(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo54:[0-9]+]] @foo54(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo55:[0-9]+]] @foo55(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo56:[0-9]+]] @foo56(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo57:[0-9]+]] @foo57(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo58:[0-9]+]] @foo58(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo59:[0-9]+]] @foo59(unprototyped) -> void [linkage=external] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
