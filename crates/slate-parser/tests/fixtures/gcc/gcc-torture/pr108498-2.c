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
// DEFAULT-NEXT:     type @type0 U = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 16>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 V = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 16>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type2 S = struct {
// DEFAULT-NEXT:         field0 a: u32 : 3;
// DEFAULT-NEXT:         field1 b: u32 : 8;
// DEFAULT-NEXT:         field2 c: u32 : 21;
// DEFAULT-NEXT:         field3 d: @type0;
// DEFAULT-NEXT:         field4 e: u32;
// DEFAULT-NEXT:         field5 f: @type1;
// DEFAULT-NEXT:         field6 g: u32 : 5;
// DEFAULT-NEXT:         field7 h: u32 : 27;
// DEFAULT-NEXT:     } [size=44, align=4, offsets=[0, 0, 1, 4, 20, 24, 40, 40], bit_offsets=[Some(0), Some(3), Some(11), None, None, None, Some(320), Some(325)], bit_units=[(0, 4), (40, 4)], field_units=[Some(0), Some(0), Some(0), None, None, None, Some(1), Some(1)]];
// DEFAULT-NEXT:     type @type3 T = struct {
// DEFAULT-NEXT:         field0 a: u32 : 16;
// DEFAULT-NEXT:         field1 b: u32 : 8;
// DEFAULT-NEXT:         field2 c: u32 : 8;
// DEFAULT-NEXT:         field3 d: @type0;
// DEFAULT-NEXT:         field4 e: u32;
// DEFAULT-NEXT:         field5 f: @type1;
// DEFAULT-NEXT:         field6 g: u32 : 5;
// DEFAULT-NEXT:         field7 h: u32 : 27;
// DEFAULT-NEXT:     } [size=44, align=4, offsets=[0, 2, 3, 4, 20, 24, 40, 40], bit_offsets=[Some(0), Some(16), Some(24), None, None, None, Some(320), Some(325)], bit_units=[(0, 4), (40, 4)], field_units=[Some(0), Some(0), Some(0), None, None, None, Some(1), Some(1)]];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %24 .str24: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %26 .str26: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %30 .str30: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %4 @foo(%5 p: ptr<@type2>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(deref(read<ptr<@type2>>(%5))), reinterpret<u32, reason=assign, fits=always>(const<i32>(231)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(deref(read<ptr<@type2>>(%5))), reinterpret<u32, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         write<@type0>(field3(deref(read<ptr<@type2>>(%5))), copy<@type0, reason=assign>(read<@type0>(compound_literal %13 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0])))));
// DEFAULT-NEXT:         write<u32>(field4(deref(read<ptr<@type2>>(%5))), const<u32>(3735928559));
// DEFAULT-NEXT:         write<@type1>(field5(deref(read<ptr<@type2>>(%5))), copy<@type1, reason=assign>(read<@type1>(compound_literal %14 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar(%7 p: ptr<@type2>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(deref(read<ptr<@type2>>(%7))), reinterpret<u32, reason=assign, fits=always>(const<i32>(231)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(deref(read<ptr<@type2>>(%7))), reinterpret<u32, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         write<@type0>(field3(deref(read<ptr<@type2>>(%7))), copy<@type0, reason=assign>(read<@type0>(compound_literal %15 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0])))));
// DEFAULT-NEXT:         write<u32>(field4(deref(read<ptr<@type2>>(%7))), const<u32>(3735928559));
// DEFAULT-NEXT:         write<@type1>(field5(deref(read<ptr<@type2>>(%7))), copy<@type1, reason=assign>(read<@type1>(compound_literal %16 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0])))));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(deref(read<ptr<@type2>>(%7))), reinterpret<u32, reason=assign, fits=always>(const<i32>(12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @baz(%9 p: ptr<@type3>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(deref(read<ptr<@type3>>(%9))), reinterpret<u32, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         write<@type0>(field3(deref(read<ptr<@type3>>(%9))), copy<@type0, reason=assign>(read<@type0>(compound_literal %17 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = code_units<array<i8, 16>>([97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 0])))));
// DEFAULT-NEXT:         write<u32>(field4(deref(read<ptr<@type3>>(%9))), const<u32>(3735928559));
// DEFAULT-NEXT:         write<@type1>(field5(deref(read<ptr<@type3>>(%9))), copy<@type1, reason=assign>(read<@type1>(compound_literal %18 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = code_units<array<i8, 16>>([65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 0])))));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(deref(read<ptr<@type3>>(%9))), reinterpret<u32, reason=assign, fits=always>(const<i32>(12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(const<i32>(8), const<i32>(8)), ne<i32>(const<i32>(4), const<i32>(4)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %11 s: @type2 [storage=automatic] = aggregate<@type2, zero_fill=true>();
// DEFAULT-NEXT:         let %12 t: @type3 [storage=automatic] = aggregate<@type3, zero_fill=true>();
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>) -> void>(%4, addr_of<ptr<@type2>>(%11));
// DEFAULT-NEXT:         let %31: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%11))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(%11))), const<i32>(231))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(%11))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%31, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%31, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%11)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%19)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %32: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%31), ne<u32>(read<u32>(field4(%11)), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%32, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%32, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%11)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%20)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%32), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%11))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%11))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%11)), const<i32>(0), const<u64>(44));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%11), reinterpret<u32, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%11), reinterpret<u32, reason=assign, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         write<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%11), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>) -> void>(%4, addr_of<ptr<@type2>>(%11));
// DEFAULT-NEXT:         let %33: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%11))), const<i32>(7)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(%11))), const<i32>(231))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(%11))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%33, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%33, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%11)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%21)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %34: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%33), ne<u32>(read<u32>(field4(%11)), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%34, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%34, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%11)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%22)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%34), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%11))), const<i32>(31))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%11)))), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%11)), const<i32>(0), const<u64>(44));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>) -> void>(%6, addr_of<ptr<@type2>>(%11));
// DEFAULT-NEXT:         let %35: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%11))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(%11))), const<i32>(231))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(%11))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%35, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%35, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%11)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%23)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %36: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%35), ne<u32>(read<u32>(field4(%11)), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%36, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%36, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%11)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%24)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%36), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%11))), const<i32>(12))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%11))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%11)), const<i32>(0), const<u64>(44));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%11), reinterpret<u32, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%11), reinterpret<u32, reason=assign, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         write<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%11), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>) -> void>(%6, addr_of<ptr<@type2>>(%11));
// DEFAULT-NEXT:         let %37: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..3>(%11))), const<i32>(7)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=3..11>(%11))), const<i32>(231))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=11..32>(%11))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%37, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%37, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%11)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%25)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %38: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%37), ne<u32>(read<u32>(field4(%11)), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%38, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%38, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%11)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%26)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%38), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%11))), const<i32>(12))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%11)))), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>) -> void>(%8, addr_of<ptr<@type3>>(%12));
// DEFAULT-NEXT:         let %39: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%12))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%12))), const<i32>(0))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%12))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%39, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%39, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%12)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%27)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %40: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%39), ne<u32>(read<u32>(field4(%12)), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%40, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%40, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%12)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%28)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%40), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%12))), const<i32>(12))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%12))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%11)), const<i32>(0), const<u64>(44));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%12), reinterpret<u32, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%12), reinterpret<u32, reason=assign, fits=always>(const<i32>(255)));
// DEFAULT-NEXT:         write<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%12), reinterpret<u32, reason=assign, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         write<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%12), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>) -> void>(%8, addr_of<ptr<@type3>>(%12));
// DEFAULT-NEXT:         let %41: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%12))), const<i32>(7)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%12))), const<i32>(255))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%12))), const<i32>(42)))
// DEFAULT-NEXT:             write<bool>(%41, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%41, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field3(%12)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%29)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         let %42: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%41), ne<u32>(read<u32>(field4(%12)), const<u32>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%42, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%42, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<array<i8, 16>>>(field0(field5(%12)))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%30)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%42), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield6<unit=1, bytes=40..44, bits=0..5>(%12))), const<i32>(12))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield7<unit=1, bytes=40..44, bits=5..32>(%12)))), sub<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(1), const<i32>(27)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
