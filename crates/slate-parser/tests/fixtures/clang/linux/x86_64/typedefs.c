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
// DEFAULT-NEXT:     type @type0 my_int = i32;
// DEFAULT-NEXT:     type @type1 byte = u8;
// DEFAULT-NEXT:     type @type2 wide = i64;
// DEFAULT-NEXT:     type @type3 multi_scalar = i64;
// DEFAULT-NEXT:     type @type4 multi_array = array<i64, 23>;
// DEFAULT-NEXT:     type @type5 multi_pointer = ptr<i64>;
// DEFAULT-NEXT:     type @type6 Box = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:         field1 tag: u8;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%18 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @add_alias(%10 a: i32, %11 b: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%10), read<i32>(%11));
// DEFAULT-NEXT:         return read<i32>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 x: i32 [storage=automatic] = const<i32>(40);
// DEFAULT-NEXT:         let %15 y: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(200)));
// DEFAULT-NEXT:         let %16 z: i64 [storage=automatic] = const<i64>(9000000000);
// DEFAULT-NEXT:         let %17 bx: @type6 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%17), call<i32, signature=fn(i32, i32) -> i32>(%9, read<i32>(%14), const<i32>(2)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, i32) -> i32>(%9, read<i32>(%14), const<i32>(2));
// DEFAULT-NEXT:         write<u8>(field1(%17), read<u8>(%15));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%19)), read<i32>(field0(%17)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%20)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(field1(%17)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%21)), read<i64>(%16));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%22)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
