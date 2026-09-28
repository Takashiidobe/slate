#include <stdio.h>

struct event {
  int type;
  union {
    struct {
      char *value;
    } alias;
    struct {
      char *handle;
      char *suffix;
    } tag;
  } data;
};

int main(void) {
  struct event e;
  e.type            = 1;
  char h[]          = "H";
  char s[]          = "S";
  e.data.tag.handle = h;
  e.data.tag.suffix = s;
  printf("%d %s%s\n", e.type, e.data.tag.handle, e.data.tag.suffix);
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
// DEFAULT-NEXT:     type @type0 event = struct {
// DEFAULT-NEXT:         field0 type: i32;
// DEFAULT-NEXT:         field1 data: @type1;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 alias: @type2;
// DEFAULT-NEXT:         field1 tag: @type3;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 value: ptr<i8>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 handle: ptr<i8>;
// DEFAULT-NEXT:         field1 suffix: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([37, 100, 32, 37, 115, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 e: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%6), const<i32>(1));
// DEFAULT-NEXT:         let %7 h: array<i8, 2> [storage=automatic] = code_units<array<i8, 2>>([72, 0]);
// DEFAULT-NEXT:         let %8 s: array<i8, 2> [storage=automatic] = code_units<array<i8, 2>>([83, 0]);
// DEFAULT-NEXT:         write<ptr<i8>>(field0(field1(field1(%6))), array_decay<ptr<i8>, length=Some(2)>(%7));
// DEFAULT-NEXT:         write<ptr<i8>>(field1(field1(field1(%6))), array_decay<ptr<i8>, length=Some(2)>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%10)), read<i32>(field0(%6)), read<ptr<i8>>(field0(field1(field1(%6)))), read<ptr<i8>>(field1(field1(field1(%6)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
