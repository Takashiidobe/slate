// { dg-do run }
// { dg-require-weak "" }
// On darwin, we use attr-weakref-1-darwin.c.

// This test requires support for undefined weak symbols.  This support
// is not available on the following targets.  The test is skipped rather than
// xfailed to suppress the warning that would otherwise arise.
// { dg-skip-if "" { "hppa*-*-hpux*" "*-*-aix*" "nvptx-*-*" } }
// { dg-skip-if PR119369 { amdgcn-*-* } }

// For kernel modules and static RTPs, the loader treats undefined weak
// symbols in the same way as undefined strong symbols.  The test
// therefore fails to load, so skip it.
// { dg-skip-if "" { "*-*-vxworks*" && nonpic } "*" { "-non-static" } }
// { dg-options "-O2" }
// { dg-additional-options "-Wl,-undefined,dynamic_lookup" { target *-*-darwin* } }
// { dg-additional-options "-Wl,-flat_namespace" { target *-*-darwin[89]* } }
// One subtest doesn't assemble with the Solaris/x86 as (PR ipa/70582)
// { dg-additional-options "-DSOLARIS_X86_AS" { target { x86 && solaris_as } } }
// { dg-additional-sources "attr-weakref-1a.c" }

// Copyright 2005 Free Software Foundation, Inc.
// Contributed by Alexandre Oliva <aoliva@redhat.com>

// Torture test for weakrefs.  The first letter of an identifier
// indicates whether/how it is defined; the second letter indicates
// whether it is part of a variable or function test; the number that
// follows is a test counter, and a letter that may follow enables
// multiple identifiers within the same test (e.g., multiple weakrefs
// or pointers to the same identifier).

// Identifiers starting with W are weakrefs; those with p are
// pointers; those with g are global definitions; those with l are
// local definitions; those with w are expected to be weak undefined
// in the symbol table; those with u are expected to be marked as
// non-weak undefined in the symbol table.

#include <stdlib.h>

#define USED __attribute__((used))

typedef int vtype;

extern vtype wv1;
static vtype Wv1a __attribute__((weakref ("wv1")));
static vtype *pv1a USED = &Wv1a;

vtype gv2;
static vtype Wv2a __attribute__((weakref ("gv2")));
static vtype *pv2a USED = &Wv2a;

#if !defined SOLARIS_X86_AS
static vtype lv3;
static vtype Wv3a __attribute__((weakref ("lv3")));
static vtype *pv3a USED = &Wv3a;
#endif

extern vtype uv4;
static vtype Wv4a __attribute__((weakref ("uv4")));
static vtype *pv4a USED = &Wv4a;
static vtype *pv4 USED = &uv4;

static vtype Wv5a __attribute__((weakref ("uv5")));
static vtype *pv5a USED = &Wv5a;
extern vtype uv5;
static vtype *pv5 USED = &uv5;

static vtype Wv6a __attribute__((weakref ("wv6")));
static vtype *pv6a USED = &Wv6a;
extern vtype wv6;

static vtype Wv7a __attribute__((weakref ("uv7")));
static vtype* USED fv7 (void) {
  return &Wv7a;
}
extern vtype uv7;
static vtype* USED fv7a (void) {
  return &uv7;
}

extern vtype uv8;
static vtype* USED fv8a (void) {
  return &uv8;
}
static vtype Wv8a __attribute__((weakref ("uv8")));
static vtype* USED fv8 (void) {
  return &Wv8a;
}

extern vtype wv9 __attribute__((weak));
static vtype Wv9a __attribute__((weakref ("wv9")));
static vtype *pv9a USED = &Wv9a;

static vtype Wv10a __attribute__((weakref ("Wv10b")));
static vtype Wv10b __attribute__((weakref ("Wv10c")));
static vtype Wv10c __attribute__((weakref ("Wv10d")));
static vtype Wv10d __attribute__((weakref ("wv10")));
extern vtype wv10;

