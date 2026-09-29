#include <stddef.h>
#include <stdio.h>

struct __attribute__((aligned(16))) Aligned {
  char a;
  int  b;
};

static int next_value(void) { return 9; }

static int effectful_case(void) {
  struct Aligned effectful;
  effectful.a = next_value();
  effectful.b = 7;
  return effectful.a + effectful.b;
}

static int repeated_case(void) {
  struct Aligned repeated;
  repeated.a = 1;
  repeated.a = 2;
  repeated.b = 3;
  return repeated.a + repeated.b;
}

static int dependent_case(void) {
  struct Aligned dependent;
  dependent.b = 8;
  dependent.a = dependent.b;
  return dependent.a + dependent.b;
}

static int counter;

static void touch(void) { counter++; }

static int interrupted_case(void) {
  struct Aligned interrupted;
  interrupted.a = 4;
  touch();
  interrupted.b = 6;
  return interrupted.a + interrupted.b + counter;
}

int main(void) {
  struct Aligned s;
  s.a = 5;
  s.b = 0x1234;

  printf("%zu %zu\n", sizeof(struct Aligned), _Alignof(struct Aligned));
  printf("%zu %zu\n", offsetof(struct Aligned, a), offsetof(struct Aligned, b));
  printf("%d %x\n", s.a, s.b);
  printf("%d %d %d %d\n", effectful_case(), repeated_case(), dependent_case(),
         interrupted_case());
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
// DEFAULT-NEXT:     type @type[[TYPE_Aligned:[0-9]+]] Aligned = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_counter:[0-9]+]] counter: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([37, 122, 117, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([37, 122, 117, 32, 37, 122, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 120, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_next_value:[0-9]+]] @next_value() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_effectful_case:[0-9]+]] @effectful_case() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_effectful:[0-9]+]] effectful: @type[[TYPE_Aligned]] [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(%[[VALUE_effectful]]), truncate<i8, reason=assign, fits=unknown>(call<i32, signature=fn() -> i32>(%[[VALUE_next_value]])));
// DEFAULT-NEXT:         truncate<i8, reason=assign, fits=unknown>(call<i32, signature=fn() -> i32>(%[[VALUE_next_value]]));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_effectful]]), const<i32>(7));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_effectful]]))), read<i32>(field1(%[[VALUE_effectful]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_repeated_case:[0-9]+]] @repeated_case() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_repeated:[0-9]+]] repeated: @type[[TYPE_Aligned]] [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(%[[VALUE_repeated]]), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(field0(%[[VALUE_repeated]]), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_repeated]]), const<i32>(3));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_repeated]]))), read<i32>(field1(%[[VALUE_repeated]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dependent_case:[0-9]+]] @dependent_case() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_dependent:[0-9]+]] dependent: @type[[TYPE_Aligned]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_dependent]]), const<i32>(8));
// DEFAULT-NEXT:         write<i8>(field0(%[[VALUE_dependent]]), truncate<i8, reason=assign, fits=unknown>(read<i32>(field1(%[[VALUE_dependent]]))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_dependent]]))), read<i32>(field1(%[[VALUE_dependent]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_touch:[0-9]+]] @touch() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_counter]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_counter]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_interrupted_case:[0-9]+]] @interrupted_case() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_interrupted:[0-9]+]] interrupted: @type[[TYPE_Aligned]] [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(%[[VALUE_interrupted]]), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_touch]]);
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_interrupted]]), const<i32>(6));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_interrupted]]))), read<i32>(field1(%[[VALUE_interrupted]]))), read<i32>(%[[VALUE_counter]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_Aligned]] [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(%[[VALUE_s]]), truncate<i8, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_s]]), const<i32>(4660));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str]])), const<u64>(16), const<u64>(16));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_2]])), const<u64>(0), const<u64>(4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_3]])), widen<i32, reason=vararg>(read<i8>(field0(%[[VALUE_s]]))), read<i32>(field1(%[[VALUE_s]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_4]])), call<i32, signature=fn() -> i32>(%[[VALUE_effectful_case]]), call<i32, signature=fn() -> i32>(%[[VALUE_repeated_case]]), call<i32, signature=fn() -> i32>(%[[VALUE_dependent_case]]), call<i32, signature=fn() -> i32>(%[[VALUE_interrupted_case]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
