#include <string.h>

struct pw_entry {
  char *pw_name;
};

char *entry_name(struct pw_entry *entry) {
  char *local = entry->pw_name;
  return local;
}

int main(void) {
  struct pw_entry entry;
  entry.pw_name = "root";
  return strcmp(entry_name(&entry), "root") == 0 ? 0 : 1;
}

// LOWERING: #![allow(

// REWRITES: #![allow(
