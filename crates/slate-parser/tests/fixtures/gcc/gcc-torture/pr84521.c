/* { dg-require-effective-target indirect_jumps } */
/* { dg-additional-options "-fomit-frame-pointer -fno-inline" }  */

extern void abort(void);

void broken_longjmp(void *p) { __builtin_longjmp(p, 1); }

volatile int x   = 256;
void *volatile p = (void *)&x;
void *volatile p1;

void test(void) {
  void *buf[5];
  void *volatile q = p;

  if (!__builtin_setjmp(buf))
    broken_longjmp(buf);

  /* Fails if stack pointer corrupted.  */
  if (p != q)
    abort();
}

void test2(void) {
  void *volatile q = p;
  p1               = __builtin_alloca(x);
  test();

  /* Fails if frame pointer corrupted.  */
  if (p != q)
    abort();
}

int main(void) {
  void *volatile q = p;
  test();
  test2();
  /* Fails if stack pointer corrupted.  */
  if (p != q)
    abort();

  return 0;
}


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
// DEFAULT-NEXT:     global %3 x: volatile i32 [storage=static] = const<i32>(256) [linkage=external];
// DEFAULT-NEXT:     global %4 p: volatile ptr<void> [storage=static] = pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<volatile i32>>(%3)) [linkage=external];
// DEFAULT-NEXT:     global %5 p1: volatile ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @broken_longjmp(%2 p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<void>>, i32) -> void>(__builtin_longjmp, pointer_cast<ptr<ptr<void>>, reason=arg>(read<ptr<void>>(%2)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 buf: array<ptr<void>, 5> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %8 q: volatile ptr<void> [storage=automatic] = read<ptr<void>, volatile>(%4);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<ptr<void>>) -> i32>(__builtin_setjmp, array_decay<ptr<ptr<void>>, length=Some(5)>(%7)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<ptr<void>>, length=Some(5)>(%7)));
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>, volatile>(%4), read<ptr<void>, volatile>(%8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 q: volatile ptr<void> [storage=automatic] = read<ptr<void>, volatile>(%4);
// DEFAULT-NEXT:         write<ptr<void>, volatile>(%5, call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_alloca, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32, volatile>(%3)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_alloca, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32, volatile>(%3))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>, volatile>(%4), read<ptr<void>, volatile>(%10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 q: volatile ptr<void> [storage=automatic] = read<ptr<void>, volatile>(%4);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>, volatile>(%4), read<ptr<void>, volatile>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
