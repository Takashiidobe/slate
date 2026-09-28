#include <stdio.h>

enum Status { STATUS_OK, STATUS_FAIL };

typedef enum Status Processor(int x);

struct Dispatcher {
  Processor *run;
};

static int lastCode = -1;

static enum Status succeed(int x) {
  lastCode = x + 100;
  return STATUS_OK;
}

static enum Status fail(int x) {
  lastCode = x + 200;
  return STATUS_FAIL;
}

int main(void) {
  struct Dispatcher d;
  d.run = succeed;
  d.run(1);
  int a = lastCode;
  d.run = fail;
  d.run(2);
  int b = lastCode;
  printf("%d %d\n", a, b);
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
// DEFAULT-NEXT:     type @type0 Status = enum : u32 {
// DEFAULT-NEXT:         %0 STATUS_OK = const<i32>(0);
// DEFAULT-NEXT:         %1 STATUS_FAIL = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 Processor = fn(i32) -> @type0;
// DEFAULT-NEXT:     type @type2 Dispatcher = struct {
// DEFAULT-NEXT:         field0 run: ptr<fn(i32) -> @type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %6 lastCode: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%15 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @succeed(%8 x: i32) -> @type0 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%6, add<i32, overflow=ub>(read<i32>(%8), const<i32>(100)));
// DEFAULT-NEXT:         return int_to_enum<@type0, reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @fail(%10 x: i32) -> @type0 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%6, add<i32, overflow=ub>(read<i32>(%10), const<i32>(200)));
// DEFAULT-NEXT:         return int_to_enum<@type0, reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 d: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(i32) -> @type0>>(field0(%12), function_decay<ptr<fn(i32) -> @type0>>(%7));
// DEFAULT-NEXT:         call<@type0, signature=fn(i32) -> @type0>(read<ptr<fn(i32) -> @type0>>(field0(%12)), const<i32>(1));
// DEFAULT-NEXT:         let %13 a: i32 [storage=automatic] = read<i32>(%6);
// DEFAULT-NEXT:         write<ptr<fn(i32) -> @type0>>(field0(%12), function_decay<ptr<fn(i32) -> @type0>>(%9));
// DEFAULT-NEXT:         call<@type0, signature=fn(i32) -> @type0>(read<ptr<fn(i32) -> @type0>>(field0(%12)), const<i32>(2));
// DEFAULT-NEXT:         let %14 b: i32 [storage=automatic] = read<i32>(%6);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%16)), read<i32>(%13), read<i32>(%14));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
