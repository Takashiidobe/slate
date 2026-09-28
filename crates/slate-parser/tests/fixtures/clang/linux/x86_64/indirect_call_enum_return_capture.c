#include <stdio.h>

typedef enum { E_OK = 0, E_FAIL = 1 } Status;

typedef Status (*Handler)(int);

struct Parser {
  Handler processor;
};

static Status handle_ok(int x) {
  (void)x;
  return E_OK;
}

static Status handle_fail(int x) {
  (void)x;
  return E_FAIL;
}

int main(void) {
  struct Parser p;
  for (int i = 0; i < 2; i++) {
    p.processor   = (i == 0) ? handle_ok : handle_fail;
    Status result = p.processor(5);
    if (result == E_OK) {
      printf("ok\n");
    } else {
      printf("fail\n");
    }
  }
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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 E_OK = const<i32>(0);
// DEFAULT-NEXT:         %1 E_FAIL = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 Status = @type0;
// DEFAULT-NEXT:     type @type2 Handler = ptr<fn(i32) -> @type0>;
// DEFAULT-NEXT:     type @type3 Parser = struct {
// DEFAULT-NEXT:         field0 processor: ptr<fn(i32) -> @type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([111, 107, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 97, 105, 108, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%16 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @handle_ok(%9 x: i32) -> @type0 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<i32>(%9);
// DEFAULT-NEXT:         return int_to_enum<@type0, reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @handle_fail(%11 x: i32) -> @type0 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         read<i32>(%11);
// DEFAULT-NEXT:         return int_to_enum<@type0, reason=return>(reinterpret<u32, reason=return, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 p: @type3 [storage=automatic];
// DEFAULT-NEXT:         for %17
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %14 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%14), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%21));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<fn(i32) -> @type0>>(field0(%13), conditional<ptr<fn(i32) -> @type0>>(eq<i32>(read<i32>(%14), const<i32>(0)), function_decay<ptr<fn(i32) -> @type0>>(%8), function_decay<ptr<fn(i32) -> @type0>>(%10)));
// DEFAULT-NEXT:                     let %15 result: @type0 [storage=automatic] = call<@type0, signature=fn(i32) -> @type0>(read<ptr<fn(i32) -> @type0>>(field0(%13)), const<i32>(5));
// DEFAULT-NEXT:                     if eq<u32>(enum_to_int<u32, reason=promotion>(read<@type0>(%15)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%18)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%19)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
