/* Test for various combinations of const, arrays and typedefs:
   should never actually get const on the final array type, but
   all should end up in a read-only section.  PR c/12165.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-Wno-discarded-array-qualifiers" } */
/* { dg-additional-options "-fno-pie" { target pie } } */
/* The MMIX port always switches to the .data section at the end of a file.  */
/* { dg-final { scan-assembler-not "\\.data(?!\\.rel\\.ro)" { xfail powerpc*-*-aix* mmix-*-* x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler-symbol-section {^_?a$} {^\.(const|rodata|srodata|sdata)|\[RO\]} } } */
static const int a[2] = { 1, 2 };
/* { dg-final { scan-assembler-symbol-section {^_?a1$} {^\.(const|rodata|srodata|sdata)|\[RO\]} } } */
const int a1[2] = { 1, 2 };
typedef const int ci;
/* { dg-final { scan-assembler-symbol-section {^_?b$} {^\.(const|rodata|srodata|sdata)|\[RO\]} } } */
static ci b[2] = { 3, 4 };
/* { dg-final { scan-assembler-symbol-section {^_?b1$} {^\.(const|rodata|srodata|sdata)|\[RO\]} } } */
ci b1[2] = { 3, 4 };
typedef int ia[2];
/* { dg-final { scan-assembler-symbol-section {^_?c$} {^\.(const|rodata|srodata|sdata)|\[RO\]} } } */
static const ia c = { 5, 6 };
/* { dg-final { scan-assembler-symbol-section {^_?c1$} {^\.(const|rodata|srodata|sdata)|\[RO\]} } } */
const ia c1 = { 5, 6 };
typedef const int cia[2];
/* { dg-final { scan-assembler-symbol-section {^_?d$} {^\.(const|rodata|srodata|sdata)|\[RO\]} } } */
static cia d = { 7, 8 };
/* { dg-final { scan-assembler-symbol-section {^_?d1$} {^\.(const|rodata|srodata|sdata)|\[RO\]} } } */
cia d1 = { 7, 8 };
/* { dg-final { scan-assembler-symbol-section {^_?e$} {^\.(const|rodata|srodata|sdata)|\[RO\]} } } */
static cia e[2] = { { 1, 2 }, { 3, 4 } };
/* { dg-final { scan-assembler-symbol-section {^_?e1$} {^\.(const|rodata|srodata|sdata)|\[RO\]} } } */
cia e1[2] = { { 1, 2 }, { 3, 4 } };
/* { dg-final { scan-assembler-symbol-section {^_?p$} {^\.(const|rodata|srodata|sdata|data.rel.ro.local)|\[RW\]} } } */
void *const p = &a;
/* { dg-final { scan-assembler-symbol-section {^_?q$} {^\.(const|rodata|srodata|sdata|data.rel.ro.local)|\[RW\]} } } */
void *const q = &b;
/* { dg-final { scan-assembler-symbol-section {^_?r$} {^\.(const|rodata|srodata|sdata|data.rel.ro.local)|\[RW\]} } } */
void *const r = &c;
/* { dg-final { scan-assembler-symbol-section {^_?s$} {^\.(const|rodata|srodata|sdata|data.rel.ro.local)|\[RW\]} } } */
void *const s = &d;
/* { dg-final { scan-assembler-symbol-section {^_?t$} {^\.(const|rodata|srodata|sdata|data.rel.ro.local)|\[RW\]} } } */
void *const t = &e;
/* { dg-final { scan-assembler-symbol-section {^_?p1$} {^\.(const|rodata|srodata|sdata|data.rel.ro.local)|\[RW\]} } } */
void *const p1 = &a1;
/* { dg-final { scan-assembler-symbol-section {^_?q1$} {^\.(const|rodata|srodata|sdata|data.rel.ro.local)|\[RW\]} } } */
void *const q1 = &b1;
/* { dg-final { scan-assembler-symbol-section {^_?r1$} {^\.(const|rodata|srodata|sdata|data.rel.ro.local)|\[RW\]} } } */
void *const r1 = &c1;
/* { dg-final { scan-assembler-symbol-section {^_?s1$} {^\.(const|rodata|srodata|sdata|data.rel.ro.local)|\[RW\]} } } */
void *const s1 = &d1;
/* { dg-final { scan-assembler-symbol-section {^_?t1$} {^\.(const|rodata|srodata|sdata|data.rel.ro.local)|\[RW\]} } } */
void *const t1 = &e1;

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 ci = i32;
// DEFAULT-NEXT:     type @type1 ia = array<i32, 2>;
// DEFAULT-NEXT:     type @type2 cia = array<i32, 2>;
// DEFAULT-NEXT:     global %0 a: array<i32, 2> [storage=static] [const] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=internal];
// DEFAULT-NEXT:     global %1 a1: array<i32, 2> [storage=static] [const] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %3 b: array<i32, 2> [storage=static] [const] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4)) [linkage=internal];
// DEFAULT-NEXT:     global %4 b1: array<i32, 2> [storage=static] [const] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4)) [linkage=external];
// DEFAULT-NEXT:     global %6 c: array<i32, 2> [storage=static] [const] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(5), index1 = const<i32>(6)) [linkage=internal];
// DEFAULT-NEXT:     global %7 c1: array<i32, 2> [storage=static] [const] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(5), index1 = const<i32>(6)) [linkage=external];
// DEFAULT-NEXT:     global %9 d: array<i32, 2> [storage=static] [const] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(7), index1 = const<i32>(8)) [linkage=internal];
// DEFAULT-NEXT:     global %10 d1: array<i32, 2> [storage=static] [const] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(7), index1 = const<i32>(8)) [linkage=external];
// DEFAULT-NEXT:     global %11 e: array<array<i32, 2>, 2> [storage=static] [const] [align=16] = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4))) [linkage=internal];
// DEFAULT-NEXT:     global %12 e1: array<array<i32, 2>, 2> [storage=static] [const] [align=16] = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4))) [linkage=external];
// DEFAULT-NEXT:     global %13 p: ptr<void> [storage=static] [const] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<const array<i32, 2>>>(%0)) [linkage=external];
// DEFAULT-NEXT:     global %14 q: ptr<void> [storage=static] [const] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<const array<i32, 2>>>(%3)) [linkage=external];
// DEFAULT-NEXT:     global %15 r: ptr<void> [storage=static] [const] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<const array<i32, 2>>>(%6)) [linkage=external];
// DEFAULT-NEXT:     global %16 s: ptr<void> [storage=static] [const] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<const array<i32, 2>>>(%9)) [linkage=external];
// DEFAULT-NEXT:     global %17 t: ptr<void> [storage=static] [const] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<const array<array<i32, 2>, 2>>>(%11)) [linkage=external];
// DEFAULT-NEXT:     global %18 p1: ptr<void> [storage=static] [const] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<const array<i32, 2>>>(%1)) [linkage=external];
// DEFAULT-NEXT:     global %19 q1: ptr<void> [storage=static] [const] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<const array<i32, 2>>>(%4)) [linkage=external];
// DEFAULT-NEXT:     global %20 r1: ptr<void> [storage=static] [const] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<const array<i32, 2>>>(%7)) [linkage=external];
// DEFAULT-NEXT:     global %21 s1: ptr<void> [storage=static] [const] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<const array<i32, 2>>>(%10)) [linkage=external];
// DEFAULT-NEXT:     global %22 t1: ptr<void> [storage=static] [const] = pointer_cast<ptr<void>, reason=assign>(addr_of<ptr<const array<array<i32, 2>, 2>>>(%12)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
