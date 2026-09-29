#include <ctype.h>
#include <stdio.h>

int classify(char c) { return isalpha(c); }

int main(void) {
  if (classify('A')) {
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE__ISupper:[0-9]+]] _ISupper = const<i32>(256);
// DEFAULT-NEXT:         %[[VALUE__ISlower:[0-9]+]] _ISlower = const<i32>(512);
// DEFAULT-NEXT:         %[[VALUE__ISalpha:[0-9]+]] _ISalpha = const<i32>(1024);
// DEFAULT-NEXT:         %[[VALUE__ISdigit:[0-9]+]] _ISdigit = const<i32>(2048);
// DEFAULT-NEXT:         %[[VALUE__ISxdigit:[0-9]+]] _ISxdigit = const<i32>(4096);
// DEFAULT-NEXT:         %[[VALUE__ISspace:[0-9]+]] _ISspace = const<i32>(8192);
// DEFAULT-NEXT:         %[[VALUE__ISprint:[0-9]+]] _ISprint = const<i32>(16384);
// DEFAULT-NEXT:         %[[VALUE__ISgraph:[0-9]+]] _ISgraph = const<i32>(32768);
// DEFAULT-NEXT:         %[[VALUE__ISblank:[0-9]+]] _ISblank = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE__IScntrl:[0-9]+]] _IScntrl = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE__ISpunct:[0-9]+]] _ISpunct = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE__ISalnum:[0-9]+]] _ISalnum = const<i32>(8);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([121, 101, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([110, 111, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___ctype_b_loc:[0-9]+]] @__ctype_b_loc() -> ptr<ptr<const u16>> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_classify:[0-9]+]] @classify(%[[VALUE_c:[0-9]+]] c: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(read<ptr<const u16>>(deref(call<ptr<ptr<const u16>>, signature=fn() -> ptr<ptr<const u16>>>(%[[VALUE___ctype_b_loc]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_c]]))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(1024))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i8) -> i32>(%[[VALUE_classify]], truncate<i8, reason=arg, fits=always>(const<i32>(65))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
