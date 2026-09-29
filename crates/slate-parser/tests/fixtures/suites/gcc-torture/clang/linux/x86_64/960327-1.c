#include <stdio.h>

void abort(void);
void exit(int);

int g(void) { return '\n'; }

void f(void) {
  char  s[] = "abcedfg012345";
  char *sp  = s + 12;

  switch (g()) {
  case '\n':
    break;
  }

  while (*--sp == '0')
    ;
  sprintf(sp + 1, "X");

  if (s[12] != 'X')
    abort();
}

int main(void) {
  f();
  exit(0);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([88, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_sprintf:[0-9]+]] @sprintf(%[[VALUE___s:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: array<i8, 14> [storage=automatic] = code_units<array<i8, 14>>([97, 98, 99, 101, 100, 102, 103, 48, 49, 50, 51, 52, 53, 0]);
// DEFAULT-NEXT:         let %[[VALUE_sp:[0-9]+]] sp: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_s]]), const<i32>(12));
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] call<i32, signature=fn() -> i32>(%[[VALUE_g]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(10):
// DEFAULT-NEXT:                     break %[[VALUE1]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while %[[VALUE2:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_sp]]);
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%[[VALUE_sp]], read<ptr<i8>>(%[[VALUE4]]));
// DEFAULT-NEXT:             yield eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE4]])))), const<i32>(48));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sprintf]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_sp]]), const<i32>(1)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_s]]), const<i32>(12))))), const<i32>(88))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_f]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
