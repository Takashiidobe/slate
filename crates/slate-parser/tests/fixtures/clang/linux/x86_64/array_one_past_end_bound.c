#include <stdio.h>

struct cursor {
  unsigned char *p;
  unsigned char  buf[8];
};

static void fill(struct cursor *c, unsigned char n) {
  c->p = c->buf;
  while (c->p < &c->buf[sizeof(c->buf)]) {
    *c->p = n;
    n++;
    c->p++;
  }
}

int main(void) {
  struct cursor c;
  fill(&c, 1);

  int total = 0;
  for (int i = 0; i < 8; i++) {
    total += c.buf[i];
  }
  printf("%d\n", total);
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
// DEFAULT-NEXT:     type @type[[TYPE_cursor:[0-9]+]] cursor = struct {
// DEFAULT-NEXT:         field0 p: ptr<u8>;
// DEFAULT-NEXT:         field1 buf: array<u8, 8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fill:[0-9]+]] @fill(%[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_cursor]]>, %[[VALUE_n:[0-9]+]] n: u8) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<u8>>(field0(deref(read<ptr<@type[[TYPE_cursor]]>>(%[[VALUE_c]]))), array_decay<ptr<u8>, length=Some(8)>(field1(deref(read<ptr<@type[[TYPE_cursor]]>>(%[[VALUE_c]])))));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] lt<ptr<u8>>(read<ptr<u8>>(field0(deref(read<ptr<@type[[TYPE_cursor]]>>(%[[VALUE_c]])))), addr_of<ptr<u8>>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(deref(read<ptr<@type[[TYPE_cursor]]>>(%[[VALUE_c]])))), const<u64>(8)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u8>(deref(read<ptr<u8>>(field0(deref(read<ptr<@type[[TYPE_cursor]]>>(%[[VALUE_c]]))))), read<u8>(%[[VALUE_n]]));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u8 [synthetic] = read<u8>(%[[VALUE_n]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE1]]))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u8>(%[[VALUE_n]], read<u8>(%[[VALUE2]]));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<@type[[TYPE_cursor]]> [synthetic] = read<ptr<@type[[TYPE_cursor]]>>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(field0(deref(read<ptr<@type[[TYPE_cursor]]>>(%[[VALUE3]]))));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(field0(deref(read<ptr<@type[[TYPE_cursor]]>>(%[[VALUE3]]))), read<ptr<u8>>(%[[VALUE5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: @type[[TYPE_cursor]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_cursor]]>, u8) -> void>(%[[VALUE_fill]], addr_of<ptr<@type[[TYPE_cursor]]>>(%[[VALUE_c_2]]), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%[[VALUE_c_2]])), read<i32>(%[[VALUE_i]])))))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), read<i32>(%[[VALUE_total]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
