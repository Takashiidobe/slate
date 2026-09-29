

#include <stdio.h>

static int cached_value = 0;
static int computed     = 0;

static int compute(void) { return 42; }

int get_value(void) {
  if (!computed) {
    cached_value = compute();
    computed     = 1;
  }
  return cached_value;
}

/* Reads the payload outside the guarded init, so the singleton fixup must
   not fire: the static isn't exclusively owned by get_value anymore. */
int peek_value(void) { return cached_value; }

int main(void) {
  printf("%d\n", get_value());
  printf("%d\n", peek_value());
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
// DEFAULT-NEXT:     global %[[VALUE_cached_value:[0-9]+]] cached_value: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_computed:[0-9]+]] computed: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_compute:[0-9]+]] @compute() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(42);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_get_value:[0-9]+]] @get_value() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_computed]]), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_cached_value]], call<i32, signature=fn() -> i32>(%[[VALUE_compute]]));
// DEFAULT-NEXT:                 call<i32, signature=fn() -> i32>(%[[VALUE_compute]]);
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_computed]], const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_cached_value]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_peek_value:[0-9]+]] @peek_value() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_cached_value]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), call<i32, signature=fn() -> i32>(%[[VALUE_get_value]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), call<i32, signature=fn() -> i32>(%[[VALUE_peek_value]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
