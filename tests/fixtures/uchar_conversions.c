#include <stdio.h>
#include <uchar.h>

int main(void) {
  mbstate_t state16        = {0};
  mbstate_t state32        = {0};
  char16_t  converted16    = 0;
  char32_t  converted32    = 0;
  char      multibyte16[4] = {0};
  char      multibyte32[4] = {0};

  size_t read16  = mbrtoc16(&converted16, "A", 1, &state16);
  size_t write16 = c16rtomb(multibyte16, u'A', &state16);
  size_t read32  = mbrtoc32(&converted32, "B", 1, &state32);
  size_t write32 = c32rtomb(multibyte32, U'B', &state32);

  printf("%zu %zu %u %d %zu %zu %u %d\n", read16, write16,
         (unsigned)converted16, multibyte16[0], read32, write32,
         (unsigned)converted32, multibyte32[0]);
  return 0;
}
// REWRITES-DAG: fn mbrtoc16(_0: *mut u16, _1: *const i8, _2: usize, _3: *mut libc::mbstate_t) -> usize;
// REWRITES-DAG: fn c16rtomb(_0: *mut i8, _1: u16, _2: *mut libc::mbstate_t) -> usize;
// REWRITES-DAG: fn mbrtoc32(_0: *mut u32, _1: *const i8, _2: usize, _3: *mut libc::mbstate_t) -> usize;
// REWRITES-DAG: fn c32rtomb(_0: *mut i8, _1: u32, _2: *mut libc::mbstate_t) -> usize;
// REWRITES-DAG: libc::mbstate_t = unsafe { std::mem::zeroed::<libc::mbstate_t>() };
// REWRITES-NOT: *mut __mbstate_t
