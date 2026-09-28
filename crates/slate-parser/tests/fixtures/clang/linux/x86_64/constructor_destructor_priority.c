#include <stdio.h>

__attribute__((constructor(200))) static void init_late(void) {
  printf("ctor: late (200)\n");
}

__attribute__((constructor(101))) static void init_early(void) {
  printf("ctor: early (101)\n");
}

__attribute__((constructor)) static void init_default(void) {
  printf("ctor: default\n");
}

__attribute__((destructor(200))) static void fini_late(void) {
  printf("dtor: late (200)\n");
}

__attribute__((destructor(101))) static void fini_early(void) {
  printf("dtor: early (101)\n");
}

__attribute__((destructor)) static void fini_default(void) {
  printf("dtor: default\n");
}

int main(void) {
  printf("main\n");
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
// DEFAULT-NEXT:     global %10 .str10: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([99, 116, 111, 114, 58, 32, 108, 97, 116, 101, 32, 40, 50, 48, 48, 41, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([99, 116, 111, 114, 58, 32, 101, 97, 114, 108, 121, 32, 40, 49, 48, 49, 41, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([99, 116, 111, 114, 58, 32, 100, 101, 102, 97, 117, 108, 116, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([100, 116, 111, 114, 58, 32, 108, 97, 116, 101, 32, 40, 50, 48, 48, 41, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([100, 116, 111, 114, 58, 32, 101, 97, 114, 108, 121, 32, 40, 49, 48, 49, 41, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([100, 116, 111, 114, 58, 32, 100, 101, 102, 97, 117, 108, 116, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([109, 97, 105, 110, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @init_late() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @init_early() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @init_default() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @fini_late() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%13)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @fini_early() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%14)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @fini_default() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%16)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
