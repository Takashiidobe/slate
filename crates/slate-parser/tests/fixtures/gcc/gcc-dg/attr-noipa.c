/* Test the noipa attribute.  */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

static inline int __attribute__((noipa))
fn1 (void) /* { dg-warning "inline function \[^\n\]* given attribute 'noinline'" "" } */
{
  return 1;
}

/* Verify the function is not inlined into its caller.  */

static __attribute__((noipa)) int
fn2 (int x, int y)
{
  return x + y;
}

int
fn3 (int x)
{
  return fn2 (x, 0);
}

/* { dg-final { scan-tree-dump "= fn2 \\(" "optimized" } } */

void fn4 (char *);

/* Verify the function is not cloned.  */

__attribute__((__noipa__)) static int
fn5 (int x, int y)
{
  char *p = __builtin_alloca (x + y);
  fn4 (p);
  return x + y;
}

int
fn6 (int x)
{
  return fn5 (x, 2);
}

/* { dg-final { scan-tree-dump "= fn5 \\(" "optimized" } } */
/* { dg-final { scan-tree-dump-not "fn5\\.constprop" "optimized" } } */

/* Verify we still remove unused function calls, even if they have
   noipa attribute.  */

static void fn7 (void) __attribute__((noipa));
static void
fn7 (void)
{
}

/* { dg-final { scan-tree-dump-not "fn7 \\(" "optimized" } } */

/* Verify noipa functions are not ICF optimized.  */

static __attribute__((noipa)) int
fn8 (int x)
{
  return x + 12;
}

static __attribute__((noipa)) int
fn9 (int x)
{
  return x + 12;
}

int
fn10 (int x)
{
  return fn8 (x) + fn9 (x);
}

/* { dg-final { scan-tree-dump "fn8 \\(int" "optimized" } } */
/* { dg-final { scan-tree-dump "fn9 \\(int" "optimized" } } */

/* Verify IPA-VRP is not performed.  */

void fn11 (void);

static int __attribute__((noipa))
fn12 (int x)
{
  if (x < 6 || x >= 29)
    fn11 ();
}

void
fn13 (int x)
{
  fn12 (6 + (x & 15));
}

/* { dg-final { scan-tree-dump "fn11 \\(\\)" "optimized" } } */

void fn14 (void);

__attribute__((noipa)) static int
fn15 (int x)
{
  return x & 7;
}

int
fn16 (int x)
{
  x = fn15 (x);
  if (x < 0 || x >= 7)
    fn14 ();
}

/* { dg-final { scan-tree-dump "fn14 \\(\\)" "optimized" } } */

/* Verify IPA BIT CP is not performed.  */

void fn17 (void);

__attribute__((noipa)) static int
fn18 (int x)
{
  if (x & 8)
    fn17 ();
}

void
fn19 (void)
{
  fn18 (1);
  fn18 (2);
  fn18 (4);
  fn18 (16);
  fn18 (32);
  fn18 (64);
}

/* { dg-final { scan-tree-dump "fn17 \\(\\)" "optimized" } } */

/* Ensure pure/const discovery is not performed.  */

int var1;
void fn20 (void);

__attribute__((noipa)) static int
fn21 (int x, int y)
{
  return x * y;
}

int
fn22 (void)
{
  var1 = 7;
  asm volatile ("" : "+g" (var1) : : "memory");
  int a = var1;
  int b = fn21 (a, a);
  if (a != var1)
    fn20 ();
  return b;
}

/* { dg-final { scan-tree-dump "fn20 \\(\\)" "optimized" } } */

/* Verify IPA alignment propagation is not performed.  */

static __attribute__ ((aligned(16))) char var2[32];
void fn23 (void);

__attribute__((noipa)) static void
fn24 (char *p)
{
  if ((((__UINTPTR_TYPE__) p) & 15) != 0)
    fn23 ();
  asm ("");
}

void
fn25 (void)
{
  fn24 (var2);
  fn24 (var2 + 16);
}

/* { dg-final { scan-tree-dump "fn20 \\(\\)" "optimized" } } */

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
// DEFAULT-NEXT:     global %34 var1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %42 var2: array<i8, 32> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @fn1() -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1 @fn2(%2 x: i32, %3 y: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%2), read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @fn3(%5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%1, read<i32>(%5), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @fn4(%47 <unnamed>: ptr<i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %49 @__builtin_alloca(%48 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @fn5(%8 x: i32, %9 y: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%49, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(read<i32>(%8), read<i32>(%9))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%6, read<ptr<i8>>(%10));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%8), read<i32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @fn6(%12 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%7, read<i32>(%12), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @fn7() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @fn8(%15 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%15), const<i32>(12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @fn9(%17 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%17), const<i32>(12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @fn10(%19 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%14, read<i32>(%19)), call<i32, signature=fn(i32) -> i32>(%16, read<i32>(%19)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @fn11() -> void [linkage=external];
// DEFAULT-NEXT:     fn %21 @fn12(%22 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%22), const<i32>(6)), ge<i32>(read<i32>(%22), const<i32>(29)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @fn13(%24 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%21, add<i32, overflow=ub>(const<i32>(6), and<i32>(read<i32>(%24), const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @fn14() -> void [linkage=external];
// DEFAULT-NEXT:     fn %26 @fn15(%27 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(read<i32>(%27), const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @fn16(%29 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%29, call<i32, signature=fn(i32) -> i32>(%26, read<i32>(%29)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%26, read<i32>(%29));
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%29), const<i32>(0)), ge<i32>(read<i32>(%29), const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @fn17() -> void [linkage=external];
// DEFAULT-NEXT:     fn %31 @fn18(%32 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(and<i32>(read<i32>(%32), const<i32>(8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%30);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @fn19() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%31, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%31, const<i32>(2));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%31, const<i32>(4));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%31, const<i32>(16));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%31, const<i32>(32));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%31, const<i32>(64));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @fn20() -> void [linkage=external];
// DEFAULT-NEXT:     fn %36 @fn21(%37 x: i32, %38 y: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%37), read<i32>(%38));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @fn22() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%34, const<i32>(7));
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             inlateout 0 "g" [reg | mem | imm | sym] -> reg width 32 place<i32>(%34);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %40 a: i32 [storage=automatic] = read<i32>(%34);
// DEFAULT-NEXT:         let %41 b: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%36, read<i32>(%40), read<i32>(%40));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%40), read<i32>(%34))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%35);
// DEFAULT-NEXT:         return read<i32>(%41);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @fn23() -> void [linkage=external];
// DEFAULT-NEXT:     fn %44 @fn24(%45 p: ptr<i8>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u64>(and<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%45)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(15)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%43);
// DEFAULT-NEXT:         asm "" [dialect=att] [options=nostack];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @fn25() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%44, array_decay<ptr<i8>, length=Some(32)>(%42));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%44, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(32)>(%42), const<i32>(16)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
