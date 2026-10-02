#include <stddef.h>
#include <stdlib.h>

struct Entry { long value; int tag; };
struct List { int count; struct Entry entries[]; };
struct Bytes { double head; int tag; unsigned char bytes[]; };
struct Wide { char tag; long double values[]; };

int main(void) {
  struct List header = {2};
  struct List copied = header;
  if (copied.count != 2) return 7;
  struct List *list = malloc(sizeof(*list) + 5 * sizeof(struct Entry));
  struct Bytes *bytes = malloc(sizeof(*bytes) + 9);
  struct Wide *wide = malloc(sizeof(*wide) + 3 * sizeof(long double));
  if (!list || !bytes || !wide) return 1;
  if ((char *)list->entries - (char *)list != offsetof(struct List, entries)) return 2;
  if ((char *)bytes->bytes - (char *)bytes != offsetof(struct Bytes, bytes)) return 3;
  if ((char *)wide->values - (char *)wide != offsetof(struct Wide, values)) return 4;
  if ((char *)(bytes + 1) - (char *)bytes != sizeof(*bytes)) return 5;
  if ((char *)(wide + 1) - (char *)wide != sizeof(*wide)) return 6;
  list->count = 5;
  bytes->head = 2.5;
  bytes->tag = 17;
  wide->tag = 4;
  for (int i = 0; i < list->count; ++i) {
    list->entries[i].value = i * 13;
    list->entries[i].tag = i + 7;
  }
  unsigned char (*array)[] = &bytes->bytes;
  for (int i = 0; i < 9; ++i) (*array)[i] = i * 3;
  for (int i = 0; i < 3; ++i) wide->values[i] = i + 0.5L;
  const struct List *view = list;
  int result = view->entries[4].value != 52 || view->entries[3].tag != 10
    || bytes->bytes[8] != 24 || bytes->head != 2.5 || bytes->tag != 17
    || wide->values[2] != 2.5L || wide->tag != 4;
  free(wide);
  free(bytes);
  free(list);
  return result;
}
