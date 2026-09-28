#include <stdio.h>
#include <stdlib.h>

int *alloc(void) { return malloc(sizeof(int) * 10); }

int main(void) {
  int *x = NULL;
  x      = alloc();
  x      = x + 1;
  *x     = 10;
  printf("%d\n", *x);
  free(x - 1);
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
// DEFAULT-NEXT:     global %13 .str13: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%10 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%11 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %6 @free(%12 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @alloc() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<i32>, reason=return>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 x: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:         write<ptr<i32>>(%9, call<ptr<i32>, signature=fn() -> ptr<i32>>(%7));
// DEFAULT-NEXT:         call<ptr<i32>, signature=fn() -> ptr<i32>>(%7);
// DEFAULT-NEXT:         write<ptr<i32>>(%9, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%9)), const<i32>(10));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%13)), read<i32>(deref(read<ptr<i32>>(%9))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%6, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%9), const<i32>(1))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
