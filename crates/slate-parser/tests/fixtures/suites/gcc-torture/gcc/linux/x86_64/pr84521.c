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
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: volatile i32 [storage=static] = const<i32>(256) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: volatile ptr<void> [storage=static] = pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<volatile i32>>(%[[VALUE_x]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p1:[0-9]+]] p1: volatile ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_longjmp:[0-9]+]] @__builtin_longjmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_broken_longjmp:[0-9]+]] @broken_longjmp(%[[VALUE_p_2:[0-9]+]] p: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i32) -> void>(%[[VALUE___builtin_longjmp]], read<ptr<void>>(%[[VALUE_p_2]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_setjmp:[0-9]+]] @__builtin_setjmp(%[[VALUE2:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<ptr<void>, 5> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: volatile ptr<void> [storage=automatic] = read<ptr<void>, volatile>(%[[VALUE_p]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE___builtin_setjmp]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<ptr<void>>, length=Some(5)>(%[[VALUE_buf]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_broken_longjmp]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<ptr<void>>, length=Some(5)>(%[[VALUE_buf]])));
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>, volatile>(%[[VALUE_p]]), read<ptr<void>, volatile>(%[[VALUE_q]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_alloca:[0-9]+]] @__builtin_alloca(%[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q_2:[0-9]+]] q: volatile ptr<void> [storage=automatic] = read<ptr<void>, volatile>(%[[VALUE_p]]);
// DEFAULT-NEXT:         write<ptr<void>, volatile>(%[[VALUE_p1]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32, volatile>(%[[VALUE_x]])))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_alloca]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32, volatile>(%[[VALUE_x]]))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test]]);
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>, volatile>(%[[VALUE_p]]), read<ptr<void>, volatile>(%[[VALUE_q_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_q_3:[0-9]+]] q: volatile ptr<void> [storage=automatic] = read<ptr<void>, volatile>(%[[VALUE_p]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test2]]);
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>, volatile>(%[[VALUE_p]]), read<ptr<void>, volatile>(%[[VALUE_q_3]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
