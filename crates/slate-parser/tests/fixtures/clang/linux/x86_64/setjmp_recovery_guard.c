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
// DEFAULT-NEXT:     type @type[[TYPE___jmp_buf:[0-9]+]] __jmp_buf = array<i64, 8>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __val: array<u64, 16>;
// DEFAULT-NEXT:     } [size=128, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE___sigset_t:[0-9]+]] __sigset_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE___jmp_buf_tag:[0-9]+]] __jmp_buf_tag = struct {
// DEFAULT-NEXT:         field0 __jmpbuf: array<i64, 8>;
// DEFAULT-NEXT:         field1 __mask_was_saved: i32;
// DEFAULT-NEXT:         field2 __saved_mask: @type[[TYPE0]];
// DEFAULT-NEXT:     } [size=200, align=8, offsets=[0, 64, 72]];
// DEFAULT-NEXT:     type @type[[TYPE_jmp_buf:[0-9]+]] jmp_buf = array<@type[[TYPE___jmp_buf_tag]], 1>;
// DEFAULT-NEXT:     global %[[VALUE_env:[0-9]+]] env: array<@type[[TYPE___jmp_buf_tag]], 1> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_failures:[0-9]+]] failures: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([70, 65, 73, 76, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 97, 115, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([80, 65, 83, 83, 58, 32, 99, 97, 115, 101, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 111, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([102, 97, 105, 108, 117, 114, 101, 115, 58, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE__setjmp:[0-9]+]] @_setjmp(%[[VALUE___env:[0-9]+]] __env: ptr<@type[[TYPE___jmp_buf_tag]]> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_longjmp:[0-9]+]] @longjmp(%[[VALUE___env_2:[0-9]+]] __env: ptr<@type[[TYPE___jmp_buf_tag]]> [array=1], %[[VALUE___val:[0-9]+]] __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_record_failure:[0-9]+]] @record_failure(%[[VALUE_phase:[0-9]+]] phase: ptr<const i8>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_failures]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_failures]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<ptr<const i8>>(%[[VALUE_phase]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_inner_check:[0-9]+]] @inner_check(%[[VALUE_ok:[0-9]+]] ok: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>, i32) -> void>(%[[VALUE_longjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_env]]), const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_run_case:[0-9]+]] @run_case(%[[VALUE_id:[0-9]+]] id: i32, %[[VALUE_should_fail:[0-9]+]] should_fail: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>) -> i32>(%[[VALUE__setjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_env]])), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>) -> void>(%[[VALUE_record_failure]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_inner_check]], from_bool<i32, reason=arg>(not<bool>(ne<i32>(read<i32>(%[[VALUE_should_fail]]), const<i32>(0)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_3]])), read<i32>(%[[VALUE_id]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>) -> i32>(%[[VALUE__setjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_env]])), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             call<void, signature=fn(ptr<const i8>) -> void>(%[[VALUE_record_failure]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:                             continue %[[VALUE2]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32) -> void>(%[[VALUE_run_case]], read<i32>(%[[VALUE_i]]), from_bool<i32, reason=arg>(eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_5]])), read<i32>(%[[VALUE_failures]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
