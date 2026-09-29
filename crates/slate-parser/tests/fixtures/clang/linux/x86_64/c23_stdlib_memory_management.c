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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 107, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_realloc:[0-9]+]] @realloc(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>, %[[VALUE___size_2:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr_2:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free_sized:[0-9]+]] @free_sized(%[[VALUE___ptr_3:[0-9]+]] __ptr: ptr<void>, %[[VALUE___size_3:[0-9]+]] __size: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free_aligned_sized:[0-9]+]] @free_aligned_sized(%[[VALUE___ptr_4:[0-9]+]] __ptr: ptr<void>, %[[VALUE___alignment:[0-9]+]] __alignment: u64, %[[VALUE___size_4:[0-9]+]] __size: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_aligned_alloc:[0-9]+]] @aligned_alloc(%[[VALUE___alignment_2:[0-9]+]] __alignment: u64, %[[VALUE___size_5:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memalignment:[0-9]+]] @memalignment(%[[VALUE___p:[0-9]+]] __p: ptr<const void>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(%[[VALUE_free_sized]], read<ptr<void>>(%[[VALUE_p]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64) -> void>(%[[VALUE_free_aligned_sized]], read<ptr<void>>(%[[VALUE_q]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], null<ptr<void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_r]]));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), from_bool<i32, reason=vararg>(eq<u64>(rem<u64, by_zero=ub>(call<u64, signature=fn(ptr<const void>) -> u64>(%[[VALUE_memalignment]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%[[VALUE_a]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(64)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
