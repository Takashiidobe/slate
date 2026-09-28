#include <stdio.h>

struct inner {
  int start;
  int end;
};

struct outer {
  struct inner buf;
  int          error;
};

static void bump(int *p) { *p = *p + 10; }

static void init(struct outer *o) {
  o->buf.start = 1;
  o->buf.end   = 2;
  o->error     = 0;
  bump(&o->buf.start);
}

int main(void) {
  struct outer o;
  init(&o);
  printf("%d %d %d\n", o.buf.start, o.buf.end, o.error);
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
// DEFAULT-NEXT:     type @type0 inner = struct {
// DEFAULT-NEXT:         field0 start: i32;
// DEFAULT-NEXT:         field1 end: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 outer = struct {
// DEFAULT-NEXT:         field0 buf: @type0;
// DEFAULT-NEXT:         field1 error: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%10 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @bump(%5 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%5)), add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%5))), const<i32>(10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @init(%7 o: ptr<@type1>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(field0(field0(deref(read<ptr<@type1>>(%7)))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(field0(deref(read<ptr<@type1>>(%7)))), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type1>>(%7))), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%4, addr_of<ptr<i32>>(field0(field0(deref(read<ptr<@type1>>(%7))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 o: @type1 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%6, addr_of<ptr<@type1>>(%9));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%11)), read<i32>(field0(field0(%9))), read<i32>(field1(field0(%9))), read<i32>(field1(%9)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
