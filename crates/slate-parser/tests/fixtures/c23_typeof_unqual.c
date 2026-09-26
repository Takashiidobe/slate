#include <stdio.h>

int main(void) {
  const volatile int qualified   = 9;
  typeof_unqual(qualified) copy  = qualified;
  typeof(qualified) preserved    = 12;
  int              *pointer      = nullptr;
  constexpr int     width        = 7;
  unsigned _BitInt(width) narrow = 100;
  int unqualified = __builtin_types_compatible_p(typeof(copy), int);
  int still_qualified =
      __builtin_types_compatible_p(typeof(preserved), const volatile int);
  printf("%d %d %d %d %d\n", copy, preserved, pointer == nullptr, (int)narrow,
         unqualified + still_qualified);
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
// DEFAULT-NEXT:     global %11 .str11: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%10 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 qualified: volatile i32 [storage=automatic] [const] = const<i32>(9);
// DEFAULT-NEXT:         let %3 copy: i32 [storage=automatic] = read<i32, volatile>(%2);
// DEFAULT-NEXT:         let %4 preserved: volatile i32 [storage=automatic] [const] = const<i32>(12);
// DEFAULT-NEXT:         let %5 pointer: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:         let %6 width: i32 [storage=automatic] [const] [constexpr] = const<i32>(7);
// DEFAULT-NEXT:         let %7 narrow: u7b [storage=automatic] = reinterpret<u7b, reason=assign, fits=unknown>(truncate<i7b, reason=assign, fits=unknown>(const<i32>(100)));
// DEFAULT-NEXT:         let %8 unqualified: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %9 still_qualified: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%11)), read<i32>(%3), read<i32, volatile>(%4), from_bool<i32, reason=vararg>(eq<ptr<i32>>(read<ptr<i32>>(%5), null<ptr<i32>>)), reinterpret<i32, reason=explicit, fits=unknown>(widen<u32, reason=explicit>(read<u7b>(%7))), add<i32, overflow=ub>(read<i32>(%8), read<i32>(%9)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
