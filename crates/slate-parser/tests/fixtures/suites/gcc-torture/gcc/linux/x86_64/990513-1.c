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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE___s:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_BM_tab:[0-9]+]] BM_tab: ptr<i32>, %[[VALUE_j:[0-9]+]] j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_BM_tab_base:[0-9]+]] BM_tab_base: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_BM_tab_base]], read<ptr<i32>>(%[[VALUE_BM_tab]]));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_BM_tab]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE0]]), const<i32>(256));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_BM_tab]], read<ptr<i32>>(%[[VALUE1]]));
// DEFAULT-NEXT:         while %[[VALUE2:[0-9]+]] ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_BM_tab_base]]), read<ptr<i32>>(%[[VALUE_BM_tab]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_BM_tab]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_BM_tab]], read<ptr<i32>>(%[[VALUE4]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE4]])), read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_BM_tab]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_BM_tab]], read<ptr<i32>>(%[[VALUE6]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE6]])), read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_BM_tab]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_BM_tab]], read<ptr<i32>>(%[[VALUE8]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE8]])), read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_BM_tab]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=true, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_BM_tab]], read<ptr<i32>>(%[[VALUE10]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE10]])), read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_BM_tab_2:[0-9]+]] BM_tab: array<i32, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(256)>(%[[VALUE_BM_tab_2]])), const<i32>(0), const<u64>(1024));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%[[VALUE_foo]], array_decay<ptr<i32>, length=Some(256)>(%[[VALUE_BM_tab_2]]), const<i32>(6));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(256)>(%[[VALUE_BM_tab_2]]), const<i32>(0)))), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
