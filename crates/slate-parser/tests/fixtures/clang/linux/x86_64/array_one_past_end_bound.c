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
// DEFAULT-NEXT:     type @type0 cursor = struct {
// DEFAULT-NEXT:         field0 p: ptr<u8>;
// DEFAULT-NEXT:         field1 buf: array<u8, 8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%10 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @fill(%4 c: ptr<@type0>, %5 n: u8) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<u8>>(field0(deref(read<ptr<@type0>>(%4))), array_decay<ptr<u8>, length=Some(8)>(field1(deref(read<ptr<@type0>>(%4)))));
// DEFAULT-NEXT:         while %11 lt<ptr<u8>>(read<ptr<u8>>(field0(deref(read<ptr<@type0>>(%4)))), addr_of<ptr<u8>>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(deref(read<ptr<@type0>>(%4)))), const<u64>(8)))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u8>(deref(read<ptr<u8>>(field0(deref(read<ptr<@type0>>(%4))))), read<u8>(%5));
// DEFAULT-NEXT:                 let %14: u8 [synthetic] = read<u8>(%5);
// DEFAULT-NEXT:                 let %15: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%14))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u8>(%5, read<u8>(%15));
// DEFAULT-NEXT:                 let %16: ptr<@type0> [synthetic] = read<ptr<@type0>>(%4);
// DEFAULT-NEXT:                 let %17: ptr<u8> [synthetic] = read<ptr<u8>>(field0(deref(read<ptr<@type0>>(%16))));
// DEFAULT-NEXT:                 let %18: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%17), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(field0(deref(read<ptr<@type0>>(%16))), read<ptr<u8>>(%18));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 c: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, u8) -> void>(%3, addr_of<ptr<@type0>>(%7), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %8 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %9 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %21: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                     let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field1(%7)), read<i32>(%9)))))));
// DEFAULT-NEXT:                     write<i32>(%8, read<i32>(%22));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%13)), read<i32>(%8));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
