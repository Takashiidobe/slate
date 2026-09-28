// { dg-do link }
// { dg-skip-if "" { "powerpc-ibm-aix*" } }
// { dg-require-alias "" }
// { dg-options "-O2 -fno-common" }

// Copyright 2005 Free Software Foundation, Inc.
// Contributed by Alexandre Oliva <aoliva@redhat.com>

// PR middle-end/24295

// The unit-at-a-time call graph code used to fail to emit variables
// without external linkage that were only used indirectly, through
// aliases.  Although the PR above is about #pragma weak-introduced
// aliases, the underlying machinery is the same.

#ifndef ATTRIBUTE_USED
# define ATTRIBUTE_USED __attribute__((used))
#endif

static int lv1;
extern int Av1a __attribute__((alias ("lv1")));
int *pv1a = &Av1a;

static int lv2;
extern int Av2a __attribute__((alias ("lv2")));
int *pv2a = &lv2;

static int lv3;
extern int Av3a __attribute__((alias ("lv3")));
static int *pv3a ATTRIBUTE_USED = &Av3a;

static int lv4;
extern int Av4a __attribute__((alias ("lv4")));
static int *pv4a = &Av4a;

typedef void ftype(void);

static void lf1(void) {}
extern ftype Af1a __attribute__((alias ("lf1")));
ftype *pf1a = &Af1a;

static void lf2(void) {}
extern ftype Af2a __attribute__((alias ("lf2")));
ftype *pf2a = &Af2a;

static void lf3(void) {}
extern ftype Af3a __attribute__((alias ("lf3")));
static ftype *pf3a ATTRIBUTE_USED = &Af3a;

static void lf4(void) {}
extern ftype Af4a __attribute__((alias ("lf4")));
static ftype *pf4a = &Af4a;

int
main() {
#ifdef __mips
  /* Use real asm for MIPS, to stop the assembler warning about
     orphaned high-part relocations.  */
  asm volatile ("lw $2,%0\n\tlw $2,%1" : : "m" (pv4a), "m" (pf4a) : "$2");
#else
  asm volatile ("" : : "m" (pv4a), "m" (pf4a));
#endif
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fno-common
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
// DEFAULT-NEXT:     type @type0 ftype = fn() -> void;
// DEFAULT-NEXT:     global %0 lv1: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %1 Av1a: i32 [storage=static] [linkage=external] [alias="lv1"];
// DEFAULT-NEXT:     global %2 pv1a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%1) [linkage=external];
// DEFAULT-NEXT:     global %3 lv2: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %4 Av2a: i32 [storage=static] [linkage=external] [alias="lv2"];
// DEFAULT-NEXT:     global %5 pv2a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%3) [linkage=external];
// DEFAULT-NEXT:     global %6 lv3: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %7 Av3a: i32 [storage=static] [linkage=external] [alias="lv3"];
// DEFAULT-NEXT:     global %8 pv3a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%7) [linkage=internal] [used];
// DEFAULT-NEXT:     global %9 lv4: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %10 Av4a: i32 [storage=static] [linkage=external] [alias="lv4"];
// DEFAULT-NEXT:     global %11 pv4a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%10) [linkage=internal];
// DEFAULT-NEXT:     global %15 pf1a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%14) [linkage=external];
// DEFAULT-NEXT:     global %18 pf2a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%17) [linkage=external];
// DEFAULT-NEXT:     global %21 pf3a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%20) [linkage=internal] [used];
// DEFAULT-NEXT:     global %24 pf4a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%23) [linkage=internal];
// DEFAULT-NEXT:     fn %13 @lf1() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @Af1a() -> void [linkage=external] [alias="lf1"];
// DEFAULT-NEXT:     fn %16 @lf2() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @Af2a() -> void [linkage=external] [alias="lf2"];
// DEFAULT-NEXT:     fn %19 @lf3() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @Af3a() -> void [linkage=external] [alias="lf3"];
// DEFAULT-NEXT:     fn %22 @lf4() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @Af4a() -> void [linkage=external] [alias="lf4"];
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=readonly,nostack] {
// DEFAULT-NEXT:             in 0 "m" [mem] width 64 place<ptr<i32>>(%11);
// DEFAULT-NEXT:             in 1 "m" [mem] width 64 place<ptr<fn() -> void>>(%24);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
