/* { dg-do compile } */
/* { dg-options "-O2 -fpic" } */
/* { dg-options "-O2 -fpic -mregparm=3" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */
/* { dg-require-effective-target tls } */
/* { dg-require-effective-target fpic } */

extern __thread int i, j, k;
extern void bar(int *, int *, int *);
void foo(void)
{
  bar(&i, &j, &k);
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
// DEFAULT-NEXT:     extern %[[VALUE_i:[0-9]+]] i: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_j:[0-9]+]] j: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_k:[0-9]+]] k: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, ptr<i32>, ptr<i32>) -> void>(%[[VALUE_bar]], addr_of<ptr<i32>>(%[[VALUE_i]]), addr_of<ptr<i32>>(%[[VALUE_j]]), addr_of<ptr<i32>>(%[[VALUE_k]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
