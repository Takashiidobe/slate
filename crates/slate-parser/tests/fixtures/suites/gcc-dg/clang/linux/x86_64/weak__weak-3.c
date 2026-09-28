/* { dg-do compile } */
/* { dg-require-alias "" } */
/* { dg-require-weak "" } */
/* { dg-options "-fno-common -Waddress" } */
/* { dg-skip-if "" { x86_64-*-mingw* } } */
/* { dg-skip-if PR119369 { amdgcn-*-* } } */

/* { dg-final { scan-weak "ffoo1a" } } */
/* { dg-final { scan-weak "ffoo1b" } } */
/* { dg-final { scan-weak "ffoo1c" } } */
/* { dg-final { scan-not-weak "ffoo1d" } } */
/* { dg-final { scan-weak "ffoo1e" } } */
/* { dg-final { scan-weak "ffoo1f" } } */
/* { dg-final { scan-weak "ffoo1g" } } */

/* test function addresses with __attribute__((weak)) */

extern void * ffoo1a (void) __attribute__((weak));
extern void * ffoo1a (void);
void * foo1a (void)
{
  return (void *)ffoo1a;
}


extern void * ffoo1b (void);
extern void * ffoo1b (void) __attribute__((weak));
void * foo1b (void)
{
  return (void *)ffoo1b;
}


extern void * ffoo1c (void);  
void * foo1c (void)
{
  return (void *)ffoo1c;
}
extern void * ffoo1c (void) __attribute__((weak));


int ffoo1d (void);
int ffoo1d (void) __attribute__((weak));


extern void * ffoo1e (void);
extern void * ffoo1e (void)  __attribute__((weak));
void * foo1e (void)
{
  if (ffoo1e)
    ffoo1e ();
  return 0;
}


extern void * ffoo1f (void);    
void * foo1f (void)
{
  if (ffoo1f) /* { dg-warning "-Waddress" } */
    ffoo1f ();
  return 0;
}
void * ffoox1f (void) { return (void *)0; }
extern void * ffoo1f (void)  __attribute__((weak, alias ("ffoox1f")));


extern void * ffoo1g (void);
void * ffoox1g (void) { return (void *)0; }
extern void * ffoo1g (void)  __attribute__((weak, alias ("ffoox1g")));
void * foo1g (void)
{
  /* ffoo1g is a weak alias for a symbol defined in this file, expect
     a -Waddress for the test (which is folded to true).  */
  if (ffoo1g)       // { dg-warning "-Waddress" }
    ffoo1g ();
  return 0;
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
// DEFAULT-NEXT:     fn %0 @ffoo1a() -> ptr<void> [linkage=external] [weak];
// DEFAULT-NEXT:     fn %1 @foo1a() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn() -> ptr<void>>>(%0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @ffoo1b() -> ptr<void> [linkage=external] [weak];
// DEFAULT-NEXT:     fn %3 @foo1b() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn() -> ptr<void>>>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @ffoo1c() -> ptr<void> [linkage=external] [weak];
// DEFAULT-NEXT:     fn %5 @foo1c() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn() -> ptr<void>>>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @ffoo1d() -> i32 [linkage=external] [weak];
// DEFAULT-NEXT:     fn %7 @ffoo1e() -> ptr<void> [linkage=external] [weak];
// DEFAULT-NEXT:     fn %8 @foo1e() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<ptr<fn() -> ptr<void>>>(function_decay<ptr<fn() -> ptr<void>>>(%7), null<ptr<fn() -> ptr<void>>>)
// DEFAULT-NEXT:             call<ptr<void>, signature=fn() -> ptr<void>>(%7);
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @ffoo1f() -> ptr<void> [linkage=external] [weak] [alias="ffoox1f"];
// DEFAULT-NEXT:     fn %10 @foo1f() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<ptr<fn() -> ptr<void>>>(function_decay<ptr<fn() -> ptr<void>>>(%9), null<ptr<fn() -> ptr<void>>>)
// DEFAULT-NEXT:             call<ptr<void>, signature=fn() -> ptr<void>>(%9);
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @ffoox1f() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @ffoo1g() -> ptr<void> [linkage=external] [weak] [alias="ffoox1g"];
// DEFAULT-NEXT:     fn %13 @ffoox1g() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @foo1g() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<ptr<fn() -> ptr<void>>>(function_decay<ptr<fn() -> ptr<void>>>(%12), null<ptr<fn() -> ptr<void>>>)
// DEFAULT-NEXT:             call<ptr<void>, signature=fn() -> ptr<void>>(%12);
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
