void *memcpy(void *dest, const void *src, unsigned long n) {
  volatile unsigned char *d = dest;
  const unsigned char *s = src;
  while (n--)
    *d++ = *s++;
  return dest;
}

void *memset(void *s, int c, unsigned long n) {
  volatile unsigned char *p = s;
  while (n--)
    *p++ = (unsigned char)c;
  return s;
}

int memcmp(const void *s1, const void *s2, int len) {
  const volatile unsigned char *a = s1;
  const unsigned char *b = s2;
  for (; len > 0; len--, a++, b++)
    if (*a != *b)
      return *a < *b ? -1 : 1;
  return 0;
}

int bcmp(const void *s1, const void *s2, unsigned long len) {
  return memcmp(s1, s2, (int)len) != 0;
}

unsigned long strlen(const char *s) {
  const volatile char *p = s;
  while (*p)
    p++;
  return (unsigned long)(p - s);
}
