/* PR rtl-optimization/93908 */

struct T {
  int            b;
  int            c;
  unsigned short d;
  unsigned       e : 1, f : 1, g : 1, h : 2, i : 1, j : 1;
  signed int     k : 2;
};

struct S {
  struct T s;
  char     c[64];
} buf[2];

__attribute__((noipa)) void *baz(void) {
  static int cnt;
  return (void *)&buf[cnt++];
}

static inline __attribute__((always_inline)) struct T *bar(const char *a) {
  struct T *s;
  s    = baz();
  s->b = 1;
  s->k = -1;
  return s;
}

__attribute__((noipa)) void foo(const char *x, struct T **y) {
  struct T *l = bar(x);
  struct T *m = bar(x);
  y[0]        = l;
  y[1]        = m;
}

int main() {
  struct T *r[2];
  foo("foo", r);
  if (r[0]->e || r[0]->f || r[0]->g || r[0]->h || r[0]->i || r[0]->j ||
      r[0]->k != -1)
    __builtin_abort();
  if (r[1]->e || r[1]->f || r[1]->g || r[1]->h || r[1]->i || r[1]->j ||
      r[1]->k != -1)
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
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 b: i32;
// DEFAULT-NEXT:         field1 c: i32;
// DEFAULT-NEXT:         field2 d: u16;
// DEFAULT-NEXT:         field3 e: u32 : 1;
// DEFAULT-NEXT:         field4 f: u32 : 1;
// DEFAULT-NEXT:         field5 g: u32 : 1;
// DEFAULT-NEXT:         field6 h: u32 : 2;
// DEFAULT-NEXT:         field7 i: u32 : 1;
// DEFAULT-NEXT:         field8 j: u32 : 1;
// DEFAULT-NEXT:         field9 k: i32 : 2;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8, 10, 10, 10, 10, 10, 10, 10], bit_offsets=[None, None, None, Some(80), Some(81), Some(82), Some(83), Some(85), Some(86), Some(87)], bit_units=[(10, 2)], field_units=[None, None, None, Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 s: @type[[TYPE_T]];
// DEFAULT-NEXT:         field1 c: array<i8, 64>;
// DEFAULT-NEXT:     } [size=76, align=4, offsets=[0, 12]];
// DEFAULT-NEXT:     global %[[VALUE_buf:[0-9]+]] buf: array<@type[[TYPE_S]], 2> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cnt:[0-9]+]] cnt: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_cnt]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_cnt]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type[[TYPE_S]]>>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_buf]]), read<i32>(%[[VALUE0]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_a:[0-9]+]] a: ptr<const i8>) -> ptr<@type[[TYPE_T]]> [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_T]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_T]]>>(%[[VALUE_s]], pointer_cast<ptr<@type[[TYPE_T]]>, reason=assign>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_baz]])));
// DEFAULT-NEXT:         pointer_cast<ptr<@type[[TYPE_T]]>, reason=assign>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_baz]]));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_s]]))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(bitfield9<unit=0, bytes=10..12, bits=7..9>(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_s]]))), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_T]]>>(%[[VALUE_s]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<const i8>, %[[VALUE_y:[0-9]+]] y: ptr<ptr<@type[[TYPE_T]]>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: ptr<@type[[TYPE_T]]> [storage=automatic] = call<ptr<@type[[TYPE_T]]>, signature=fn(ptr<const i8>) -> ptr<@type[[TYPE_T]]>>(%[[VALUE_bar]], read<ptr<const i8>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: ptr<@type[[TYPE_T]]> [storage=automatic] = call<ptr<@type[[TYPE_T]]>, signature=fn(ptr<const i8>) -> ptr<@type[[TYPE_T]]>>(%[[VALUE_bar]], read<ptr<const i8>>(%[[VALUE_x]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_T]]>>>(%[[VALUE_y]]), const<i32>(0))), read<ptr<@type[[TYPE_T]]>>(%[[VALUE_l]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_T]]>>>(%[[VALUE_y]]), const<i32>(1))), read<ptr<@type[[TYPE_T]]>>(%[[VALUE_m]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: array<ptr<@type[[TYPE_T]]>, 2> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, ptr<ptr<@type[[TYPE_T]]>>) -> void>(%[[VALUE_foo]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=10..12, bits=0..1>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(0)))))))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield4<unit=0, bytes=10..12, bits=1..2>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(0)))))))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield5<unit=0, bytes=10..12, bits=2..3>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(0)))))))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=0, bytes=10..12, bits=3..5>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(0)))))))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=0, bytes=10..12, bits=5..6>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(0)))))))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield8<unit=0, bytes=10..12, bits=6..7>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(0)))))))), const<i32>(0))), ne<i32>(read<i32>(bitfield9<unit=0, bytes=10..12, bits=7..9>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(0))))))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=10..12, bits=0..1>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(1)))))))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield4<unit=0, bytes=10..12, bits=1..2>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(1)))))))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield5<unit=0, bytes=10..12, bits=2..3>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(1)))))))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=0, bytes=10..12, bits=3..5>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(1)))))))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=0, bytes=10..12, bits=5..6>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(1)))))))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield8<unit=0, bytes=10..12, bits=6..7>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(1)))))))), const<i32>(0))), ne<i32>(read<i32>(bitfield9<unit=0, bytes=10..12, bits=7..9>(deref(read<ptr<@type[[TYPE_T]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_T]]>>, subtract=false, element=ptr<@type[[TYPE_T]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_T]]>>, length=Some(2)>(%[[VALUE_r]]), const<i32>(1))))))), neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
