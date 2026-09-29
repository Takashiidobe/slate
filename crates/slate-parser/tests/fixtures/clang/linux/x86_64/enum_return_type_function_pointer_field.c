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
// DEFAULT-NEXT:     type @type[[TYPE_Status:[0-9]+]] Status = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_STATUS_OK:[0-9]+]] STATUS_OK = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_STATUS_FAIL:[0-9]+]] STATUS_FAIL = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Processor:[0-9]+]] Processor = fn(i32) -> @type[[TYPE_Status]];
// DEFAULT-NEXT:     type @type[[TYPE_Dispatcher:[0-9]+]] Dispatcher = struct {
// DEFAULT-NEXT:         field0 run: ptr<fn(i32) -> @type[[TYPE_Status]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_lastCode:[0-9]+]] lastCode: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_STATUS_FAIL]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_succeed:[0-9]+]] @succeed(%[[VALUE_x:[0-9]+]] x: i32) -> @type[[TYPE_Status]] [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_lastCode]], add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(100)));
// DEFAULT-NEXT:         return int_to_enum<@type[[TYPE_Status]], reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fail:[0-9]+]] @fail(%[[VALUE_x_2:[0-9]+]] x: i32) -> @type[[TYPE_Status]] [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_lastCode]], add<i32, overflow=ub>(read<i32>(%[[VALUE_x_2]]), const<i32>(200)));
// DEFAULT-NEXT:         return int_to_enum<@type[[TYPE_Status]], reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: @type[[TYPE_Dispatcher]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(i32) -> @type[[TYPE_Status]]>>(field0(%[[VALUE_d]]), function_decay<ptr<fn(i32) -> @type[[TYPE_Status]]>>(%[[VALUE_succeed]]));
// DEFAULT-NEXT:         call<@type[[TYPE_Status]], signature=fn(i32) -> @type[[TYPE_Status]]>(read<ptr<fn(i32) -> @type[[TYPE_Status]]>>(field0(%[[VALUE_d]])), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = read<i32>(%[[VALUE_lastCode]]);
// DEFAULT-NEXT:         write<ptr<fn(i32) -> @type[[TYPE_Status]]>>(field0(%[[VALUE_d]]), function_decay<ptr<fn(i32) -> @type[[TYPE_Status]]>>(%[[VALUE_fail]]));
// DEFAULT-NEXT:         call<@type[[TYPE_Status]], signature=fn(i32) -> @type[[TYPE_Status]]>(read<ptr<fn(i32) -> @type[[TYPE_Status]]>>(field0(%[[VALUE_d]])), const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = read<i32>(%[[VALUE_lastCode]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_STATUS_FAIL]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
