#include <setjmp.h>
#include <stdio.h>

typedef void Callback(int x);
typedef void Dispatch(int x, int y);

struct Ctx {
  Dispatch *run;
};

static jmp_buf   env;
static int       failures = 0;
static Callback *g_callback;

static void quiet_callback(int x) { printf("quiet %d\n", x); }

static void panicky_callback(int x) {
  if (x == 2) {
    longjmp(env, 1);
  }
  printf("panicky %d\n", x);
}

static void dispatcher(int x, int y) {
  (void)y;
  g_callback(x);
}

int main(void) {
  struct Ctx c;
  c.run = dispatcher;

  g_callback = quiet_callback;
  for (int i = 0; i < 2; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered quiet %d\n", i);
      continue;
    }
    c.run(i, 0);
  }

  g_callback = panicky_callback;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered panicky %d\n", i);
      continue;
    }
    c.run(i, 0);
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
// DEFAULT-NEXT:     type @type5 Callback = fn(i32) -> void;
// DEFAULT-NEXT:     type @type6 Dispatch = fn(i32, i32) -> void;
// DEFAULT-NEXT:     type @type7 Ctx = struct {
// DEFAULT-NEXT:         field0 run: ptr<fn(i32, i32) -> void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %18 env: array<@type3, 1> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %19 failures: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %20 g_callback: ptr<fn(i32) -> void> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([113, 117, 105, 101, 116, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([112, 97, 110, 105, 99, 107, 121, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([114, 101, 99, 111, 118, 101, 114, 101, 100, 32, 113, 117, 105, 101, 116, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([114, 101, 99, 111, 118, 101, 114, 101, 100, 32, 112, 97, 110, 105, 99, 107, 121, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 97, 105, 108, 117, 114, 101, 115, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %6 @_setjmp(%32 __env: ptr<@type3> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @longjmp(%33 __env: ptr<@type3> [array=1], %34 __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @printf(%35 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @quiet_callback(%22 x: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%36)), read<i32>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @panicky_callback(%24 x: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%24), const<i32>(2))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type3>, i32) -> void>(%9, array_decay<ptr<@type3>, length=Some(1)>(%18), const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%37)), read<i32>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @dispatcher(%26 x: i32, %27 y: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<i32>(%27);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(read<ptr<fn(i32) -> void>>(%20), read<i32>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %29 c: @type7 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(i32, i32) -> void>>(field0(%29), function_decay<ptr<fn(i32, i32) -> void>>(%25));
// DEFAULT-NEXT:         write<ptr<fn(i32) -> void>>(%20, function_decay<ptr<fn(i32) -> void>>(%21));
// DEFAULT-NEXT:         for %38
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %30 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%30), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %43: i32 [synthetic] = read<i32>(%30);
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%30, read<i32>(%44));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<@type3>) -> i32>(%6, array_decay<ptr<@type3>, length=Some(1)>(%18)), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %45: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                             let %46: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%45), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%19, read<i32>(%46));
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(20)>(%39)), read<i32>(%30));
// DEFAULT-NEXT:                             continue %38;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32) -> void>(read<ptr<fn(i32, i32) -> void>>(field0(%29)), read<i32>(%30), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<ptr<fn(i32) -> void>>(%20, function_decay<ptr<fn(i32) -> void>>(%23));
// DEFAULT-NEXT:         for %40
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %31 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%31), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %47: i32 [synthetic] = read<i32>(%31);
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%47), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%31, read<i32>(%48));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<@type3>) -> i32>(%6, array_decay<ptr<@type3>, length=Some(1)>(%18)), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %49: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                             let %50: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%49), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%19, read<i32>(%50));
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%41)), read<i32>(%31));
// DEFAULT-NEXT:                             continue %40;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32) -> void>(read<ptr<fn(i32, i32) -> void>>(field0(%29)), read<i32>(%31), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%42)), read<i32>(%19));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
