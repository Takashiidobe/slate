#include <stdio.h>
#include <stdlib.h>

typedef void (*test_fn)(void);

typedef struct {
  test_fn *tests;
  int      ntests;
} suite_t;

static int last_ran = -1;

static void test_a(void) { last_ran = 0; }

static void test_b(void) { last_ran = 1; }

int main(void) {
  suite_t suite;
  suite.tests    = malloc(2 * sizeof(test_fn));
  suite.tests[0] = test_a;
  suite.tests[1] = test_b;
  suite.ntests   = 2;

  for (int i = 0; i < suite.ntests; i++) {
    suite.tests[i]();
    printf("%d\n", last_ran);
  }

  free(suite.tests);
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 test_fn = ptr<fn() -> void>;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 tests: ptr<ptr<fn() -> void>>;
// DEFAULT-NEXT:         field1 ntests: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 suite_t = @type2;
// DEFAULT-NEXT:     global %7 last_ran: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%13 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @malloc(%14 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @free(%15 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @test_a() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test_b() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 suite: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<ptr<fn() -> void>>>(field0(%11), pointer_cast<ptr<ptr<fn() -> void>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8)))));
// DEFAULT-NEXT:         pointer_cast<ptr<ptr<fn() -> void>>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))), const<u64>(8))));
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(deref(ptr_offset<ptr<ptr<fn() -> void>>, subtract=false, element=ptr<fn() -> void>, overflow=ub>(read<ptr<ptr<fn() -> void>>>(field0(%11)), const<i32>(0))), function_decay<ptr<fn() -> void>>(%8));
// DEFAULT-NEXT:         write<ptr<fn() -> void>>(deref(ptr_offset<ptr<ptr<fn() -> void>>, subtract=false, element=ptr<fn() -> void>, overflow=ub>(read<ptr<ptr<fn() -> void>>>(field0(%11)), const<i32>(1))), function_decay<ptr<fn() -> void>>(%9));
// DEFAULT-NEXT:         write<i32>(field1(%11), const<i32>(2));
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %12 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), read<i32>(field1(%11)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(read<ptr<fn() -> void>>(deref(ptr_offset<ptr<ptr<fn() -> void>>, subtract=false, element=ptr<fn() -> void>, overflow=ub>(read<ptr<ptr<fn() -> void>>>(field0(%11)), read<i32>(%12)))));
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), read<i32>(%7));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<ptr<fn() -> void>>>(field0(%11))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
