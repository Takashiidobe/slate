struct mouse_button_str {
  unsigned char left   : 1;
  unsigned char right  : 1;
  unsigned char middle : 1;
} button;

static char fct(struct mouse_button_str newbutton) __attribute__((__noipa__));
static char fct(struct mouse_button_str newbutton) {
  char l = newbutton.left;
  char r = newbutton.right;
  char m = newbutton.middle;
  return l && r && m;
}

int main(void) {
  struct mouse_button_str newbutton1;
  newbutton1.left   = 1;
  newbutton1.middle = 1;
  newbutton1.right  = 1;
  if (!fct(newbutton1))
    __builtin_abort();

  newbutton1.left   = 0;
  newbutton1.middle = 1;
  newbutton1.right  = 1;
  if (fct(newbutton1))
    __builtin_abort();
  newbutton1.left   = 1;
  newbutton1.middle = 0;
  newbutton1.right  = 1;
  if (fct(newbutton1))
    __builtin_abort();
  newbutton1.left   = 1;
  newbutton1.middle = 1;
  newbutton1.right  = 0;
  if (fct(newbutton1))
    __builtin_abort();

  newbutton1.left   = 1;
  newbutton1.middle = 0;
  newbutton1.right  = 0;
  if (fct(newbutton1))
    __builtin_abort();
  newbutton1.left   = 0;
  newbutton1.middle = 1;
  newbutton1.right  = 0;
  if (fct(newbutton1))
    __builtin_abort();
  newbutton1.left   = 0;
  newbutton1.middle = 0;
  newbutton1.right  = 1;
  if (fct(newbutton1))
    __builtin_abort();
  newbutton1.left   = 0;
  newbutton1.middle = 0;
  newbutton1.right  = 0;
  if (fct(newbutton1))
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_mouse_button_str:[0-9]+]] mouse_button_str = struct {
// DEFAULT-NEXT:         field0 left: u8 : 1;
// DEFAULT-NEXT:         field1 right: u8 : 1;
// DEFAULT-NEXT:         field2 middle: u8 : 1;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(1), Some(2)], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_button:[0-9]+]] button: @type[[TYPE_mouse_button_str]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fct:[0-9]+]] @fct(%[[VALUE_newbutton:[0-9]+]] newbutton: @type[[TYPE_mouse_button_str]]) -> i8 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_newbutton]])))));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%[[VALUE_newbutton]])))));
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(bitfield2<unit=0, bytes=0..1, bits=2..3>(%[[VALUE_newbutton]])))));
// DEFAULT-NEXT:         return from_bool<i8, reason=return>(logical_and<bool>(logical_and<bool>(ne<i8>(read<i8>(%[[VALUE_l]]), const<i8>(0)), ne<i8>(read<i8>(%[[VALUE_r]]), const<i8>(0))), ne<i8>(read<i8>(%[[VALUE_m]]), const<i8>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_newbutton1:[0-9]+]] newbutton1: @type[[TYPE_mouse_button_str]] [storage=automatic];
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=2..3>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if not<bool>(ne<i8>(call<i8, signature=fn(@type[[TYPE_mouse_button_str]]) -> i8, abi=sysv64(native_c) -> scalar>(%[[VALUE_fct]], copy<@type[[TYPE_mouse_button_str]], reason=arg>(read<@type[[TYPE_mouse_button_str]]>(%[[VALUE_newbutton1]]))), const<i8>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=2..3>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i8>(call<i8, signature=fn(@type[[TYPE_mouse_button_str]]) -> i8, abi=sysv64(native_c) -> scalar>(%[[VALUE_fct]], copy<@type[[TYPE_mouse_button_str]], reason=arg>(read<@type[[TYPE_mouse_button_str]]>(%[[VALUE_newbutton1]]))), const<i8>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=2..3>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i8>(call<i8, signature=fn(@type[[TYPE_mouse_button_str]]) -> i8, abi=sysv64(native_c) -> scalar>(%[[VALUE_fct]], copy<@type[[TYPE_mouse_button_str]], reason=arg>(read<@type[[TYPE_mouse_button_str]]>(%[[VALUE_newbutton1]]))), const<i8>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=2..3>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i8>(call<i8, signature=fn(@type[[TYPE_mouse_button_str]]) -> i8, abi=sysv64(native_c) -> scalar>(%[[VALUE_fct]], copy<@type[[TYPE_mouse_button_str]], reason=arg>(read<@type[[TYPE_mouse_button_str]]>(%[[VALUE_newbutton1]]))), const<i8>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=2..3>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i8>(call<i8, signature=fn(@type[[TYPE_mouse_button_str]]) -> i8, abi=sysv64(native_c) -> scalar>(%[[VALUE_fct]], copy<@type[[TYPE_mouse_button_str]], reason=arg>(read<@type[[TYPE_mouse_button_str]]>(%[[VALUE_newbutton1]]))), const<i8>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=2..3>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         write<u8>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i8>(call<i8, signature=fn(@type[[TYPE_mouse_button_str]]) -> i8, abi=sysv64(native_c) -> scalar>(%[[VALUE_fct]], copy<@type[[TYPE_mouse_button_str]], reason=arg>(read<@type[[TYPE_mouse_button_str]]>(%[[VALUE_newbutton1]]))), const<i8>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=2..3>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i8>(call<i8, signature=fn(@type[[TYPE_mouse_button_str]]) -> i8, abi=sysv64(native_c) -> scalar>(%[[VALUE_fct]], copy<@type[[TYPE_mouse_button_str]], reason=arg>(read<@type[[TYPE_mouse_button_str]]>(%[[VALUE_newbutton1]]))), const<i8>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u8>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield2<unit=0, bytes=0..1, bits=2..3>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u8>(bitfield1<unit=0, bytes=0..1, bits=1..2>(%[[VALUE_newbutton1]]), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i8>(call<i8, signature=fn(@type[[TYPE_mouse_button_str]]) -> i8, abi=sysv64(native_c) -> scalar>(%[[VALUE_fct]], copy<@type[[TYPE_mouse_button_str]], reason=arg>(read<@type[[TYPE_mouse_button_str]]>(%[[VALUE_newbutton1]]))), const<i8>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
