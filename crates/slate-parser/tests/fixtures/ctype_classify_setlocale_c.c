#include <ctype.h>
#include <locale.h>
#include <stdio.h>

int main(void) {
  setlocale(LC_ALL, "C");
  char c = 'A';
  if (isalpha(c)) {
    printf("yes\n");
  } else {
    printf("no\n");
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
// DEFAULT-NEXT:         %0 _ISupper = const<i32>(256);
// DEFAULT-NEXT:         %1 _ISlower = const<i32>(512);
// DEFAULT-NEXT:         %2 _ISalpha = const<i32>(1024);
// DEFAULT-NEXT:         %3 _ISdigit = const<i32>(2048);
// DEFAULT-NEXT:         %4 _ISxdigit = const<i32>(4096);
// DEFAULT-NEXT:         %5 _ISspace = const<i32>(8192);
// DEFAULT-NEXT:         %6 _ISprint = const<i32>(16384);
// DEFAULT-NEXT:         %7 _ISgraph = const<i32>(32768);
// DEFAULT-NEXT:         %8 _ISblank = const<i32>(1);
// DEFAULT-NEXT:         %9 _IScntrl = const<i32>(2);
// DEFAULT-NEXT:         %10 _ISpunct = const<i32>(4);
// DEFAULT-NEXT:         %11 _ISalnum = const<i32>(8);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([67, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %13 @__ctype_b_loc() -> ptr<ptr<const u16>> [linkage=external];
// DEFAULT-NEXT:     fn %14 @setlocale(%18 __category: i32, %19 __locale: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %15 @printf(%20 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(i32, ptr<const i8>) -> ptr<i8>>(%14, const<i32>(6), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%21)));
// DEFAULT-NEXT:         let %17 c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(65));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%13))), widen<i32, reason=explicit>(read<i8>(%17))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1024)))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%15, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%22)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%15, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%23)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
