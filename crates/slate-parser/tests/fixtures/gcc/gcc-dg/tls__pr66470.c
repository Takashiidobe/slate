/* PR target/66470 */
/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target tls } */

extern __thread unsigned long long a[10];
extern __thread struct S { int a, b; } b[10];

unsigned long long
foo (long x)
{
  return a[x];
}

struct S
bar (long x)
{
  return b[x];
}

#ifdef __SIZEOF_INT128__
extern __thread unsigned __int128 c[10];

unsigned __int128
baz (long x)
{
  return c[x];
}
#endif

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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     extern %0 a: array<u64, 10> [storage=thread] [align=16] [linkage=external];
// DEFAULT-NEXT:     extern %2 b: array<@type0, 10> [storage=thread] [align=16] [linkage=external];
// DEFAULT-NEXT:     extern %7 c: array<u128, 10> [storage=thread] [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 x: i64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(10)>(%0), read<i64>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 x: i64) -> @type0 [linkage=external] [abi=sysv64(scalar) -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(10)>(%2), read<i64>(%6)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @baz(%9 x: i64) -> u128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u128>(deref(ptr_offset<ptr<u128>, subtract=false, element=u128, overflow=ub>(array_decay<ptr<u128>, length=Some(10)>(%7), read<i64>(%9))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
