#include <stdint.h>
#include <stdio.h>

static int gnu_builtin_stack(void) {
  unsigned char *first   = __builtin_alloca(16);
  unsigned char *aligned = __builtin_alloca_with_align(16, 128);
  __builtin_memset(first, 3, 16);
  __builtin_memset(aligned, 5, 16);
  return first[0] + first[15] + aligned[0] + aligned[15] +
         ((uintptr_t)aligned % 16 == 0);
}

static int gnu_builtin_memory(void) {
  unsigned char source[16] = "abcdefghijklmno";
  unsigned char first[32]  = {};
  unsigned char second[32] = {};
  unsigned char third[32]  = {};
  void         *end        = __builtin_mempcpy(first, source, 8);
  __builtin_memcpy_inline(second, source, 8);
  __builtin_memcpy(third, source, 16);
  __builtin_memmove(third + 2, third, 8);
  __builtin_bzero(second + 8, 8);
  __builtin___memcpy_chk(first + 8, source + 8, 8, sizeof(first) - 8);
  __builtin___memmove_chk(second + 1, second, 7, sizeof(second) - 1);
  __builtin___memset_chk(second + 16, 9, 4, sizeof(second) - 16);
  return (end == first + 8) + (__builtin_memcmp(first, source, 16) == 0) +
         (__builtin_bcmp(first, source, 16) == 0) +
         (__builtin_memchr(first, 'h', 16) == first + 7) + (third[2] == 'a') +
         (third[9] == 'h') + second[0] + second[1] + second[16] + second[19];
}

static int gnu_builtin_strings(void) {
  char  first[64]  = {};
  char  second[64] = {};
  char  third[64]  = {};
  char *end;
  __builtin_strcpy(first, "gnu");
  end = __builtin_stpcpy(first + 3, "builtins");
  __builtin_strncpy(second, first, 6);
  __builtin_strcpy(third, "memory");
  __builtin_strcat(third, "-ok");
  __builtin___strcpy_chk(second, "clang", sizeof(second));
  __builtin___stpcpy_chk(second + 5, "-head", sizeof(second) - 5);
  __builtin___strcat_chk(second, "-gnu", sizeof(second));
  return (int)__builtin_strlen(first) + (end == first + 11) +
         (__builtin_strcmp(third, "memory-ok") == 0) +
         (__builtin_strncmp(second, "clang-head-gnu", 14) == 0) +
         (__builtin_strchr(second, '-') == second + 5) +
         (__builtin_strrchr(second, '-') == second + 10) +
         (__builtin_strstr(second, "head") == second + 6) +
         (int)__builtin_strspn(second, "clang") +
         (int)__builtin_strcspn(second, "-") +
         (__builtin_strpbrk(second, "-g") == second + 4);
}

int main(void) {
  printf("%d %d %d\n", gnu_builtin_stack(), gnu_builtin_memory(),
         gnu_builtin_strings());
  return 0;
}
