#include <stdio.h>
#include <stdlib.h>

typedef void *(*ReallocFn)(void *, size_t);
typedef void  (*FreeFn)(void *);

struct Allocator {
  ReallocFn realloc_fn;
  FreeFn    free_fn;
};

struct Allocator alloc = {realloc, free};

int main(void) {
  void *p = alloc.realloc_fn(NULL, 8);
  alloc.free_fn(p);
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
// DEFAULT-NEXT:     type @type1 ReallocFn = ptr<fn(ptr<void>, u64) -> ptr<void>>;
// DEFAULT-NEXT:     type @type2 FreeFn = ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:     type @type3 Allocator = struct {
// DEFAULT-NEXT:         field0 realloc_fn: ptr<fn(ptr<void>, u64) -> ptr<void>>;
// DEFAULT-NEXT:         field1 free_fn: ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %11 alloc: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = function_decay<ptr<fn(ptr<void>, u64) -> ptr<void>>>(%5), field1 = function_decay<ptr<fn(ptr<void>) -> void>>(%7)) [linkage=external];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 107, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%14 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @realloc(%15 __ptr: ptr<void>, %16 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @free(%17 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(read<ptr<fn(ptr<void>, u64) -> ptr<void>>>(field0(%11)), null<ptr<void>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(read<ptr<fn(ptr<void>) -> void>>(field1(%11)), read<ptr<void>>(%13));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%18)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
