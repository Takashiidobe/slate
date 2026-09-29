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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bump:[0-9]+]] @bump(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_p]])), add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_p]]))), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_peek:[0-9]+]] @peek(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_p_2]]))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use_and_free:[0-9]+]] @use_and_free(%[[VALUE_y:[0-9]+]] y: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_y]])), add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_y]]))), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE_y]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_y]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_bump]], addr_of<ptr<i32>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = const<i32>(10);
// DEFAULT-NEXT:         let %[[VALUE_peeked:[0-9]+]] peeked: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_peek]], addr_of<ptr<i32>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_c]])), const<i32>(100));
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>) -> i32>(%[[VALUE_use_and_free]], read<ptr<i32>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str]])), read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_peeked]]), read<i32>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_peeked]])), read<i32>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
