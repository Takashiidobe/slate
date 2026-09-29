/* { dg-do compile } */
/* { dg-require-effective-target fpic } */
/* { dg-options "-O2 -fpic -ftls-model=global-dynamic" } */
/* { dg-require-effective-target tls } */

extern __thread long e1;
extern __thread int e2;
static __thread long s1;
static __thread int s2;

long *ae1 (void)
{
  return &e1;
}

int *ae2 (void)
{
  return &e2;
}

long *as1 (void)
{
  return &s1;
}

int *as2 (void)
{
  return &s2;
}

long ge1 (void)
{
  return e1;
}

int ge2 (void)
{
  return e2;
}

long gs1 (void)
{
  return s1;
}

int gs2 (void)
{
  return s2;
}

long ge3 (void)
{
  return e1 + e2;
}

long gs3 (void)
{
  return s1 + s2;
}

long ge4 (void)
{
  if (0)
    return e1;
  return e2;
}

long gs4 (void)
{
  if (0)
    return s1;
  return s2;
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
// DEFAULT-NEXT:     extern %[[VALUE_e1:[0-9]+]] e1: i64 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_e2:[0-9]+]] e2: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: i64 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_s2:[0-9]+]] s2: i32 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ae1:[0-9]+]] @ae1() -> ptr<i64> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i64>>(%[[VALUE_e1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ae2:[0-9]+]] @ae2() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(%[[VALUE_e2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_as1:[0-9]+]] @as1() -> ptr<i64> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i64>>(%[[VALUE_s1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_as2:[0-9]+]] @as2() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(%[[VALUE_s2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge1:[0-9]+]] @ge1() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_e1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge2:[0-9]+]] @ge2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_e2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gs1:[0-9]+]] @gs1() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_s1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gs2:[0-9]+]] @gs2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_s2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge3:[0-9]+]] @ge3() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i64, overflow=ub>(read<i64>(%[[VALUE_e1]]), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_e2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gs3:[0-9]+]] @gs3() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i64, overflow=ub>(read<i64>(%[[VALUE_s1]]), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_s2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ge4:[0-9]+]] @ge4() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             return read<i64>(%[[VALUE_e1]]);
// DEFAULT-NEXT:         return widen<i64, reason=return>(read<i32>(%[[VALUE_e2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gs4:[0-9]+]] @gs4() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             return read<i64>(%[[VALUE_s1]]);
// DEFAULT-NEXT:         return widen<i64, reason=return>(read<i32>(%[[VALUE_s2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
