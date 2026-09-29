#include <stdio.h>

typedef enum color_e { RED, GREEN, BLUE } color_t;

struct palette {
  color_t *start;
  color_t *top;
};

int main(void) {
  color_t        data[3] = {RED, GREEN, BLUE};
  struct palette p;
  p.start   = data;
  p.top     = data + 3;
  color_t c = *(p.start + 1);
  printf("%d\n", (int)c);
  printf("%d\n", (int)(p.top - p.start));
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
// DEFAULT-NEXT:     type @type[[TYPE_color_e:[0-9]+]] color_e = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_RED:[0-9]+]] RED = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_GREEN:[0-9]+]] GREEN = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_BLUE:[0-9]+]] BLUE = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_color_t:[0-9]+]] color_t = @type[[TYPE_color_e]];
// DEFAULT-NEXT:     type @type[[TYPE_palette:[0-9]+]] palette = struct {
// DEFAULT-NEXT:         field0 start: ptr<@type[[TYPE_color_e]]>;
// DEFAULT-NEXT:         field1 top: ptr<@type[[TYPE_color_e]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_GREEN]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_data:[0-9]+]] data: array<@type[[TYPE_color_e]], 3> [storage=automatic] = aggregate<array<@type[[TYPE_color_e]], 3>, zero_fill=false>(index0 = int_to_enum<@type[[TYPE_color_e]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))), index1 = int_to_enum<@type[[TYPE_color_e]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))), index2 = int_to_enum<@type[[TYPE_color_e]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: @type[[TYPE_palette]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_color_e]]>>(field0(%[[VALUE_p]]), array_decay<ptr<@type[[TYPE_color_e]]>, length=Some(3)>(%[[VALUE_data]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_color_e]]>>(field1(%[[VALUE_p]]), ptr_offset<ptr<@type[[TYPE_color_e]]>, subtract=false, element=@type[[TYPE_color_e]], overflow=ub>(array_decay<ptr<@type[[TYPE_color_e]]>, length=Some(3)>(%[[VALUE_data]]), const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_color_e]] [storage=automatic] = read<@type[[TYPE_color_e]]>(deref(ptr_offset<ptr<@type[[TYPE_color_e]]>, subtract=false, element=@type[[TYPE_color_e]], overflow=ub>(read<ptr<@type[[TYPE_color_e]]>>(field0(%[[VALUE_p]])), const<i32>(1))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_GREEN]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_color_e]]>(%[[VALUE_c]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_GREEN]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), truncate<i32, reason=explicit, fits=unknown>(ptr_diff<i64, element=@type[[TYPE_color_e]], same_array=required, overflow=ub>(read<ptr<@type[[TYPE_color_e]]>>(field1(%[[VALUE_p]])), read<ptr<@type[[TYPE_color_e]]>>(field0(%[[VALUE_p]])))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
