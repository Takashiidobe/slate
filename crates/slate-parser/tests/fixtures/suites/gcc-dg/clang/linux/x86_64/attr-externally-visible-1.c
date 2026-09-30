/* { dg-do compile } */
/* { dg-options "-O3 -fwhole-program" } */
/* { dg-final { scan-assembler "foo1" } } */
/* { dg-final { scan-assembler "foo2" } } */
/* { dg-final { scan-assembler "foo3" } } */
/* { dg-final { scan-assembler "foo4" } } */
/* { dg-final { scan-assembler "foo5" } } */
/* { dg-final { scan-assembler-not "foo6" } } */
/* { dg-final { scan-assembler "bar1" } } */
/* { dg-final { scan-assembler "bar2" } } */
/* { dg-final { scan-assembler "bar3" } } */
/* { dg-final { scan-assembler "bar4" } } */
/* { dg-final { scan-assembler "bar5" } } */
/* { dg-final { scan-assembler-not "bar6" } } */

extern void foo1 (void) __attribute__((externally_visible));
void foo1 (void) { }

extern void foo2 (void) __attribute__((externally_visible));
__attribute__((externally_visible)) void foo2 (void) { }

extern void foo3 (void);
__attribute__((externally_visible)) void foo3 (void) { }

__attribute__((externally_visible)) void foo4 (void) { }

void foo5 (void) { }
extern void foo5 (void) __attribute__((externally_visible));

void foo6 (void) { }

extern char *bar1 __attribute__((externally_visible));
char *bar1;

extern char *bar2 __attribute__((externally_visible));
char *bar2 __attribute__((externally_visible));

extern char *bar3;
char *bar3 __attribute__((externally_visible));

char *bar4 __attribute__((externally_visible));

char *bar5;
extern char *bar5 __attribute__((externally_visible));

char *bar6;

int main (void) { }

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
// DEFAULT-NEXT:     global %[[VALUE_bar1:[0-9]+]] bar1: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bar2:[0-9]+]] bar2: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bar3:[0-9]+]] bar3: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bar4:[0-9]+]] bar4: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bar5:[0-9]+]] bar5: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bar6:[0-9]+]] bar6: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo1:[0-9]+]] @foo1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo3:[0-9]+]] @foo3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo4:[0-9]+]] @foo4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo5:[0-9]+]] @foo5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo6:[0-9]+]] @foo6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
