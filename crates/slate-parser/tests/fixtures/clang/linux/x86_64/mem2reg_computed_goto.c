#include <stdio.h>

void walk(int *position) {
  static void *labels[] = {&&again, &&done};
  goto        *labels[*position];
again:
  position++;
  goto *labels[*position];
done:
  return;
}

int main(void) {
  int path[] = {0, 1};
  walk(path);
  printf("done\n");
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
// DEFAULT-NEXT:     global %6 labels: array<ptr<void>, 2> [storage=static] [align=16] = aggregate<array<ptr<void>, 2>, zero_fill=false>(index0 = label_addr<ptr<void>>(%3), index1 = label_addr<ptr<void>>(%4)) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([100, 111, 110, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @walk(%5 position: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%6), read<i32>(deref(read<ptr<i32>>(%5))))));
// DEFAULT-NEXT:         label %3 again:
// DEFAULT-NEXT:             let %11: ptr<i32> [synthetic] = read<ptr<i32>>(%5);
// DEFAULT-NEXT:             let %12: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%11), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i32>>(%5, read<ptr<i32>>(%12));
// DEFAULT-NEXT:         goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%6), read<i32>(deref(read<ptr<i32>>(%5))))));
// DEFAULT-NEXT:         label %4 done:
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 path: array<i32, 2> [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%2, array_decay<ptr<i32>, length=Some(2)>(%8));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%10)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
