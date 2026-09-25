#include <stdio.h>

typedef unsigned char yaml_char_t;
typedef int           yaml_read_handler_t(void *, yaml_char_t *, unsigned long,
                                          unsigned long *);

typedef struct {
  yaml_read_handler_t *read_handler;
  void                *read_handler_data;
} parser_t;

static int read_bytes(void *data, yaml_char_t *buffer, unsigned long size,
                      unsigned long *size_read) {
  yaml_char_t *source = (yaml_char_t *)data;
  for (unsigned long i = 0; i < size; i++) {
    buffer[i] = source[i];
  }
  *size_read = size;
  return 1;
}

int main(void) {
  yaml_char_t   input[]   = "abc";
  yaml_char_t   tag[]     = "tag:yaml.org,2002:str";
  yaml_char_t   buffer[4] = {0};
  unsigned long size_read = 0;
  parser_t      parser;
  parser.read_handler      = read_bytes;
  parser.read_handler_data = input;
  int ok = parser.read_handler(parser.read_handler_data, buffer, 3, &size_read);
  printf("%d %lu %c %c\n", ok, size_read, buffer[1], tag[4]);
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
// DEFAULT-NEXT:     type @type0 yaml_char_t = u8;
// DEFAULT-NEXT:     type @type1 yaml_read_handler_t = fn(ptr<void>, ptr<u8>, u64, ptr<u64>) -> i32;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 read_handler: ptr<fn(ptr<void>, ptr<u8>, u64, ptr<u64>) -> i32>;
// DEFAULT-NEXT:         field1 read_handler_data: ptr<void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 parser_t = @type2;
// DEFAULT-NEXT:     global %21 .str21: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([37, 100, 32, 37, 108, 117, 32, 37, 99, 32, 37, 99, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%19 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @read_bytes(%6 data: ptr<void>, %7 buffer: ptr<u8>, %8 size: u64, %9 size_read: ptr<u64>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 source: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=explicit>(read<ptr<void>>(%6));
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %11 i: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%11), read<u64>(%8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %22: u64 [synthetic] = read<u64>(%11);
// DEFAULT-NEXT:                 let %23: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%22), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%11, read<u64>(%23));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%7), read<u64>(%11))), read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%10), read<u64>(%11)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%9)), read<u64>(%8));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 input: array<u8, 4> [storage=automatic] = code_units<array<u8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %14 tag: array<u8, 22> [storage=automatic] = code_units<array<u8, 22>>([116, 97, 103, 58, 121, 97, 109, 108, 46, 111, 114, 103, 44, 50, 48, 48, 50, 58, 115, 116, 114, 0]);
// DEFAULT-NEXT:         let %15 buffer: array<u8, 4> [storage=automatic] = aggregate<array<u8, 4>, zero_fill=true>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %16 size_read: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %17 parser: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(ptr<void>, ptr<u8>, u64, ptr<u64>) -> i32>>(field0(%17), function_decay<ptr<fn(ptr<void>, ptr<u8>, u64, ptr<u64>) -> i32>>(%5));
// DEFAULT-NEXT:         write<ptr<void>>(field1(%17), pointer_cast<ptr<void>, reason=assign>(array_decay<ptr<u8>, length=Some(4)>(%13)));
// DEFAULT-NEXT:         let %18 ok: i32 [storage=automatic] = call<i32, signature=fn(ptr<void>, ptr<u8>, u64, ptr<u64>) -> i32>(read<ptr<fn(ptr<void>, ptr<u8>, u64, ptr<u64>) -> i32>>(field0(%17)), read<ptr<void>>(field1(%17)), array_decay<ptr<u8>, length=Some(4)>(%15), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))), addr_of<ptr<u64>>(%16));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%21)), read<i32>(%18), read<u64>(%16), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(%15), const<i32>(1)))))), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(22)>(%14), const<i32>(4)))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
