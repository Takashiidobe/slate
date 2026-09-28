/* { dg-require-effective-target indirect_jumps } */
#include <setjmp.h>

extern void abort(void);
extern void exit(int);

static jmp_buf env;

void baz(void) { __asm__ volatile("" : : : "memory"); }

static inline int g(int x) {
  if (x) {
    baz();
    return 0;
  } else {
    baz();
    return 1;
  }
}

int f(int *e) {
  if (*e)
    return 1;

  int x = setjmp(env);
  int n = g(x);
  if (n == 0)
    exit(0);
  if (x)
    abort();
  longjmp(env, 42);
}

int main(int argc, char **argv) {
  int v = 0;
  return f(&v);
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
// DEFAULT-NEXT:     fn %6 @_setjmp(%24 __env: ptr<@type3> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @longjmp(%25 __env: ptr<@type3> [array=1], %26 __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @exit(%27 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %13 @baz() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @g(%15 x: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%15), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @f(%17 e: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(%17))), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         let %18 x: i32 [storage=automatic] = call<i32, signature=fn(ptr<@type3>) -> i32>(%6, array_decay<ptr<@type3>, length=Some(1)>(%12));
// DEFAULT-NEXT:         let %19 n: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%14, read<i32>(%18));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%19), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%11, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%18), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%9, array_decay<ptr<@type3>, length=Some(1)>(%12), const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main(%21 argc: i32, %22 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %23 v: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<i32>) -> i32>(%16, addr_of<ptr<i32>>(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
