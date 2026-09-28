#include <stdbool.h>
#include <stdio.h>

int main(void) {
  bool flag   = false;
  int  first  = __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  int  second = __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  __atomic_clear(&flag, __ATOMIC_SEQ_CST);
  printf("%d %d %d\n", first, second, !flag);
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
// DEFAULT-NEXT:     global %7 .str7: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%6 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 flag: bool [storage=automatic] = const<bool>(false);
// DEFAULT-NEXT:         let %4 first: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8: bool [synthetic] = update<bool, result=old, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%3)), const<bool>(true));
// DEFAULT-NEXT:         write<i32>(%4, from_bool<i32, reason=assign>(read<bool>(%8)));
// DEFAULT-NEXT:         let %5 second: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9: bool [synthetic] = update<bool, result=old, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%3)), const<bool>(true));
// DEFAULT-NEXT:         write<i32>(%5, from_bool<i32, reason=assign>(read<bool>(%9)));
// DEFAULT-NEXT:         write<bool, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%3)), const<bool>(false));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%7)), read<i32>(%4), read<i32>(%5), from_bool<i32, reason=vararg>(not<bool>(read<bool>(%3))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
