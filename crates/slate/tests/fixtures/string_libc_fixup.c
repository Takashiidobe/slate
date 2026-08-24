#include <stdio.h>
#include <string.h>

int main(void) {
  char          alpha[]     = "abc";
  char          beta[]      = "abd";
  unsigned char bytes_a[]   = "\xff\x01";
  unsigned char bytes_b[]   = "\xff\x02";
  char          hay[]       = "abacad";
  char          sub[]       = "aca";
  char          empty[]     = "";
  char          set[]       = "cx";
  char          prefix[]    = "ab";
  char          reject[]    = "cd";
  char          utf8[]      = "hé";
  int           second_byte = 0xa9;

  printf("%zu %d %d %d %d\n", strlen(alpha), strcmp(alpha, alpha) == 0,
         strcmp(alpha, beta) < 0, strncmp(alpha, beta, 2) == 0,
         memcmp(bytes_a, bytes_b, 1) == 0);
  char *first = strchr(hay, 'a');
  char *last  = strrchr(hay, 'a');
  printf("%c %c %d %d %d %d %zu %zu\n", *first, *last, last == first + 4,
         strstr(hay, sub) != 0, strstr(hay, empty) != 0, strpbrk(hay, set) == 0,
         strspn(hay, prefix), strcspn(hay, reject));
  printf("%d %d\n", strchr(hay, 0) != 0, strchr(hay, 'z') == 0);
  printf("%d\n", strchr(utf8, second_byte) != 0);
  return 0;
}
// REWRITES-DAG: let alpha: &str = "abc";
// REWRITES-DAG: let bytes_a: &[u8] = b"\xff\x01";
// REWRITES-DAG: : usize = alpha.len();
// REWRITES-DAG: alpha == alpha
// REWRITES-DAG: alpha.cmp(beta) == std::cmp::Ordering::Less
// REWRITES-DAG: .split_at(std::cmp::min(
// REWRITES-DAG: hay.find(char::from(
// REWRITES-DAG: hay.rfind(char::from(
// REWRITES-DAG: hay.find(sub)
// REWRITES-DAG: hay.find(|__slate_ch| set.contains(__slate_ch))
// REWRITES-DAG: hay.find(|__slate_ch| !prefix.contains(__slate_ch)).unwrap_or(hay.len())
// REWRITES-DAG: hay.find(|__slate_ch| reject.contains(__slate_ch)).unwrap_or(hay.len())
// REWRITES-DAG: let utf8: &str = "h\u{e9}";
// REWRITES-DAG: utf8.as_bytes().iter().position(
// REWRITES-NOT: alpha.len() as u64
// REWRITES-NOT: fn strlen(
// REWRITES-NOT: fn strcmp(
// REWRITES-NOT: fn strncmp(
// REWRITES-NOT: fn memcmp(
// REWRITES-NOT: fn strchr(
// REWRITES-NOT: fn strrchr(
// REWRITES-NOT: fn strstr(
// REWRITES-NOT: fn strpbrk(
// REWRITES-NOT: fn strspn(
// REWRITES-NOT: fn strcspn(
