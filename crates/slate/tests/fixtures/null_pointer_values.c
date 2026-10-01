#include <stddef.h>
#include <stdio.h>

struct Node {
  int value;
  struct Node *next;
};

struct Entry {
  const char *key;
  int *slot;
};

static struct Node *head = NULL;
static const char *last_key = 0;
void *opaque = (void *)0;
static int *table[3] = {NULL, 0, NULL};

static struct Node *find(struct Node *list, int value) {
  for (struct Node *node = list; node != NULL; node = node->next) {
    if (node->value == value)
      return node;
  }
  return NULL;
}

static int length(const struct Node *list) {
  int count = 0;
  while (list) {
    count++;
    list = list->next;
  }
  return count;
}

static const char *name_or_default(const char *name) { return name ? name : "default"; }

int main(void) {
  struct Node c = {3, NULL};
  struct Node b = {2, &c};
  struct Node a = {1, &b};
  int stored = 7;
  struct Entry entries[2] = {{"present", &stored}, {NULL, NULL}};
  int missing = find(&a, 9) == NULL;
  int found = find(&a, 2) != 0;
  head = &a;
  int *cursor = NULL;
  if (!cursor)
    cursor = entries[0].slot;
  printf("%d %d %d %d\n", length(head), missing, found, *cursor);
  printf("%s %s\n", name_or_default(entries[0].key), name_or_default(entries[1].key));
  printf("%d %d %d\n", last_key == NULL, opaque == NULL, table[1] == NULL);
  head = NULL;
  last_key = entries[0].key;
  printf("%d %s\n", length(head), last_key);
  return entries[1].slot == NULL ? 0 : 1;
}
