#include <stdio.h>

struct First {
  int value;
};

struct Second {
  int value;
};

typedef union {
  struct First  *first;
  struct Second *second;
} PointerArgument __attribute__((transparent_union));

__attribute__((noinline)) static int read_value(PointerArgument argument) {
  return argument.first->value;
}

int main(void) {
  struct First  first  = {.value = 17};
  struct Second second = {.value = 29};
  printf("%d %d\n", read_value(&first), read_value(&second));
  return 0;
}




// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: ⚠ 'transparent_union' attribute ignored; it applies only to unions
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/transparent_union_call.c:14:34]
// DEFAULT: 13 │   struct Second *second;
// DEFAULT: 14 │ } PointerArgument __attribute__((transparent_union));
// DEFAULT: ·                                  ─────────────────
// DEFAULT: 15 │
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × conversion between a struct or union and an unrelated type
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/transparent_union_call.c:23:21]
// DEFAULT: 22 │   struct Second second = {.value = 29};
// DEFAULT: 23 │   printf("%d %d\n", read_value(&first), read_value(&second));
// DEFAULT: ·                     ──────────────────
// DEFAULT: 24 │   return 0;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
