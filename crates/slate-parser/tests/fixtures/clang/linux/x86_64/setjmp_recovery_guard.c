#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;
static int     failures = 0;

static void record_failure(const char *phase) {
  failures++;
  printf("FAIL: %s\n", phase);
}

static void inner_check(int ok) {
  if (!ok) {
    longjmp(env, 1);
  }
}

static void run_case(int id, int should_fail) {
  if (setjmp(env)) {
    record_failure("case");
    return;
  }
  inner_check(!should_fail);
  printf("PASS: case %d\n", id);
}

int main(void) {
  for (int i = 0; i < 4; i++) {
    if (setjmp(env)) {
      record_failure("loop");
      continue;
    }
    run_case(i, i == 2);
  }
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
// DEFAULT-NEXT:     global %27 .str27: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([70, 65, 73, 76, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 97, 115, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([80, 65, 83, 83, 58, 32, 99, 97, 115, 101, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 111, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([102, 97, 105, 108, 117, 114, 101, 115, 58, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %6 @_setjmp(%23 __env: ptr<@type3> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @longjmp(%24 __env: ptr<@type3> [array=1], %25 __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @printf(%26 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %14 @record_failure(%15 phase: ptr<const i8>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %33: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%34));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%27)), read<ptr<const i8>>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @inner_check(%17 ok: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%17), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type3>, i32) -> void>(%9, array_decay<ptr<@type3>, length=Some(1)>(%12), const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @run_case(%19 id: i32, %20 should_fail: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>) -> i32>(%6, array_decay<ptr<@type3>, length=Some(1)>(%12)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>) -> void>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%28)));
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%16, from_bool<i32, reason=arg>(not<bool>(ne<i32>(read<i32>(%20), const<i32>(0)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%29)), read<i32>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %30
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %22 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%22), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = read<i32>(%22);
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%22, read<i32>(%36));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<@type3>) -> i32>(%6, array_decay<ptr<@type3>, length=Some(1)>(%12)), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<const i8>) -> void>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%31)));
// DEFAULT-NEXT:                             continue %30;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32) -> void>(%18, read<i32>(%22), from_bool<i32, reason=arg>(eq<i32>(read<i32>(%22), const<i32>(2))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%32)), read<i32>(%13));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
