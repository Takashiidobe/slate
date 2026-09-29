#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_DEPTH 8

static jmp_buf *jb_stack;
static int      jb_top = 0;

#define TRY      if (setjmp(jb_stack[jb_top++]) == 0)
#define CATCH    else
#define THROW(v) longjmp(jb_stack[--jb_top], (v))

static void inner(int fail) {
  if (fail) {
    THROW(42);
  }
}

static void run_case(int id, int fail) {
  TRY {
    inner(fail);
    printf("case %d: no exception\n", id);
  }
  CATCH { printf("case %d: caught\n", id); }
}

int main(void) {
  jb_stack = malloc(sizeof(jmp_buf) * MAX_DEPTH);
  run_case(0, 0);
  run_case(1, 1);
  run_case(2, 0);
  free(jb_stack);
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_jb_stack:[0-9]+]] jb_stack: ptr<array<@type[[TYPE___jmp_buf_tag]], 1>> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_jb_top:[0-9]+]] jb_top: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([99, 97, 115, 101, 32, 37, 100, 58, 32, 110, 111, 32, 101, 120, 99, 101, 112, 116, 105, 111, 110, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([99, 97, 115, 101, 32, 37, 100, 58, 32, 99, 97, 117, 103, 104, 116, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE__setjmp:[0-9]+]] @_setjmp(%[[VALUE___env:[0-9]+]] __env: ptr<@type[[TYPE___jmp_buf_tag]]> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_longjmp:[0-9]+]] @longjmp(%[[VALUE___env_2:[0-9]+]] __env: ptr<@type[[TYPE___jmp_buf_tag]]> [array=1], %[[VALUE___val:[0-9]+]] __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_inner:[0-9]+]] @inner(%[[VALUE_fail:[0-9]+]] fail: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_fail]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_jb_top]]);
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_jb_top]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>, i32) -> void>(%[[VALUE_longjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(deref(ptr_offset<ptr<array<@type[[TYPE___jmp_buf_tag]], 1>>, subtract=false, element=array<@type[[TYPE___jmp_buf_tag]], 1>, overflow=ub>(read<ptr<array<@type[[TYPE___jmp_buf_tag]], 1>>>(%[[VALUE_jb_stack]]), read<i32>(%[[VALUE1]])))), const<i32>(42));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_run_case:[0-9]+]] @run_case(%[[VALUE_id:[0-9]+]] id: i32, %[[VALUE_fail_2:[0-9]+]] fail: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_jb_top]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_jb_top]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>) -> i32>(%[[VALUE__setjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(deref(ptr_offset<ptr<array<@type[[TYPE___jmp_buf_tag]], 1>>, subtract=false, element=array<@type[[TYPE___jmp_buf_tag]], 1>, overflow=ub>(read<ptr<array<@type[[TYPE___jmp_buf_tag]], 1>>>(%[[VALUE_jb_stack]]), read<i32>(%[[VALUE2]]))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_inner]], read<i32>(%[[VALUE_fail_2]]));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str]])), read<i32>(%[[VALUE_id]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_id]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<array<@type[[TYPE___jmp_buf_tag]], 1>>>(%[[VALUE_jb_stack]], pointer_cast<ptr<array<@type[[TYPE___jmp_buf_tag]], 1>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(const<u64>(200), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))));
// DEFAULT-NEXT:         pointer_cast<ptr<array<@type[[TYPE___jmp_buf_tag]], 1>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(const<u64>(200), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_run_case]], const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_run_case]], const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_run_case]], const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<array<@type[[TYPE___jmp_buf_tag]], 1>>>(%[[VALUE_jb_stack]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
