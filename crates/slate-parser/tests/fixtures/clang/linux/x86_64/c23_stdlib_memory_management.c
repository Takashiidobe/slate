#include <stdio.h>
#include <stdlib.h>

int main(void) {
  void *p = malloc(16);
  free_sized(p, 16);

  void *q = aligned_alloc(16, 32);
  free_aligned_sized(q, 16, 32);

  void *r = realloc(NULL, 0);
  free(r);

  void *a = aligned_alloc(64, 64);
  printf("%d\n", memalignment(a) % 64 == 0);
  free(a);

  printf("ok\n");
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
// DEFAULT-NEXT:     global %40 .str40: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 107, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%27 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%28 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @realloc(%29 __ptr: ptr<void>, %30 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %9 @free(%31 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @free_sized(%32 __ptr: ptr<void>, %33 __size: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %16 @free_aligned_sized(%34 __ptr: ptr<void>, %35 __alignment: u64, %36 __size: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %19 @aligned_alloc(%37 __alignment: u64, %38 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %21 @memalignment(%39 __p: ptr<const void>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %22 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %23 p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%12, read<ptr<void>>(%23), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         let %24 q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%19, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64) -> void>(%16, read<ptr<void>>(%24), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         let %25 r: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%7, null<ptr<void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%9, read<ptr<void>>(%25));
// DEFAULT-NEXT:         let %26 a: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%19, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%40)), from_bool<i32, reason=vararg>(eq<u64>(rem<u64, by_zero=ub>(call<u64, signature=fn(ptr<const void>) -> u64>(%21, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%26))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(64)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%9, read<ptr<void>>(%26));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%41)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
