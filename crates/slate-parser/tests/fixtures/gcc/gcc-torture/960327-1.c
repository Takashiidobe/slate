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
// DEFAULT-NEXT:     global %13 .str13: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([88, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @sprintf(%8 __s: ptr<i8> [restrict], %9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @exit(%10 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @g() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 s: array<i8, 14> [storage=automatic] = code_units<array<i8, 14>>([97, 98, 99, 101, 100, 102, 103, 48, 49, 50, 51, 52, 53, 0]);
// DEFAULT-NEXT:         let %6 sp: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(14)>(%5), const<i32>(12));
// DEFAULT-NEXT:         switch %11 call<i32, signature=fn() -> i32>(%3)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %11 const<i32>(10):
// DEFAULT-NEXT:                     break %11;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while %12 {
// DEFAULT-NEXT:             let %14: ptr<i8> [synthetic] = read<ptr<i8>>(%6);
// DEFAULT-NEXT:             let %15: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(read<ptr<i8>>(%14), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%6, read<ptr<i8>>(%15));
// DEFAULT-NEXT:             yield eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%15)))), const<i32>(48));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%0, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%6), const<i32>(1)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%13)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(14)>(%5), const<i32>(12))))), const<i32>(88))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
