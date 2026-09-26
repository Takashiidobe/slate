struct C {
  unsigned int c;
  struct D {
    unsigned int columns       : 4;
    unsigned int fore          : 12;
    unsigned int back          : 6;
    unsigned int fragment      : 1;
    unsigned int standout      : 1;
    unsigned int underline     : 1;
    unsigned int strikethrough : 1;
    unsigned int reverse       : 1;
    unsigned int blink         : 1;
    unsigned int half          : 1;
    unsigned int bold          : 1;
    unsigned int invisible     : 1;
    unsigned int pad           : 1;
  } attr;
};

struct A {
  struct C    *data;
  unsigned int len;
};

struct B {
  struct A     *cells;
  unsigned char soft_wrapped : 1;
};

struct E {
  long     row, col;
  struct C defaults;
};

__attribute__((noinline)) void foo(struct E *screen, unsigned int c,
                                   int columns, struct B *row) {
  struct D attr;
  long     col;
  int      i;
  col                        = screen->col;
  attr                       = screen->defaults.attr;
  attr.columns               = columns;
  row->cells->data[col].c    = c;
  row->cells->data[col].attr = attr;
  col++;
  attr.fragment = 1;
  for (i = 1; i < columns; i++) {
    row->cells->data[col].c    = c;
    row->cells->data[col].attr = attr;
    col++;
  }
}

int main(void) {
  struct E e = {.row      = 5,
                .col      = 0,
                .defaults = {6, {-1, -1, -1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}}};
  struct C c[4];
  struct A a = {c, 4};
  struct B b = {&a, 1};
  struct D d;
  __builtin_memset(&c, 0, sizeof c);
  foo(&e, 65, 2, &b);
  d         = e.defaults.attr;
  d.columns = 2;
  if (__builtin_memcmp(&d, &c[0].attr, sizeof d))
    __builtin_abort();
  d.fragment = 1;
  if (__builtin_memcmp(&d, &c[1].attr, sizeof d))
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
// DEFAULT-NEXT:     type @type0 C = struct {
// DEFAULT-NEXT:         field0 c: u32;
// DEFAULT-NEXT:         field1 attr: @type1;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 D = struct {
// DEFAULT-NEXT:         field0 columns: u32 : 4;
// DEFAULT-NEXT:         field1 fore: u32 : 12;
// DEFAULT-NEXT:         field2 back: u32 : 6;
// DEFAULT-NEXT:         field3 fragment: u32 : 1;
// DEFAULT-NEXT:         field4 standout: u32 : 1;
// DEFAULT-NEXT:         field5 underline: u32 : 1;
// DEFAULT-NEXT:         field6 strikethrough: u32 : 1;
// DEFAULT-NEXT:         field7 reverse: u32 : 1;
// DEFAULT-NEXT:         field8 blink: u32 : 1;
// DEFAULT-NEXT:         field9 half: u32 : 1;
// DEFAULT-NEXT:         field10 bold: u32 : 1;
// DEFAULT-NEXT:         field11 invisible: u32 : 1;
// DEFAULT-NEXT:         field12 pad: u32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3], bit_offsets=[Some(0), Some(4), Some(16), Some(22), Some(23), Some(24), Some(25), Some(26), Some(27), Some(28), Some(29), Some(30), Some(31)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type2 A = struct {
// DEFAULT-NEXT:         field0 data: ptr<@type0>;
// DEFAULT-NEXT:         field1 len: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 B = struct {
// DEFAULT-NEXT:         field0 cells: ptr<@type2>;
// DEFAULT-NEXT:         field1 soft_wrapped: u8 : 1;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[None, Some(64)], bit_units=[(8, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     type @type4 E = struct {
// DEFAULT-NEXT:         field0 row: i64;
// DEFAULT-NEXT:         field1 col: i64;
// DEFAULT-NEXT:         field2 defaults: @type0;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     fn %5 @foo(%6 screen: ptr<@type4>, %7 c: u32, %8 columns: i32, %9 row: ptr<@type3>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 attr: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %11 col: i64 [storage=automatic];
// DEFAULT-NEXT:         let %12 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%11, read<i64>(field1(deref(read<ptr<@type4>>(%6)))));
// DEFAULT-NEXT:         write<@type1>(%10, copy<@type1, reason=assign>(read<@type1>(field1(field2(deref(read<ptr<@type4>>(%6)))))));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..4>(%10), reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%8)));
// DEFAULT-NEXT:         write<u32>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(field0(deref(read<ptr<@type2>>(field0(deref(read<ptr<@type3>>(%9))))))), read<i64>(%11)))), read<u32>(%7));
// DEFAULT-NEXT:         write<@type1>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(field0(deref(read<ptr<@type2>>(field0(deref(read<ptr<@type3>>(%9))))))), read<i64>(%11)))), copy<@type1, reason=assign>(read<@type1>(%10)));
// DEFAULT-NEXT:         let %20: i64 [synthetic] = read<i64>(%11);
// DEFAULT-NEXT:         let %21: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%20), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%11, read<i64>(%21));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=22..23>(%10), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         for %19
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%12, const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), read<i32>(%8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%23));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u32>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(field0(deref(read<ptr<@type2>>(field0(deref(read<ptr<@type3>>(%9))))))), read<i64>(%11)))), read<u32>(%7));
// DEFAULT-NEXT:                     write<@type1>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(field0(deref(read<ptr<@type2>>(field0(deref(read<ptr<@type3>>(%9))))))), read<i64>(%11)))), copy<@type1, reason=assign>(read<@type1>(%10)));
// DEFAULT-NEXT:                     let %24: i64 [synthetic] = read<i64>(%11);
// DEFAULT-NEXT:                     let %25: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%24), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i64>(%11, read<i64>(%25));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 e: @type4 [storage=automatic] = aggregate<@type4, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(5)), field1 = widen<i64, reason=assign>(const<i32>(0)), field2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(6)), field1 = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), field1 = reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), field2 = reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))), field3 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field4 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field5 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field6 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field7 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field8 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field9 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field10 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field11 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), field12 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         let %15 c: array<@type0, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %16 a: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = array_decay<ptr<@type0>, length=Some(4)>(%15), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         let %17 b: @type3 [storage=automatic] = aggregate<@type3, zero_fill=false>(field0 = addr_of<ptr<@type2>>(%16), field1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %18 d: @type1 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<array<@type0, 4>>>(%15)), const<i32>(0), const<u64>(32));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, u32, i32, ptr<@type3>) -> void>(%5, addr_of<ptr<@type4>>(%14), reinterpret<u32, reason=arg, fits=always>(const<i32>(65)), const<i32>(2), addr_of<ptr<@type3>>(%17));
// DEFAULT-NEXT:         write<@type1>(%18, copy<@type1, reason=assign>(read<@type1>(field1(field2(%14)))));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..4>(%18), reinterpret<u32, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%18)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(%15), const<i32>(0)))))), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=22..23>(%18), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%18)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(%15), const<i32>(1)))))), const<u64>(4)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