extern vtype wv11;
static vtype Wv11d __attribute__((weakref ("wv11")));
static vtype Wv11c __attribute__((weakref ("Wv11d")));
static vtype Wv11b __attribute__((weakref ("Wv11c")));
static vtype Wv11a __attribute__((weakref ("Wv11b")));

static vtype Wv12 __attribute__((weakref ("wv12")));
extern vtype wv12 __attribute__((weak));

static vtype Wv13 __attribute__((weakref ("wv13")));
extern vtype wv13 __attribute__((weak));

static vtype Wv14a __attribute__((weakref ("wv14")));
static vtype Wv14b __attribute__((weakref ("wv14")));
extern vtype wv14 __attribute__((weak));

typedef void ftype(void);

extern ftype wf1;
static ftype Wf1a __attribute__((weakref ("wf1")));
static ftype *pf1a USED = &Wf1a;
static ftype Wf1c __attribute__((weakref));
extern ftype Wf1c __attribute__((alias ("wf1")));
static ftype *pf1c USED = &Wf1c;

void gf2(void) {}
static ftype Wf2a __attribute__((weakref ("gf2")));
static ftype *pf2a USED = &Wf2a;

static void lf3(void) {}
static ftype Wf3a __attribute__((weakref ("lf3")));
static ftype *pf3a USED = &Wf3a;

extern ftype uf4;
static ftype Wf4a __attribute__((weakref ("uf4")));
static ftype *pf4a USED = &Wf4a;
static ftype *pf4 USED = &uf4;

static ftype Wf5a __attribute__((weakref ("uf5")));
static ftype *pf5a USED = &Wf5a;
extern ftype uf5;
static ftype *pf5 USED = &uf5;

static ftype Wf6a __attribute__((weakref ("wf6")));
static ftype *pf6a USED = &Wf6a;
extern ftype wf6;

static ftype Wf7a __attribute__((weakref ("uf7")));
static ftype* USED ff7 (void) {
  return &Wf7a;
}
extern ftype uf7;
static ftype* USED ff7a (void) {
  return &uf7;
}

extern ftype uf8;
static ftype* USED ff8a (void) {
  return &uf8;
}
static ftype Wf8a __attribute__((weakref ("uf8")));
static ftype* USED ff8 (void) {
  return &Wf8a;
}

extern ftype wf9 __attribute__((weak));
static ftype Wf9a __attribute__((weakref ("wf9")));
static ftype *pf9a USED = &Wf9a;

static ftype Wf10a __attribute__((weakref ("Wf10b")));
static ftype Wf10b __attribute__((weakref ("Wf10c")));
static ftype Wf10c __attribute__((weakref ("Wf10d")));
static ftype Wf10d __attribute__((weakref ("wf10")));
extern ftype wf10;

extern ftype wf11;
static ftype Wf11d __attribute__((weakref ("wf11")));
static ftype Wf11c __attribute__((weakref ("Wf11d")));
static ftype Wf11b __attribute__((weakref ("Wf11c")));
static ftype Wf11a __attribute__((weakref ("Wf11b")));

static ftype Wf12 __attribute__((weakref ("wf12")));
extern ftype wf12 __attribute__((weak));

static ftype Wf13 __attribute__((weakref ("wf13")));
extern ftype wf13 __attribute__((weak));

static ftype Wf14a __attribute__((weakref ("wf14")));
static ftype Wf14b __attribute__((weakref ("wf14")));
extern ftype wf14 __attribute__((weak));

#ifndef __APPLE__
#define chk(p) do { if (!p) abort (); } while (0)
#else
#define chk(p) /* */
#endif

