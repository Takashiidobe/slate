#include <stdio.h>

void *memcpy(void *dest, const void *src, unsigned long n);
void *memset(void *s, int c, unsigned long n);
int memcmp(const void *s1, const void *s2, int len);
int bcmp(const void *s1, const void *s2, unsigned long len);
unsigned long strlen(const char *s);
extern void *strlen_ref;
extern void *memcmp_ref;

int main(void) {
  char buffer[16];
  memset(buffer, 'x', sizeof buffer);
  memcpy(buffer, "slate", 6);
  printf("%s %lu %d %d %d %c\n", buffer, strlen(buffer), memcmp("abc", "abd", 3),
         memcmp("abc", "abc", 3), bcmp("ab", "ac", 2), buffer[10]);
  printf("%d %d\n", strlen_ref == (void *)strlen, memcmp_ref == (void *)memcmp);
  return 0;
}
