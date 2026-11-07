/* Test for -Wtraditional warnings on label conflicts with identifiers.
   Note, gcc should omit these warnings in system header files.
   Origin: Kaveh R. Ghazi <ghazi@caip.rutgers.edu> 8/24/2000.  */
/* { dg-do compile } */
/* { dg-options "-Wno-old-style-definition -Wtraditional" } */

enum foo1 { a };
struct foo2 { int i; };
union foo3 { int j; };
int foo4;
typedef int foo5;

void
testfunc1 (foo6)
     int foo6;
{
  int foo7;

 foo1:
 foo2:
 foo3:
 foo4: /* { dg-warning "traditional C lacks" "label conflicts with identifier" } */
 foo5: /* { dg-warning "traditional C lacks" "label conflicts with identifier" } */
 foo6: /* { dg-warning "traditional C lacks" "label conflicts with identifier" } */
 foo7: /* { dg-warning "traditional C lacks" "label conflicts with identifier" } */
 testfunc1: /* { dg-warning "traditional C lacks" "label conflicts with identifier" } */
 a: /* { dg-warning "traditional C lacks" "label conflicts with identifier" } */
 i:
 j: ;
}
  
# 32 "sys-header.h" 3
/* We are in system headers now, no -Wtraditional warnings should issue.  */

void
testfunc2 (foo6)
     int foo6;
{
  int foo7;

 foo1:
 foo2:
 foo3:
 foo4:
 foo5:
 foo6:
 foo7:
 testfunc2:
 a:
 i:
 j: ;
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
// DEFAULT-NEXT:     type @type[[TYPE_foo1:[0-9]+]] foo1 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_a:[0-9]+]] a = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_foo2:[0-9]+]] foo2 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo3:[0-9]+]] foo3 = union {
// DEFAULT-NEXT:         field0 j: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_foo5:[0-9]+]] foo5 = i32;
// DEFAULT-NEXT:     global %[[VALUE_foo4:[0-9]+]] foo4: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_testfunc1:[0-9]+]] @testfunc1(%[[VALUE_foo6:[0-9]+]] foo6: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_foo7:[0-9]+]] foo7: i32 [storage=automatic];
// DEFAULT-NEXT:         label %[[VALUE_foo1:[0-9]+]] foo1:
// DEFAULT-NEXT:             label %[[VALUE_foo2:[0-9]+]] foo2:
// DEFAULT-NEXT:                 label %[[VALUE_foo3:[0-9]+]] foo3:
// DEFAULT-NEXT:                     label %[[VALUE_foo4_2:[0-9]+]] foo4:
// DEFAULT-NEXT:                         label %[[VALUE_foo5:[0-9]+]] foo5:
// DEFAULT-NEXT:                             label %[[VALUE_foo6_2:[0-9]+]] foo6:
// DEFAULT-NEXT:                                 label %[[VALUE_foo7_2:[0-9]+]] foo7:
// DEFAULT-NEXT:                                     label %[[VALUE_testfunc1_2:[0-9]+]] testfunc1:
// DEFAULT-NEXT:                                         label %[[VALUE_a_2:[0-9]+]] a:
// DEFAULT-NEXT:                                             label %[[VALUE_i:[0-9]+]] i:
// DEFAULT-NEXT:                                                 label %[[VALUE_j:[0-9]+]] j:
// DEFAULT-NEXT:                                                     ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testfunc2:[0-9]+]] @testfunc2(%[[VALUE_foo6_3:[0-9]+]] foo6: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_foo7_3:[0-9]+]] foo7: i32 [storage=automatic];
// DEFAULT-NEXT:         label %[[VALUE_foo1_2:[0-9]+]] foo1:
// DEFAULT-NEXT:             label %[[VALUE_foo2_2:[0-9]+]] foo2:
// DEFAULT-NEXT:                 label %[[VALUE_foo3_2:[0-9]+]] foo3:
// DEFAULT-NEXT:                     label %[[VALUE_foo4_3:[0-9]+]] foo4:
// DEFAULT-NEXT:                         label %[[VALUE_foo5_2:[0-9]+]] foo5:
// DEFAULT-NEXT:                             label %[[VALUE_foo6_4:[0-9]+]] foo6:
// DEFAULT-NEXT:                                 label %[[VALUE_foo7_4:[0-9]+]] foo7:
// DEFAULT-NEXT:                                     label %[[VALUE_testfunc2_2:[0-9]+]] testfunc2:
// DEFAULT-NEXT:                                         label %[[VALUE_a_3:[0-9]+]] a:
// DEFAULT-NEXT:                                             label %[[VALUE_i_2:[0-9]+]] i:
// DEFAULT-NEXT:                                                 label %[[VALUE_j_2:[0-9]+]] j:
// DEFAULT-NEXT:                                                     ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
