#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;
static int     failures          = 0;
static int     teardown_failures = 0;

static void run_test(int i) {
  if (i == 2) {
    longjmp(env, 1);
  }
  printf("ran %d\n", i);
}

int main(void) {
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered %d\n", i);
      continue;
    }
    run_test(i);
    if (i == 3) {
      teardown_failures++;
      continue;
    }
    printf("teardown ok %d\n", i);
  }
  printf("failures=%d teardown_failures=%d\n", failures, teardown_failures);
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
// DEFAULT-NEXT:     type @type0 __jmp_buf = array<i64, 8>;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 __val: array<u64, 16>;
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type2 __sigset_t = @type1;
// DEFAULT-NEXT:     type @type3 __jmp_buf_tag = struct {
// DEFAULT-NEXT:         field0 __jmpbuf: array<i64, 8>;
// DEFAULT-NEXT:         field1 __mask_was_saved: i32;
// DEFAULT-NEXT:         field2 __saved_mask: @type1;
// DEFAULT-NEXT:     } [size=200, align=8, offsets=[0, 64, 72]];
// DEFAULT-NEXT:     type @type4 jmp_buf = array<@type3, 1>;
// DEFAULT-NEXT:     global %8 env: array<@type3, 1> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %9 failures: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %10 teardown_failures: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([114, 97, 110, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([114, 101, 99, 111, 118, 101, 114, 101, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([116, 101, 97, 114, 100, 111, 119, 110, 32, 111, 107, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 34> [storage=static] = code_units<array<i8, 34>>([102, 97, 105, 108, 117, 114, 101, 115, 61, 37, 100, 32, 116, 101, 97, 114, 100, 111, 119, 110, 95, 102, 97, 105, 108, 117, 114, 101, 115, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %5 @_setjmp(%15 __env: ptr<@type3> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @longjmp(%16 __env: ptr<@type3> [array=1], %17 __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @printf(%18 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @run_test(%12 i: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%12), const<i32>(2))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type3>, i32) -> void>(%6, array_decay<ptr<@type3>, length=Some(1)>(%8), const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%7, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%19)), read<i32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %14 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%25));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<@type3>) -> i32>(%5, array_decay<ptr<@type3>, length=Some(1)>(%8)), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %26: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                             let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%9, read<i32>(%27));
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%7, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%21)), read<i32>(%14));
// DEFAULT-NEXT:                             continue %20;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%11, read<i32>(%14));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%14), const<i32>(3))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %28: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                             let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%10, read<i32>(%29));
// DEFAULT-NEXT:                             continue %20;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%7, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%22)), read<i32>(%14));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%7, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(34)>(%23)), read<i32>(%9), read<i32>(%10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
