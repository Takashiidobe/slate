#include <setjmp.h>
#include <stdio.h>

static jmp_buf retry_buf;

static void might_fail(int attempt) {
  if (attempt < 3) {
    longjmp(retry_buf, attempt + 1);
  }
}

int main(void) {
  for (;;) {
    int attempt = setjmp(retry_buf);
    printf("attempt %d\n", attempt);
    if (attempt >= 3) {
      break;
    }
    might_fail(attempt);
  }
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
// DEFAULT-NEXT:     global %[[VALUE_retry_buf:[0-9]+]] retry_buf: array<@type[[TYPE___jmp_buf_tag]], 1> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([97, 116, 116, 101, 109, 112, 116, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE__setjmp:[0-9]+]] @_setjmp(%[[VALUE___env:[0-9]+]] __env: ptr<@type[[TYPE___jmp_buf_tag]]> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_longjmp:[0-9]+]] @longjmp(%[[VALUE___env_2:[0-9]+]] __env: ptr<@type[[TYPE___jmp_buf_tag]]> [array=1], %[[VALUE___val:[0-9]+]] __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_might_fail:[0-9]+]] @might_fail(%[[VALUE_attempt:[0-9]+]] attempt: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_attempt]]), const<i32>(3))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>, i32) -> void>(%[[VALUE_longjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_retry_buf]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_attempt]]), const<i32>(1)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_attempt_2:[0-9]+]] attempt: i32 [storage=automatic] = call<i32, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>) -> i32>(%[[VALUE__setjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_retry_buf]]));
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str]])), read<i32>(%[[VALUE_attempt_2]]));
// DEFAULT-NEXT:                     if ge<i32>(read<i32>(%[[VALUE_attempt_2]]), const<i32>(3))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             break %[[VALUE0]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_might_fail]], read<i32>(%[[VALUE_attempt_2]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
