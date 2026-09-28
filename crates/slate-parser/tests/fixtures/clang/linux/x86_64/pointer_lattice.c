#include <stdio.h>
#include <stdlib.h>

static void bump(int *p) { *p = *p + 1; }

static int peek(int *p) { return *p + 1; }

static int use_and_free(int *y) {
  *y    = *y + 1;
  int v = *y;
  free(y);
  return v;
}

int main(void) {
  int a = 1;
  bump(&a);

  int b      = 10;
  int peeked = peek(&b);

  int *c = malloc(sizeof(int));
  *c     = 100;
  int v  = use_and_free(c);

  printf("%d %d %d\n", a, peeked, v);
  return a + peeked + v;
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %23 .str23: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%20 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%21 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @free(%22 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @bump(%8 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%8)), add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%8))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @peek(%10 p: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%10))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @use_and_free(%12 y: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%12)), add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%12))), const<i32>(1)));
// DEFAULT-NEXT:         let %13 v: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%12)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%12)));
// DEFAULT-NEXT:         return read<i32>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%7, addr_of<ptr<i32>>(%15));
// DEFAULT-NEXT:         let %16 b: i32 [storage=automatic] = const<i32>(10);
// DEFAULT-NEXT:         let %17 peeked: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>) -> i32>(%9, addr_of<ptr<i32>>(%16));
// DEFAULT-NEXT:         let %18 c: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%18)), const<i32>(100));
// DEFAULT-NEXT:         let %19 v: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>) -> i32>(%11, read<ptr<i32>>(%18));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%23)), read<i32>(%15), read<i32>(%17), read<i32>(%19));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%15), read<i32>(%17)), read<i32>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
