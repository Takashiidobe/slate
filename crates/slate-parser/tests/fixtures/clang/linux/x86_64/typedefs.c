#include <stdio.h>

typedef int           my_int;
typedef unsigned char byte;
typedef long long     wide;
typedef long long multi_scalar, multi_array[23], *multi_pointer;

struct Box {
  my_int value;
  byte   tag;
};

static my_int add_alias(my_int a, my_int b) {
  my_int c = a + b;
  return c;
}

int main(void) {
  my_int     x = 40;
  byte       y = 200;
  wide       z = 9000000000LL;
  struct Box bx;
  bx.value = add_alias(x, 2);
  bx.tag   = y;
  printf("%d\n", bx.value);
  printf("%d\n", bx.tag);
  printf("%lld\n", z);
  printf("%d\n", (int)sizeof(my_int));
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
// DEFAULT-NEXT:     type @type[[TYPE_my_int:[0-9]+]] my_int = i32;
// DEFAULT-NEXT:     type @type[[TYPE_byte:[0-9]+]] byte = u8;
// DEFAULT-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = i64;
// DEFAULT-NEXT:     type @type[[TYPE_multi_scalar:[0-9]+]] multi_scalar = i64;
// DEFAULT-NEXT:     type @type[[TYPE_multi_array:[0-9]+]] multi_array = array<i64, 23>;
// DEFAULT-NEXT:     type @type[[TYPE_multi_pointer:[0-9]+]] multi_pointer = ptr<i64>;
// DEFAULT-NEXT:     type @type[[TYPE_Box:[0-9]+]] Box = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:         field1 tag: u8;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add_alias:[0-9]+]] @add_alias(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = const<i32>(40);
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(200)));
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i64 [storage=automatic] = const<i64>(9000000000);
// DEFAULT-NEXT:         let %[[VALUE_bx:[0-9]+]] bx: @type[[TYPE_Box]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_bx]]), call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_add_alias]], read<i32>(%[[VALUE_x]]), const<i32>(2)));
// DEFAULT-NEXT:         write<u8>(field1(%[[VALUE_bx]]), read<u8>(%[[VALUE_y]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), read<i32>(field0(%[[VALUE_bx]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(field1(%[[VALUE_bx]])))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_3]])), read<i64>(%[[VALUE_z]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
