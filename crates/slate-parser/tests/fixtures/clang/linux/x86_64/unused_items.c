#include <stdio.h>

struct totally_unused {
  int a;
  int b;
};

struct linked_a {
  int              x;
  struct linked_b *link;
};

struct linked_b {
  int              y;
  struct linked_a *link;
};

enum truly_dead { DEAD_A, DEAD_B };

enum color { RED, GREEN, BLUE };

static int hidden_static = 5;
static int live_static   = 7;

int compute(void) {
  int x = hidden_static;
  return 42;
}

int main(void) {
  enum color c = GREEN;
  printf("%d\n", c);
  printf("%d\n", live_static);
  printf("%d\n", compute());
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
// DEFAULT-NEXT:     type @type[[TYPE_totally_unused:[0-9]+]] totally_unused = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_linked_a:[0-9]+]] linked_a = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 link: ptr<@type[[TYPE_linked_b:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_linked_b]] linked_b = struct {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:         field1 link: ptr<@type[[TYPE_linked_a]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_truly_dead:[0-9]+]] truly_dead = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_DEAD_A:[0-9]+]] DEAD_A = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_DEAD_B:[0-9]+]] DEAD_B = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_color:[0-9]+]] color = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_DEAD_A]] RED = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_DEAD_B]] GREEN = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_BLUE:[0-9]+]] BLUE = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_hidden_static:[0-9]+]] hidden_static: i32 [storage=static] = const<i32>(5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_live_static:[0-9]+]] live_static: i32 [storage=static] = const<i32>(7) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_DEAD_B]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_compute:[0-9]+]] @compute() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = read<i32>(%[[VALUE_hidden_static]]);
// DEFAULT-NEXT:         return const<i32>(42);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_color]] [storage=automatic] = int_to_enum<@type[[TYPE_color]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_DEAD_B]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), enum_to_int<u32, reason=promotion>(read<@type[[TYPE_color]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_DEAD_B]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_live_static]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_DEAD_B]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])), call<i32, signature=fn() -> i32>(%[[VALUE_compute]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
