/* PR target/100402 */
/* Testcase by Hannes Domani <ssbssa@yahoo.de> */

/* { dg-require-effective-target indirect_jumps } */

#include <setjmp.h>
#include <stdbool.h>

static jmp_buf buf;
static _Bool   stop = false;

void call_func(void (*func)(void)) { func(); }

void func(void) {
  stop = true;
  longjmp(buf, 1);
}

int main(void) {
  setjmp(buf);

  while (!stop)
    call_func(func);

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
// DEFAULT-NEXT:     global %10 buf: array<@type3, 1> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %11 stop: bool [storage=static] = const<bool>(false) [linkage=internal];
// DEFAULT-NEXT:     fn %6 @_setjmp(%16 __env: ptr<@type3> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @longjmp(%17 __env: ptr<@type3> [array=1], %18 __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @call_func(%13 func: ptr<fn() -> void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @func() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%9, array_decay<ptr<@type3>, length=Some(1)>(%10), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>) -> i32>(%6, array_decay<ptr<@type3>, length=Some(1)>(%10));
// DEFAULT-NEXT:         while %19 not<bool>(read<bool>(%11))
// DEFAULT-NEXT:             call<void, signature=fn(ptr<fn() -> void>) -> void>(%12, function_decay<ptr<fn() -> void>>(%14));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
