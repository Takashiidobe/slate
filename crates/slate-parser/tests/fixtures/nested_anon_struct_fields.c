#include <stdio.h>

struct event {
  int type;
  union {
    struct {
      char *value;
    } alias;
    struct {
      char *handle;
      char *suffix;
    } tag;
  } data;
};

int main(void) {
  struct event e;
  e.type            = 1;
  char h[]          = "H";
  char s[]          = "S";
  e.data.tag.handle = h;
  e.data.tag.suffix = s;
  printf("%d %s%s\n", e.type, e.data.tag.handle, e.data.tag.suffix);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
