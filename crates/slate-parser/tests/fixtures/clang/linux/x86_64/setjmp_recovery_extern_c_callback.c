#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;
static int     failures = 0;

static void fail_now(void) { longjmp(env, 1); }

static void check(int ok) {
  if (!ok) {
    fail_now();
  }
  printf("PASS\n");
}

static void run_case(void (*fn)(int), int ok) {
  if (setjmp(env)) {
    failures++;
    printf("FAIL\n");
    return;
  }
  fn(ok);
}

int main(void) {
  void (*fn)(int) = check;
  run_case(fn, 0);
  printf("failures: %d\n", failures);
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
// DEFAULT-NEXT:     global %12 env: array<@type3, 1> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %13 failures: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([80, 65, 83, 83, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([70, 65, 73, 76, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([102, 97, 105, 108, 117, 114, 101, 115, 58, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %6 @_setjmp(%22 __env: ptr<@type3> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @longjmp(%23 __env: ptr<@type3> [array=1], %24 __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @printf(%25 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %14 @fail_now() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%9, array_decay<ptr<@type3>, length=Some(1)>(%12), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @check(%16 ok: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%16), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%26)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @run_case(%18 fn: ptr<fn(i32) -> void>, %19 ok: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>) -> i32>(%6, array_decay<ptr<@type3>, length=Some(1)>(%12)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%30));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%27)));
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(read<ptr<fn(i32) -> void>>(%18), read<i32>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %21 fn: ptr<fn(i32) -> void> [storage=automatic] = function_decay<ptr<fn(i32) -> void>>(%15);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<fn(i32) -> void>, i32) -> void>(%17, read<ptr<fn(i32) -> void>>(%21), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%28)), read<i32>(%13));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
