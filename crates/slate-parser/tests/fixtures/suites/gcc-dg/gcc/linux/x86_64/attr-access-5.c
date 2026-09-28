/* { dg-do compile }
   { dg-options "-fdump-tree-gimple" } */

__attribute__ ((aligned (32)))
__attribute__ ((access (write_only, 2, 1)))
void f (int n, void *p)
{
  __builtin_memset (p, 0, n);
}

/* Verify the DECL_ATTRIBUTE "aligned" is mentioned:
   { dg-final { scan-tree-dump "__attribute__\\(\\(aligned" "gimple" } }
   and the TYPE_ATTRIBUTE "access" is also mentioned:
   { dg-final { scan-tree-dump "__attribute__\\(\\(access" "gimple" } }
   and the function signature including its return type is mentioned:
   { dg-final { scan-tree-dump "void f *\\(int n, void *\\* *p\\)" "gimple" } } */

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
// DEFAULT-NEXT:     fn %6 @__builtin_memset(%3 <unnamed>: ptr<void>, %4 <unnamed>: i32, %5 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %0 @f(%1 n: i32, %2 p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, read<ptr<void>>(%2), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
