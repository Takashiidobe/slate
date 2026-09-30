/* PR tree-optimization/108498 */
/* { dg-require-effective-target int32plus } */

struct U {
  char c[16];
};
struct V {
  char c[16];
};
struct S {
  unsigned int a : 3, b : 8, c : 21;
  struct U     d;
  unsigned int e;
  struct V     f;
  unsigned int g : 5, h : 27;
};
struct T {
  unsigned int a : 16, b : 8, c : 8;
  struct U     d;
  unsigned int e;
  struct V     f;
  unsigned int g : 5, h : 27;
};

__attribute__((noipa)) void foo(struct S *p) {
  p->b = 231;
  p->c = 42;
  p->d = (struct U){"abcdefghijklmno"};
  p->e = 0xdeadbeef;
  p->f = (struct V){"ABCDEFGHIJKLMNO"};
}

__attribute__((noipa)) void bar(struct S *p) {
  p->b = 231;
  p->c = 42;
  p->d = (struct U){"abcdefghijklmno"};
  p->e = 0xdeadbeef;
  p->f = (struct V){"ABCDEFGHIJKLMNO"};
  p->g = 12;
}

__attribute__((noipa)) void baz(struct T *p) {
  p->c = 42;
  p->d = (struct U){"abcdefghijklmno"};
  p->e = 0xdeadbeef;
  p->f = (struct V){"ABCDEFGHIJKLMNO"};
  p->g = 12;
}

