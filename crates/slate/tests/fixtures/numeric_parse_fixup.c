#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char whole[] = "42";
  char whole_long[] = "-12345";
  char whole_unsigned[] = "77";
  char leading[] = "  -17tail";
  char empty[] = "";
  char large[] = "999999999999999999999999999999";
  char flt[] = "  -3.5e2rest";
  char end_source[] = "12tail";
  char *end = 0;

  printf("%d %ld %lu %ld %ld %lu %.1f\n", atoi(whole),
         strtol(whole_long, 0, 10), strtoul(whole_unsigned, 0, 10),
         atol(leading), strtol(large, 0, 10), strtoul(empty, 0, 10),
         strtod(flt, 0));

  long raw = strtol(end_source, &end, 10);
  printf("%ld %c\n", raw, *end);
  return 0;
}
