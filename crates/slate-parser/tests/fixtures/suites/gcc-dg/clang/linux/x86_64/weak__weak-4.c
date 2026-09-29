/* { dg-do compile } */
/* { dg-require-weak "" } */
/* { dg-options "-fno-common" } */
/* { dg-skip-if "" { x86_64-*-mingw* } } */
/* NVPTX's definition of weak looks different to normal.  */
/* { dg-skip-if "" { nvptx-*-* } } */
/* { dg-skip-if PR119369 { amdgcn-*-* } } */

/* { dg-final { scan-weak "vfoo1a" } } */
/* { dg-final { scan-weak "vfoo1b" } } */
/* { dg-final { scan-weak "vfoo1c" } } */
/* { dg-final { scan-weak "vfoo1d" } } */
/* { dg-final { scan-weak "vfoo1e" } } */
/* { dg-final { scan-weak "vfoo1f" } } */
/* { dg-final { scan-weak "vfoo1g" } } */
/* { dg-final { scan-weak "vfoo1h" } } */
/* { dg-final { scan-weak "vfoo1i" } } */
/* { dg-final { scan-weak "vfoo1j" } } */
/* { dg-final { scan-weak "vfoo1k" } } */

/* test variable addresses with #pragma weak */

#pragma weak vfoo1a
extern int vfoo1a;
void * foo1a (void)
{
  return (void *)&vfoo1a;
}


extern int vfoo1b;
#pragma weak vfoo1b
void * foo1b (void)
{
  return (void *)&vfoo1b;
}


extern int vfoo1c;
void * foo1c (void)
{
  return (void *)&vfoo1c;
}
#pragma weak vfoo1c


#pragma weak vfoo1d
int vfoo1d;
void * foo1d (void)
{
  return (void *)&vfoo1d;
}


int vfoo1e;
#pragma weak vfoo1e
void * foo1e (void)
{
  return (void *)&vfoo1e;
}


int vfoo1f;
void * foo1f (void)
{
  return (void *)&vfoo1f;
}
#pragma weak vfoo1f


extern int vfoo1g;
void * foo1g (void)
{
  return (void *)&vfoo1g;
}
#pragma weak vfoo1g
int vfoo1g;


extern int vfoo1h;
void * foo1h (void)
{
  return (void *)&vfoo1h;
}
int vfoo1h;
#pragma weak vfoo1h


int vfoo1i;
extern int vfoo1i;
void * foo1i (void)
{
  return (void *)&vfoo1i;
}
#pragma weak vfoo1i


extern int vfoo1j;
int vfoo1j;
void * foo1j (void)
{
  return (void *)&vfoo1j;
}
#pragma weak vfoo1j


#pragma weak vfoo1k
int vfoo1k = 1;

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
// DEFAULT-NEXT:     extern %[[VALUE_vfoo1a:[0-9]+]] vfoo1a: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     extern %[[VALUE_vfoo1b:[0-9]+]] vfoo1b: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     extern %[[VALUE_vfoo1c:[0-9]+]] vfoo1c: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_vfoo1d:[0-9]+]] vfoo1d: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_vfoo1e:[0-9]+]] vfoo1e: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_vfoo1f:[0-9]+]] vfoo1f: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_vfoo1g:[0-9]+]] vfoo1g: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_vfoo1h:[0-9]+]] vfoo1h: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_vfoo1i:[0-9]+]] vfoo1i: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_vfoo1j:[0-9]+]] vfoo1j: i32 [storage=static] [linkage=external] [weak];
// DEFAULT-NEXT:     global %[[VALUE_vfoo1k:[0-9]+]] vfoo1k: i32 [storage=static] = const<i32>(1) [linkage=external] [weak];
// DEFAULT-NEXT:     fn %[[VALUE_foo1a:[0-9]+]] @foo1a() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_vfoo1a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1b:[0-9]+]] @foo1b() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_vfoo1b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1c:[0-9]+]] @foo1c() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_vfoo1c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1d:[0-9]+]] @foo1d() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_vfoo1d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1e:[0-9]+]] @foo1e() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_vfoo1e]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1f:[0-9]+]] @foo1f() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_vfoo1f]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1g:[0-9]+]] @foo1g() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_vfoo1g]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1h:[0-9]+]] @foo1h() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_vfoo1h]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1i:[0-9]+]] @foo1i() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_vfoo1i]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1j:[0-9]+]] @foo1j() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_vfoo1j]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