int main() {
  if (__CHAR_BIT__ != 8 || __SIZEOF_INT__ != 4)
    return 0;
  struct S s = {};
  struct T t = {};
  foo(&s);
  if (s.a != 0 || s.b != 231 || s.c != 42 ||
      __builtin_memcmp(&s.d.c, "abcdefghijklmno", 16) || s.e != 0xdeadbeef ||
      __builtin_memcmp(&s.f.c, "ABCDEFGHIJKLMNO", 16) || s.g != 0 || s.h != 0)
    __builtin_abort();
  __builtin_memset(&s, 0, sizeof(s));
  s.a = 7;
  s.g = 31;
  s.h = (1U << 27) - 1;
  foo(&s);
  if (s.a != 7 || s.b != 231 || s.c != 42 ||
      __builtin_memcmp(&s.d.c, "abcdefghijklmno", 16) || s.e != 0xdeadbeef ||
      __builtin_memcmp(&s.f.c, "ABCDEFGHIJKLMNO", 16) || s.g != 31 ||
      s.h != (1U << 27) - 1)
    __builtin_abort();
  __builtin_memset(&s, 0, sizeof(s));
  bar(&s);
  if (s.a != 0 || s.b != 231 || s.c != 42 ||
      __builtin_memcmp(&s.d.c, "abcdefghijklmno", 16) || s.e != 0xdeadbeef ||
      __builtin_memcmp(&s.f.c, "ABCDEFGHIJKLMNO", 16) || s.g != 12 || s.h != 0)
    __builtin_abort();
  __builtin_memset(&s, 0, sizeof(s));
  s.a = 7;
  s.g = 31;
  s.h = (1U << 27) - 1;
  bar(&s);
  if (s.a != 7 || s.b != 231 || s.c != 42 ||
      __builtin_memcmp(&s.d.c, "abcdefghijklmno", 16) || s.e != 0xdeadbeef ||
      __builtin_memcmp(&s.f.c, "ABCDEFGHIJKLMNO", 16) || s.g != 12 ||
      s.h != (1U << 27) - 1)
    __builtin_abort();
  baz(&t);
  if (t.a != 0 || t.b != 0 || t.c != 42 ||
      __builtin_memcmp(&t.d.c, "abcdefghijklmno", 16) || t.e != 0xdeadbeef ||
      __builtin_memcmp(&t.f.c, "ABCDEFGHIJKLMNO", 16) || t.g != 12 || t.h != 0)
    __builtin_abort();
  __builtin_memset(&s, 0, sizeof(s));
  t.a = 7;
  t.b = 255;
  t.g = 31;
  t.h = (1U << 27) - 1;
  baz(&t);
  if (t.a != 7 || t.b != 255 || t.c != 42 ||
      __builtin_memcmp(&t.d.c, "abcdefghijklmno", 16) || t.e != 0xdeadbeef ||
      __builtin_memcmp(&t.f.c, "ABCDEFGHIJKLMNO", 16) || t.g != 12 ||
      t.h != (1U << 27) - 1)
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
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 16>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 16>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: u32 : 3;
// DEFAULT-NEXT:         field1 b: u32 : 8;
// DEFAULT-NEXT:         field2 c: u32 : 21;
// DEFAULT-NEXT:         field3 d: @type[[TYPE_U]];
// DEFAULT-NEXT:         field4 e: u32;
// DEFAULT-NEXT:         field5 f: @type[[TYPE_V]];
// DEFAULT-NEXT:         field6 g: u32 : 5;
// DEFAULT-NEXT:         field7 h: u32 : 27;
// DEFAULT-NEXT:     } [size=44, align=4, offsets=[0, 0, 1, 4, 20, 24, 40, 40], bit_offsets=[Some(0), Some(3), Some(11), None, None, None, Some(320), Some(325)], bit_units=[(0, 4), (40, 4)], field_units=[Some(0), Some(0), Some(0), None, None, None, Some(1), Some(1)]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 a: u32 : 16;
// DEFAULT-NEXT:         field1 b: u32 : 8;
// DEFAULT-NEXT:         field2 c: u32 : 8;
// DEFAULT-NEXT:         field3 d: @type[[TYPE_U]];
// DEFAULT-NEXT:         field4 e: u32;
// DEFAULT-NEXT:         field5 f: @type[[TYPE_V]];
// DEFAULT-NEXT:         field6 g: u32 : 5;
// DEFAULT-NEXT:         field7 h: u32 : 27;
// DEFAULT-NEXT:     } [size=44, align=4, offsets=[0, 2, 3, 4, 20, 24, 40, 40], bit_offsets=[Some(0), Some(16), Some(24), None, None, None, Some(320), Some(325)], bit_units=[(0, 4), (40, 4)], field_units=[Some(0), Some(0), Some(0), None, None, None, Some(1), Some(1)]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(231)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]))), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0])))));
// DEFAULT-NEXT:         write<u32>(field4(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]))), const<u32>(3735928559));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field5(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]))), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_V]], zero_fill=false>(field0 = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(231)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]]))), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0])))));
// DEFAULT-NEXT:         write<u32>(field4(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]]))), const<u32>(3735928559));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field5(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]]))), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_V]], zero_fill=false>(field0 = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0])))));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_T]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p_3]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p_3]]))), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(compound_literal %[[VALUE4:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0])))));
// DEFAULT-NEXT:         write<u32>(field4(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p_3]]))), const<u32>(3735928559));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field5(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p_3]]))), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_V]], zero_fill=false>(field0 = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0])))));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p_3]]))), reinterpret<u32, reason=assign, fits=always>(const<i32>(12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE6:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE7:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE8:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE9:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE10:[0-9]+]] <unnamed>: i32, %[[VALUE11:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(const<i32>(8), const<i32>(8)), ne<i32>(const<i32>(4), const<i32>(4)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_T]] [storage=automatic] = aggregate<@type[[TYPE_T]], zero_fill=true>();
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%[[VALUE_s]]))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(%[[VALUE_s]]))), const<i32>(231))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(%[[VALUE_s]]))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%[[VALUE_s]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE12]]), ne<u32>(read<u32>(field4(%[[VALUE_s]])), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%[[VALUE_s]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%[[VALUE13]]), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%[[VALUE_s]]))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%[[VALUE_s]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])), const<i32>(0), const<u64>(44));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%[[VALUE_s]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%[[VALUE_s]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         write<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%[[VALUE_s]]), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%[[VALUE_s]]))), const<i32>(7)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(%[[VALUE_s]]))), const<i32>(231))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(%[[VALUE_s]]))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%[[VALUE_s]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_3]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE14]]), ne<u32>(read<u32>(field4(%[[VALUE_s]])), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%[[VALUE_s]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_4]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%[[VALUE15]]), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%[[VALUE_s]]))), const<i32>(31))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%[[VALUE_s]])))), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])), const<i32>(0), const<u64>(44));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%[[VALUE_s]]))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(%[[VALUE_s]]))), const<i32>(231))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(%[[VALUE_s]]))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%[[VALUE_s]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_5]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE16]]), ne<u32>(read<u32>(field4(%[[VALUE_s]])), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE17]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE17]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%[[VALUE_s]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_6]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%[[VALUE17]]), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%[[VALUE_s]]))), const<i32>(12))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%[[VALUE_s]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])), const<i32>(0), const<u64>(44));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%[[VALUE_s]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%[[VALUE_s]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         write<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%[[VALUE_s]]), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%[[VALUE_s]]))), const<i32>(7)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(%[[VALUE_s]]))), const<i32>(231))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(%[[VALUE_s]]))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%[[VALUE_s]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_7]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE18]]), ne<u32>(read<u32>(field4(%[[VALUE_s]])), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE19]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE19]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%[[VALUE_s]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_8]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%[[VALUE19]]), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%[[VALUE_s]]))), const<i32>(12))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%[[VALUE_s]])))), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_T]]>) -> void>(%[[VALUE_baz]], addr_of<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_t]]))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_t]]))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_t]]))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE20]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE20]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%[[VALUE_t]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_9]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE20]]), ne<u32>(read<u32>(field4(%[[VALUE_t]])), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE21]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE21]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%[[VALUE_t]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_10]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%[[VALUE21]]), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%[[VALUE_t]]))), const<i32>(12))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%[[VALUE_t]]))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])), const<i32>(0), const<u64>(44));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_t]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_t]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(255)));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%[[VALUE_t]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         write<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%[[VALUE_t]]), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_T]]>) -> void>(%[[VALUE_baz]], addr_of<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]]));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%[[VALUE_t]]))), const<i32>(7)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%[[VALUE_t]]))), const<i32>(255))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%[[VALUE_t]]))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE22]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE22]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%[[VALUE_t]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_11]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE22]]), ne<u32>(read<u32>(field4(%[[VALUE_t]])), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE23]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE23]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%[[VALUE_t]])))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_12]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%[[VALUE23]]), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%[[VALUE_t]]))), const<i32>(12))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%[[VALUE_t]])))), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
