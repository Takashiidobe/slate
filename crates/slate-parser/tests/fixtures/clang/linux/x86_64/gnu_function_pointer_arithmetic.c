#include <stddef.h>
#include <stdio.h>

static int target(void) { return 42; }

int main(void) {
  int       (*base)(void) = target;
  ptrdiff_t forward       = (base + 3) - base;
  ptrdiff_t backward      = (base - 2) - base;
  ptrdiff_t difference    = base - (base + 3);
  int       unchanged     = base + 0 == base;
  printf("%td %td %td %d\n", forward, backward, difference, unchanged);
  return 0;
}

// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES IR

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 ptrdiff_t = i64;
// IR-NEXT:     global %10 .str10: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 116, 100, 32, 37, 116, 100, 32, 37, 116, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// IR-NEXT:     fn %1 @printf(%9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// IR-NEXT:     fn %2 @target() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(42);
// IR-NEXT:     }
// IR-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// IR-NEXT:         let %4 base: ptr<fn() -> i32> [storage=automatic] = function_decay<ptr<fn() -> i32>>(%2);
// IR-NEXT:         let %5 forward: i64 [storage=automatic] = ptr_diff<i64, element=fn() -> i32, same_array=required, overflow=ub>(ptr_offset<ptr<fn() -> i32>, subtract=false, element=fn() -> i32, overflow=ub>(read<ptr<fn() -> i32>>(%4), const<i32>(3)), read<ptr<fn() -> i32>>(%4));
// IR-NEXT:         let %6 backward: i64 [storage=automatic] = ptr_diff<i64, element=fn() -> i32, same_array=required, overflow=ub>(ptr_offset<ptr<fn() -> i32>, subtract=true, element=fn() -> i32, overflow=ub>(read<ptr<fn() -> i32>>(%4), const<i32>(2)), read<ptr<fn() -> i32>>(%4));
// IR-NEXT:         let %7 difference: i64 [storage=automatic] = ptr_diff<i64, element=fn() -> i32, same_array=required, overflow=ub>(read<ptr<fn() -> i32>>(%4), ptr_offset<ptr<fn() -> i32>, subtract=false, element=fn() -> i32, overflow=ub>(read<ptr<fn() -> i32>>(%4), const<i32>(3)));
// IR-NEXT:         let %8 unchanged: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<ptr<fn() -> i32>>(ptr_offset<ptr<fn() -> i32>, subtract=false, element=fn() -> i32, overflow=ub>(read<ptr<fn() -> i32>>(%4), const<i32>(0)), read<ptr<fn() -> i32>>(%4)));
// IR-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%10)), read<i64>(%5), read<i64>(%6), read<i64>(%7), read<i32>(%8));
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
