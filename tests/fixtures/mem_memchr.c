#include <stdio.h>

int main(void) {
  unsigned char       buf[8]    = {10, 20, 30, 40, 50, 60, 70, 80};
  const unsigned char cbuf[4]   = {9, 8, 7, 6};
  char                word[]    = "abc";
  int                 needle    = (int)buf[3];
  size_t volatile partial_count = 4;
  unsigned char *hit        = (unsigned char *)__builtin_memchr(buf, needle, 8);
  unsigned char *miss       = (unsigned char *)__builtin_memchr(buf, 99, 8);
  unsigned char *zero       = (unsigned char *)__builtin_memchr(buf, 10, 0);
  char          *nul_after  = (char *)__builtin_memchr(word, 0, sizeof word);
  char          *nul_equal  = (char *)__builtin_memchr(word, 0, 3);
  char          *nul_before = (char *)__builtin_memchr(word, 0, 2);
  unsigned char *partial =
      (unsigned char *)__builtin_memchr(buf, needle, partial_count);
  unsigned char *offset_base = buf + 2;
  unsigned char *offset =
      (unsigned char *)__builtin_memchr(offset_base, needle, 2);
  const unsigned char *const_hit =
      (const unsigned char *)__builtin_memchr(cbuf, 7, sizeof cbuf);
  printf("%ld %d %d %ld %d %d %ld %ld %ld\n", (long)(hit - buf), miss == 0,
         zero == 0, (long)(nul_after - word), nul_equal == 0, nul_before == 0,
         (long)(partial - buf), (long)(offset - offset_base),
         (long)(const_hit - cbuf));
  return 0;
}
// REWRITES-DAG: fn __slate_memchr(
// REWRITES-DAG: std::slice::from_raw_parts(bytes, n)
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: let hit = buf.as_slice().iter().position(
// REWRITES-DAG: (*__slate_byte as u8) == ((needle as i32) as u8)
// REWRITES-DAG: let miss = buf.as_slice().iter().position(
// REWRITES-DAG: let const_hit = cbuf.as_slice().iter().position(
// REWRITES-DAG: let nul_after = Some(3);
// REWRITES-DAG: let zero: *mut u8
// REWRITES-DAG: let nul_equal: *mut i8
// REWRITES-DAG: let nul_before: *mut i8
// REWRITES-DAG: let partial: *mut u8
// REWRITES-DAG: let offset: *mut u8
// REWRITES-DAG: partial.offset_from(
// REWRITES-DAG: offset.offset_from(
// REWRITES-NOT: let mut hit
// REWRITES-NOT: let mut miss
// REWRITES-NOT: let mut nul_after
// REWRITES-NOT: map_or(std::ptr::null_mut()
// REWRITES: {{^}}}