int main () {
  chk (!pv1a);
  chk (pv2a);
#if !defined(SOLARIS_X86_AS)
  chk (pv3a);
#endif
  chk (pv4a);
  chk (pv4);
  chk (pv5a);
  chk (pv5);
  chk (!pv6a);
  chk (fv7 ());
  chk (fv7a ());
  chk (fv8 ());
  chk (fv8a ());
  chk (!pv9a);
  chk (!&Wv10a);
  chk (!&Wv11a);
  chk (!&Wv12);
  chk (!&wv12);
  chk (!&wv13);
  chk (!&Wv14a);

  chk (!pf1a);
  chk (!pf1c);
  chk (pf2a);
  chk (pf3a);
  chk (pf4a);
  chk (pf4);
  chk (pf5a);
  chk (pf5);
  chk (!pf6a);
  chk (ff7 ());
  chk (ff7a ());
  chk (ff8 ());
  chk (ff8a ());
  chk (!pf9a);
  chk (!&Wf10a);
  chk (!&Wf11a);
  chk (!&Wf12);
  chk (!&wf12);
  chk (!&wf13);
  chk (!&Wf14a);

  exit (0);
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
// DEFAULT-NEXT:     type @type[[TYPE_vtype:[0-9]+]] vtype = i32;
// DEFAULT-NEXT:     type @type[[TYPE_ftype:[0-9]+]] ftype = fn() -> void;
// DEFAULT-NEXT:     extern %[[VALUE_wv1:[0-9]+]] wv1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_Wv1a:[0-9]+]] Wv1a: i32 [storage=static] [linkage=internal] [weakref="wv1"];
// DEFAULT-NEXT:     global %[[VALUE_pv1a:[0-9]+]] pv1a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_Wv1a]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_gv2:[0-9]+]] gv2: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_Wv2a:[0-9]+]] Wv2a: i32 [storage=static] [linkage=internal] [weakref="gv2"];
// DEFAULT-NEXT:     global %[[VALUE_pv2a:[0-9]+]] pv2a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_Wv2a]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_lv3:[0-9]+]] lv3: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     extern %[[VALUE_Wv3a:[0-9]+]] Wv3a: i32 [storage=static] [linkage=internal] [weakref="lv3"];
// DEFAULT-NEXT:     global %[[VALUE_pv3a:[0-9]+]] pv3a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_Wv3a]]) [linkage=internal] [used];
// DEFAULT-NEXT:     extern %[[VALUE_uv4:[0-9]+]] uv4: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_Wv4a:[0-9]+]] Wv4a: i32 [storage=static] [linkage=internal] [weakref="uv4"];
// DEFAULT-NEXT:     global %[[VALUE_pv4a:[0-9]+]] pv4a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_Wv4a]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_pv4:[0-9]+]] pv4: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_uv4]]) [linkage=internal] [used];
// DEFAULT-NEXT:     extern %[[VALUE_Wv5a:[0-9]+]] Wv5a: i32 [storage=static] [linkage=internal] [weakref="uv5"];
// DEFAULT-NEXT:     global %[[VALUE_pv5a:[0-9]+]] pv5a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_Wv5a]]) [linkage=internal] [used];
// DEFAULT-NEXT:     extern %[[VALUE_uv5:[0-9]+]] uv5: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pv5:[0-9]+]] pv5: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_uv5]]) [linkage=internal] [used];
// DEFAULT-NEXT:     extern %[[VALUE_Wv6a:[0-9]+]] Wv6a: i32 [storage=static] [linkage=internal] [weakref="wv6"];
// DEFAULT-NEXT:     global %[[VALUE_pv6a:[0-9]+]] pv6a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_Wv6a]]) [linkage=internal] [used];
// DEFAULT-NEXT:     extern %[[VALUE_wv6:[0-9]+]] wv6: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_Wv7a:[0-9]+]] Wv7a: i32 [storage=static] [linkage=internal] [weakref="uv7"];
// DEFAULT-NEXT:     extern %[[VALUE_uv7:[0-9]+]] uv7: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_uv8:[0-9]+]] uv8: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_Wv8a:[0-9]+]] Wv8a: i32 [storage=static] [linkage=internal] [weakref="uv8"];
// DEFAULT-NEXT:     extern %[[VALUE_wv9:[0-9]+]] wv9: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     extern %[[VALUE_Wv9a:[0-9]+]] Wv9a: i32 [storage=static] [linkage=internal] [weakref="wv9"];
// DEFAULT-NEXT:     global %[[VALUE_pv9a:[0-9]+]] pv9a: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_Wv9a]]) [linkage=internal] [used];
// DEFAULT-NEXT:     extern %[[VALUE_Wv10a:[0-9]+]] Wv10a: i32 [storage=static] [linkage=internal] [weakref="Wv10b"];
// DEFAULT-NEXT:     extern %[[VALUE_Wv10b:[0-9]+]] Wv10b: i32 [storage=static] [linkage=internal] [weakref="Wv10c"];
// DEFAULT-NEXT:     extern %[[VALUE_Wv10c:[0-9]+]] Wv10c: i32 [storage=static] [linkage=internal] [weakref="Wv10d"];
// DEFAULT-NEXT:     extern %[[VALUE_Wv10d:[0-9]+]] Wv10d: i32 [storage=static] [linkage=internal] [weakref="wv10"];
// DEFAULT-NEXT:     extern %[[VALUE_wv10:[0-9]+]] wv10: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_wv11:[0-9]+]] wv11: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_Wv11d:[0-9]+]] Wv11d: i32 [storage=static] [linkage=internal] [weakref="wv11"];
// DEFAULT-NEXT:     extern %[[VALUE_Wv11c:[0-9]+]] Wv11c: i32 [storage=static] [linkage=internal] [weakref="Wv11d"];
// DEFAULT-NEXT:     extern %[[VALUE_Wv11b:[0-9]+]] Wv11b: i32 [storage=static] [linkage=internal] [weakref="Wv11c"];
// DEFAULT-NEXT:     extern %[[VALUE_Wv11a:[0-9]+]] Wv11a: i32 [storage=static] [linkage=internal] [weakref="Wv11b"];
// DEFAULT-NEXT:     extern %[[VALUE_Wv12:[0-9]+]] Wv12: i32 [storage=static] [linkage=internal] [weakref="wv12"];
// DEFAULT-NEXT:     extern %[[VALUE_wv12:[0-9]+]] wv12: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     extern %[[VALUE_Wv13:[0-9]+]] Wv13: i32 [storage=static] [linkage=internal] [weakref="wv13"];
// DEFAULT-NEXT:     extern %[[VALUE_wv13:[0-9]+]] wv13: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     extern %[[VALUE_Wv14a:[0-9]+]] Wv14a: i32 [storage=static] [linkage=internal] [weakref="wv14"];
// DEFAULT-NEXT:     extern %[[VALUE_Wv14b:[0-9]+]] Wv14b: i32 [storage=static] [linkage=internal] [weakref="wv14"];
// DEFAULT-NEXT:     extern %[[VALUE_wv14:[0-9]+]] wv14: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_pf1a:[0-9]+]] pf1a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_Wf1a:[0-9]+]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_pf1c:[0-9]+]] pf1c: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_Wf1c:[0-9]+]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_pf2a:[0-9]+]] pf2a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_Wf2a:[0-9]+]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_pf3a:[0-9]+]] pf3a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_Wf3a:[0-9]+]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_pf4a:[0-9]+]] pf4a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_Wf4a:[0-9]+]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_pf4:[0-9]+]] pf4: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_uf4:[0-9]+]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_pf5a:[0-9]+]] pf5a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_Wf5a:[0-9]+]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_pf5:[0-9]+]] pf5: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_uf5:[0-9]+]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_pf6a:[0-9]+]] pf6a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_Wf6a:[0-9]+]]) [linkage=internal] [used];
// DEFAULT-NEXT:     global %[[VALUE_pf9a:[0-9]+]] pf9a: ptr<fn() -> void> [storage=static] = addr_of<ptr<fn() -> void>>(%[[VALUE_Wf9a:[0-9]+]]) [linkage=internal] [used];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fv7:[0-9]+]] @fv7() -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(%[[VALUE_Wv7a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fv7a:[0-9]+]] @fv7a() -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(%[[VALUE_uv7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fv8a:[0-9]+]] @fv8a() -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(%[[VALUE_uv8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fv8:[0-9]+]] @fv8() -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(%[[VALUE_Wv8a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wf1:[0-9]+]] @wf1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_Wf1a]] @Wf1a() -> void [linkage=internal] [weakref="wf1"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf1c]] @Wf1c() -> void [linkage=internal] [weakref="wf1"];
// DEFAULT-NEXT:     fn %[[VALUE_gf2:[0-9]+]] @gf2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Wf2a]] @Wf2a() -> void [linkage=internal] [weakref="gf2"];
// DEFAULT-NEXT:     fn %[[VALUE_lf3:[0-9]+]] @lf3() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Wf3a]] @Wf3a() -> void [linkage=internal] [weakref="lf3"];
// DEFAULT-NEXT:     fn %[[VALUE_uf4]] @uf4() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_Wf4a]] @Wf4a() -> void [linkage=internal] [weakref="uf4"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf5a]] @Wf5a() -> void [linkage=internal] [weakref="uf5"];
// DEFAULT-NEXT:     fn %[[VALUE_uf5]] @uf5() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_Wf6a]] @Wf6a() -> void [linkage=internal] [weakref="wf6"];
// DEFAULT-NEXT:     fn %[[VALUE_wf6:[0-9]+]] @wf6() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_Wf7a:[0-9]+]] @Wf7a() -> void [linkage=internal] [weakref="uf7"];
// DEFAULT-NEXT:     fn %[[VALUE_ff7:[0-9]+]] @ff7() -> ptr<fn() -> void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<fn() -> void>>(%[[VALUE_Wf7a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_uf7:[0-9]+]] @uf7() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ff7a:[0-9]+]] @ff7a() -> ptr<fn() -> void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<fn() -> void>>(%[[VALUE_uf7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_uf8:[0-9]+]] @uf8() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ff8a:[0-9]+]] @ff8a() -> ptr<fn() -> void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<fn() -> void>>(%[[VALUE_uf8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Wf8a:[0-9]+]] @Wf8a() -> void [linkage=internal] [weakref="uf8"];
// DEFAULT-NEXT:     fn %[[VALUE_ff8:[0-9]+]] @ff8() -> ptr<fn() -> void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<fn() -> void>>(%[[VALUE_Wf8a]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wf9:[0-9]+]] @wf9() -> void [linkage=external] [weak];
// DEFAULT-NEXT:     fn %[[VALUE_Wf9a]] @Wf9a() -> void [linkage=internal] [weakref="wf9"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf10a:[0-9]+]] @Wf10a() -> void [linkage=internal] [weakref="Wf10b"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf10b:[0-9]+]] @Wf10b() -> void [linkage=internal] [weakref="Wf10c"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf10c:[0-9]+]] @Wf10c() -> void [linkage=internal] [weakref="Wf10d"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf10d:[0-9]+]] @Wf10d() -> void [linkage=internal] [weakref="wf10"];
// DEFAULT-NEXT:     fn %[[VALUE_wf10:[0-9]+]] @wf10() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_wf11:[0-9]+]] @wf11() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_Wf11d:[0-9]+]] @Wf11d() -> void [linkage=internal] [weakref="wf11"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf11c:[0-9]+]] @Wf11c() -> void [linkage=internal] [weakref="Wf11d"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf11b:[0-9]+]] @Wf11b() -> void [linkage=internal] [weakref="Wf11c"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf11a:[0-9]+]] @Wf11a() -> void [linkage=internal] [weakref="Wf11b"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf12:[0-9]+]] @Wf12() -> void [linkage=internal] [weakref="wf12"];
// DEFAULT-NEXT:     fn %[[VALUE_wf12:[0-9]+]] @wf12() -> void [linkage=external] [weak];
// DEFAULT-NEXT:     fn %[[VALUE_Wf13:[0-9]+]] @Wf13() -> void [linkage=internal] [weakref="wf13"];
// DEFAULT-NEXT:     fn %[[VALUE_wf13:[0-9]+]] @wf13() -> void [linkage=external] [weak];
// DEFAULT-NEXT:     fn %[[VALUE_Wf14a:[0-9]+]] @Wf14a() -> void [linkage=internal] [weakref="wf14"];
// DEFAULT-NEXT:     fn %[[VALUE_Wf14b:[0-9]+]] @Wf14b() -> void [linkage=internal] [weakref="wf14"];
// DEFAULT-NEXT:     fn %[[VALUE_wf14:[0-9]+]] @wf14() -> void [linkage=external] [weak];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_pv1a]]), null<ptr<i32>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_pv2a]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_pv3a]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_pv4a]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_pv4]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_pv5a]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_pv5]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_pv6a]]), null<ptr<i32>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE_fv7]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE_fv7a]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE_fv8]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<i32>>(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE_fv8a]]), null<ptr<i32>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_pv9a]]), null<ptr<i32>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<i32>>(addr_of<ptr<i32>>(%[[VALUE_Wv10a]]), null<ptr<i32>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<i32>>(addr_of<ptr<i32>>(%[[VALUE_Wv11a]]), null<ptr<i32>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<i32>>(addr_of<ptr<i32>>(%[[VALUE_Wv12]]), null<ptr<i32>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<i32>>(addr_of<ptr<i32>>(%[[VALUE_wv12]]), null<ptr<i32>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<i32>>(addr_of<ptr<i32>>(%[[VALUE_wv13]]), null<ptr<i32>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<i32>>(addr_of<ptr<i32>>(%[[VALUE_Wv14a]]), null<ptr<i32>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%[[VALUE_pf1a]]), null<ptr<fn() -> void>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%[[VALUE_pf1c]]), null<ptr<fn() -> void>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%[[VALUE_pf2a]]), null<ptr<fn() -> void>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%[[VALUE_pf3a]]), null<ptr<fn() -> void>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%[[VALUE_pf4a]]), null<ptr<fn() -> void>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%[[VALUE_pf4]]), null<ptr<fn() -> void>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%[[VALUE_pf5a]]), null<ptr<fn() -> void>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%[[VALUE_pf5]]), null<ptr<fn() -> void>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%[[VALUE_pf6a]]), null<ptr<fn() -> void>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<fn() -> void>>(call<ptr<fn() -> void>, signature=fn() -> ptr<fn() -> void>>(%[[VALUE_ff7]]), null<ptr<fn() -> void>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<fn() -> void>>(call<ptr<fn() -> void>, signature=fn() -> ptr<fn() -> void>>(%[[VALUE_ff7a]]), null<ptr<fn() -> void>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<fn() -> void>>(call<ptr<fn() -> void>, signature=fn() -> ptr<fn() -> void>>(%[[VALUE_ff8]]), null<ptr<fn() -> void>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(ne<ptr<fn() -> void>>(call<ptr<fn() -> void>, signature=fn() -> ptr<fn() -> void>>(%[[VALUE_ff8a]]), null<ptr<fn() -> void>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<fn() -> void>>(read<ptr<fn() -> void>>(%[[VALUE_pf9a]]), null<ptr<fn() -> void>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<fn() -> void>>(addr_of<ptr<fn() -> void>>(%[[VALUE_Wf10a]]), null<ptr<fn() -> void>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<fn() -> void>>(addr_of<ptr<fn() -> void>>(%[[VALUE_Wf11a]]), null<ptr<fn() -> void>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<fn() -> void>>(addr_of<ptr<fn() -> void>>(%[[VALUE_Wf12]]), null<ptr<fn() -> void>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<fn() -> void>>(addr_of<ptr<fn() -> void>>(%[[VALUE_wf12]]), null<ptr<fn() -> void>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<fn() -> void>>(addr_of<ptr<fn() -> void>>(%[[VALUE_wf13]]), null<ptr<fn() -> void>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if not<bool>(not<bool>(ne<ptr<fn() -> void>>(addr_of<ptr<fn() -> void>>(%[[VALUE_Wf14a]]), null<ptr<fn() -> void>>)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
