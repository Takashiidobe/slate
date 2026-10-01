#include <stddef.h>
#include <stdio.h>

static unsigned char bytes[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

static unsigned char at(const void *base, long index) {
  return *(const unsigned char *)(base + index);
}

int main(void) {
  void       *start  = bytes;
  void       *end    = bytes + sizeof bytes;
  const void *middle = start + 5;
  void       *cursor = end;
  cursor             = cursor - 3;
  cursor -= 2;
  cursor += 1;
  cursor++;
  --cursor;

  ptrdiff_t span     = end - start;
  ptrdiff_t back     = start - end;
  ptrdiff_t offset   = middle - (const void *)start;
  ptrdiff_t position = cursor - start;

  printf("%td %td %td %td\n", span, back, offset, position);
  printf("%u %u %u\n", at(start, 7), at(middle, -2), *(unsigned char *)cursor);
  return (int)(cursor - middle);
}
