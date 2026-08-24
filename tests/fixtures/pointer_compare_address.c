// LOWERING-LABEL: {{^}}fn main() {
// LOWERING-DAG: _v{{[0-9]+}} == std::ptr::addr_of_mut!(utc)
// LOWERING-DAG: _v{{[0-9]+}} == std::ptr::addr_of_mut!(local)
// LOWERING-NOT: _v{{[0-9]+}} == utc
// LOWERING-NOT: _v{{[0-9]+}} == local
// LOWERING: {{^}}}

#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>

int main(void) {
  time_t    timestamp    = 0;
  struct tm utc          = {};
  struct tm local        = {};
  int       utc_result   = gmtime_r(&timestamp, &utc) == &utc;
  int       local_result = localtime_r(&timestamp, &local) == &local;
  printf("%d %d\n", utc_result, local_result);
  return 0;
}
