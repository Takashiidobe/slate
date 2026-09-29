#include <setjmp.h>
#include <stdio.h>

enum Status { STATUS_OK, STATUS_FAIL };
typedef enum Status Processor(int x);
typedef void        Callback(int x);

struct Dispatcher {
  Processor *run;
};

static jmp_buf   env;
static int       failures = 0;
static Callback *g_callback;

static enum Status risky(int x) {
  if (x == 3) {
    longjmp(env, 1);
  }
  return STATUS_OK;
}

static void panicky_callback(int x) {
  if (x == 2) {
    longjmp(env, 1);
  }
  printf("callback %d\n", x);
}

static enum Status content_like(int x) {
  g_callback(x);
  return STATUS_OK;
}

int main(void) {
  struct Dispatcher d;
  g_callback = panicky_callback;

  d.run = risky;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered risky %d\n", i);
      continue;
    }
    d.run(i);
  }

  d.run = content_like;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered content_like %d\n", i);
      continue;
    }
    d.run(i);
  }

  printf("failures=%d\n", failures);
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
// DEFAULT-NEXT:     type @type[[TYPE_Status:[0-9]+]] Status = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_STATUS_OK:[0-9]+]] STATUS_OK = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_STATUS_FAIL:[0-9]+]] STATUS_FAIL = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Processor:[0-9]+]] Processor = fn(i32) -> @type[[TYPE_Status]];
// DEFAULT-NEXT:     type @type[[TYPE_Callback:[0-9]+]] Callback = fn(i32) -> void;
// DEFAULT-NEXT:     type @type[[TYPE_Dispatcher:[0-9]+]] Dispatcher = struct {
// DEFAULT-NEXT:         field0 run: ptr<fn(i32) -> @type[[TYPE_Status]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_env:[0-9]+]] env: array<@type[[TYPE___jmp_buf_tag]], 1> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_failures:[0-9]+]] failures: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_g_callback:[0-9]+]] g_callback: ptr<fn(i32) -> void> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([99, 97, 108, 108, 98, 97, 99, 107, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([114, 101, 99, 111, 118, 101, 114, 101, 100, 32, 114, 105, 115, 107, 121, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([114, 101, 99, 111, 118, 101, 114, 101, 100, 32, 99, 111, 110, 116, 101, 110, 116, 95, 108, 105, 107, 101, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 97, 105, 108, 117, 114, 101, 115, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE__setjmp:[0-9]+]] @_setjmp(%[[VALUE___env:[0-9]+]] __env: ptr<@type[[TYPE___jmp_buf_tag]]> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_longjmp:[0-9]+]] @longjmp(%[[VALUE___env_2:[0-9]+]] __env: ptr<@type[[TYPE___jmp_buf_tag]]> [array=1], %[[VALUE___val:[0-9]+]] __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_risky:[0-9]+]] @risky(%[[VALUE_x:[0-9]+]] x: i32) -> @type[[TYPE_Status]] [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(3))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>, i32) -> void>(%[[VALUE_longjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_env]]), const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return int_to_enum<@type[[TYPE_Status]], reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_panicky_callback:[0-9]+]] @panicky_callback(%[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(2))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>, i32) -> void>(%[[VALUE_longjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_env]]), const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), read<i32>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_content_like:[0-9]+]] @content_like(%[[VALUE_x_3:[0-9]+]] x: i32) -> @type[[TYPE_Status]] [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(read<ptr<fn(i32) -> void>>(%[[VALUE_g_callback]]), read<i32>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         return int_to_enum<@type[[TYPE_Status]], reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: @type[[TYPE_Dispatcher]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(i32) -> void>>(%[[VALUE_g_callback]], function_decay<ptr<fn(i32) -> void>>(%[[VALUE_panicky_callback]]));
// DEFAULT-NEXT:         write<ptr<fn(i32) -> @type[[TYPE_Status]]>>(field0(%[[VALUE_d]]), function_decay<ptr<fn(i32) -> @type[[TYPE_Status]]>>(%[[VALUE_risky]]));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>) -> i32>(%[[VALUE__setjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_env]])), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_failures]]);
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_failures]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(20)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                             continue %[[VALUE0]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<@type[[TYPE_Status]], signature=fn(i32) -> @type[[TYPE_Status]]>(read<ptr<fn(i32) -> @type[[TYPE_Status]]>>(field0(%[[VALUE_d]])), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<ptr<fn(i32) -> @type[[TYPE_Status]]>>(field0(%[[VALUE_d]]), function_decay<ptr<fn(i32) -> @type[[TYPE_Status]]>>(%[[VALUE_content_like]]));
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE___jmp_buf_tag]]>) -> i32>(%[[VALUE__setjmp]], array_decay<ptr<@type[[TYPE___jmp_buf_tag]]>, length=Some(1)>(%[[VALUE_env]])), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_failures]]);
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_failures]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(27)>(%[[VALUE_str_3]])), read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:                             continue %[[VALUE5]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<@type[[TYPE_Status]], signature=fn(i32) -> @type[[TYPE_Status]]>(read<ptr<fn(i32) -> @type[[TYPE_Status]]>>(field0(%[[VALUE_d]])), read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_4]])), read<i32>(%[[VALUE_failures]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
