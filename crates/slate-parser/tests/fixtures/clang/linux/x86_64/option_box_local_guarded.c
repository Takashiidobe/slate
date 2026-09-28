#include <stdio.h>
#include <stdlib.h>

int compute(int flag) {
  int *p = NULL;
  if (flag) {
    p = malloc(sizeof(int));
  }
  if (p) {
    *p    = 41;
    int v = *p;
    printf("%d\n", v);
    free(p);
  } else {
    printf("no alloc\n");
  }
  return 0;
}

int main(void) {
  compute(1);
  compute(0);
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %15 .str15: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([110, 111, 32, 97, 108, 108, 111, 99, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%12 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%13 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @free(%14 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @compute(%8 flag: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 p: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<i32>>(%9, pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, const<u64>(4))));
// DEFAULT-NEXT:                 pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, const<u64>(4)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(%9), null<ptr<i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%9)), const<i32>(41));
// DEFAULT-NEXT:                 let %10 v: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%9)));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%15)), read<i32>(%10));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%9)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%16)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%7, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%7, const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
