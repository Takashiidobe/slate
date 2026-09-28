#include <string.h>

void abort(void);

void foo(int *BM_tab, int j) {
  int *BM_tab_base;

  BM_tab_base  = BM_tab;
  BM_tab      += 0400;
  while (BM_tab_base != BM_tab) {
    *--BM_tab = j;
    *--BM_tab = j;
    *--BM_tab = j;
    *--BM_tab = j;
  }
}

int main() {
  int BM_tab[0400];
  memset(BM_tab, 0, sizeof(BM_tab));
  foo(BM_tab, 6);
  if (BM_tab[0] != 6)
    abort();
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     fn %4 @memset(%12 __s: ptr<void>, %13 __c: i32, %14 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @foo(%7 BM_tab: ptr<i32>, %8 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 BM_tab_base: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%9, read<ptr<i32>>(%7));
// DEFAULT-NEXT:         let %16: ptr<i32> [synthetic] = read<ptr<i32>>(%7);
// DEFAULT-NEXT:         let %17: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%16), const<i32>(256));
// DEFAULT-NEXT:         write<ptr<i32>>(%7, read<ptr<i32>>(%17));
// DEFAULT-NEXT:         while %15 ne<ptr<i32>>(read<ptr<i32>>(%9), read<ptr<i32>>(%7))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %18: ptr<i32> [synthetic] = read<ptr<i32>>(%7);
// DEFAULT-NEXT:                 let %19: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%7, read<ptr<i32>>(%19));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%19)), read<i32>(%8));
// DEFAULT-NEXT:                 let %20: ptr<i32> [synthetic] = read<ptr<i32>>(%7);
// DEFAULT-NEXT:                 let %21: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%20), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%7, read<ptr<i32>>(%21));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%21)), read<i32>(%8));
// DEFAULT-NEXT:                 let %22: ptr<i32> [synthetic] = read<ptr<i32>>(%7);
// DEFAULT-NEXT:                 let %23: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%7, read<ptr<i32>>(%23));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%23)), read<i32>(%8));
// DEFAULT-NEXT:                 let %24: ptr<i32> [synthetic] = read<ptr<i32>>(%7);
// DEFAULT-NEXT:                 let %25: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%24), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%7, read<ptr<i32>>(%25));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%25)), read<i32>(%8));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 BM_tab: array<i32, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%4, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(256)>(%11)), const<i32>(0), const<u64>(1024));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%6, array_decay<ptr<i32>, length=Some(256)>(%11), const<i32>(6));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(256)>(%11), const<i32>(0)))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
