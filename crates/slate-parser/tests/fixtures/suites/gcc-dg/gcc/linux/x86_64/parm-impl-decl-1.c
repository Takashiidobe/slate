/* Test implicit function declarations and other odd declarations in
   function prototypes.  Bug 18239.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=gnu89" } */

/* Implicit function declaration in attribute in definition (testcase
   from bug).  */
int
foo (int __attribute__ ((__mode__ (vector_size(8)))) i) /* { dg-warning "'mode' attribute ignored" } */
{
  return (long long) i;
}

/* Various other cases.  */

int f (int [sizeof(g())]);
int f1 (int [sizeof(g1())]);

int g () { return 1; }

int
h (int (*p)[sizeof(i())])
{
  int g2 (), g3 ();
  return (*p)[0] + g3() + g2();
}

int i () { return 2; }

int f2 (int [sizeof(g2())]);
int f3 (int [sizeof(g3())]);
int g3 () { return 4; }

// SLATE-FILECHECK-STD DEFAULT gnu89
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32> [array=4]) -> i32 [linkage=external] [c="int(int[4])"];
// DEFAULT-NEXT:     fn %[[VALUE_g1:[0-9]+]] @g1(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE1:[0-9]+]] <unnamed>: ptr<i32> [array=4]) -> i32 [linkage=external] [c="int(int[4])"];
// DEFAULT-NEXT:     fn %[[VALUE_i_2:[0-9]+]] @i(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g2:[0-9]+]] @g2(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_g3:[0-9]+]] @g3(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h:[0-9]+]] @h(%[[VALUE_p:[0-9]+]] p: ptr<array<i32, 4>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(read<ptr<array<i32, 4>>>(%[[VALUE_p]]))), const<i32>(0)))), call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_g3]])), call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_g2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE2:[0-9]+]] <unnamed>: ptr<i32> [array=4]) -> i32 [linkage=external] [c="int(int[4])"];
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE3:[0-9]+]] <unnamed>: ptr<i32> [array=4]) -> i32 [linkage=external] [c="int(int[4])"];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
