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
// DEFAULT-NEXT:     type @type5 size_t = u64;
// DEFAULT-NEXT:     global %17 jb_stack: ptr<array<@type3, 1>> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %18 jb_top: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([99, 97, 115, 101, 32, 37, 100, 58, 32, 110, 111, 32, 101, 120, 99, 101, 112, 116, 105, 111, 110, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([99, 97, 115, 101, 32, 37, 100, 58, 32, 99, 97, 117, 103, 104, 116, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %6 @_setjmp(%25 __env: ptr<@type3> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @longjmp(%26 __env: ptr<@type3> [array=1], %27 __val: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @printf(%28 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %14 @malloc(%29 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %16 @free(%30 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %19 @inner(%20 fail: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%20), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %33: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%18, read<i32>(%34));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type3>, i32) -> void>(%9, array_decay<ptr<@type3>, length=Some(1)>(deref(ptr_offset<ptr<array<@type3, 1>>, subtract=false, element=array<@type3, 1>, overflow=ub>(read<ptr<array<@type3, 1>>>(%17), read<i32>(%34)))), const<i32>(42));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @run_case(%22 id: i32, %23 fail: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %35: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:         let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%18, read<i32>(%36));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<@type3>) -> i32>(%6, array_decay<ptr<@type3>, length=Some(1)>(deref(ptr_offset<ptr<array<@type3, 1>>, subtract=false, element=array<@type3, 1>, overflow=ub>(read<ptr<array<@type3, 1>>>(%17), read<i32>(%35))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%19, read<i32>(%23));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%12, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%31)), read<i32>(%22));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%12, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%32)), read<i32>(%22));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<array<@type3, 1>>>(%17, pointer_cast<ptr<array<@type3, 1>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, mul<u64, overflow=wrap>(const<u64>(200), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))))));
// DEFAULT-NEXT:         pointer_cast<ptr<array<@type3, 1>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, mul<u64, overflow=wrap>(const<u64>(200), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%21, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%21, const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%21, const<i32>(2), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%16, pointer_cast<ptr<void>, reason=arg>(read<ptr<array<@type3, 1>>>(%17)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
