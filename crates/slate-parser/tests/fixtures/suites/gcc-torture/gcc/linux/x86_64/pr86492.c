/* PR tree-optimization/86492 */

union U {
  unsigned int r;
  struct S {
    unsigned int a : 12;
    unsigned int b : 4;
    unsigned int c : 16;
  } f;
};

__attribute__((noipa)) unsigned int foo(unsigned int x) {
  union U u;
  u.r   = 0;
  u.f.c = x;
  u.f.b = 0xe;
  return u.r;
}

int main() {
  union U u;
  if (__CHAR_BIT__ * __SIZEOF_INT__ != 32 || sizeof(u.r) != sizeof(u.f))
    return 0;
  u.r = foo(0x72);
  if (u.f.a != 0 || u.f.b != 0xe || u.f.c != 0x72)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 r: u32;
// DEFAULT-NEXT:         field1 f: @type[[TYPE_S:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_S]] S = struct {
// DEFAULT-NEXT:         field0 a: u32 : 12;
// DEFAULT-NEXT:         field1 b: u32 : 4;
// DEFAULT-NEXT:         field2 c: u32 : 16;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 1, 2], bit_offsets=[Some(0), Some(12), Some(16)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         write<u32>(field0(%[[VALUE_u]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=16..32>(field1(%[[VALUE_u]])), read<u32>(%[[VALUE_x]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=12..16>(field1(%[[VALUE_u]])), reinterpret<u32, reason=assign, fits=always>(const<i32>(14)));
// DEFAULT-NEXT:         return read<u32>(field0(%[[VALUE_u]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_u_2:[0-9]+]] u: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(4)), const<i32>(32)), ne<u64>(const<u64>(4), const<u64>(4)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<u32>(field0(%[[VALUE_u_2]]), call<u32, signature=fn(u32) -> u32>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(114))));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%[[VALUE_foo]], reinterpret<u32, reason=arg, fits=always>(const<i32>(114)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(field1(%[[VALUE_u_2]])))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..16>(field1(%[[VALUE_u_2]])))), const<i32>(14))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=16..32>(field1(%[[VALUE_u_2]])))), const<i32>(114)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
