#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct GNUBuiltinOffset {
  unsigned char tag;
  int           values[3];
};

static int volatile gnu_builtin_never;

static int gnu_builtin_nonlocal(void) {
  void *buffer[5];
  if (__builtin_setjmp(buffer) == 0) {
    __builtin_longjmp(buffer, 1);
  }
  return 17;
}

static int gnu_builtin_addresses(void) {
  void *return_address = __builtin_return_address(0);
  void *extracted      = __builtin_extract_return_addr(return_address);
  void *restored       = __builtin_frob_return_addr(extracted);
  void *frame_address  = __builtin_frame_address(0);
  return (return_address != NULL) + (extracted != NULL) + (restored != NULL) +
         (frame_address != NULL);
}

static int gnu_builtin_compile_time(void) {
  int volatile runtime = 3;
  int selected         = __builtin_choose_expr(
      __builtin_types_compatible_p(int, signed int), 19, 23);
  int type_total = __builtin_types_compatible_p(int, int) +
                   __builtin_types_compatible_p(int, long);
  return __builtin_constant_p(5) + __builtin_constant_p(runtime) + selected +
         type_total;
}

static int gnu_builtin_control(void) {
  _Alignas(32) unsigned char storage[64] = {};
  unsigned char              code[8]     = {};
  void                      *assumed = __builtin_assume_aligned(storage, 32);
  int                        likely  = __builtin_expect(29, 29);
  int probable = __builtin_expect_with_probability(31, 31, 0.9);
  __builtin_assume(storage[0] == 0);
  __builtin_prefetch(storage + 16, 0, 3);
  __builtin___clear_cache((char *)code, (char *)code + sizeof(code));
  if (gnu_builtin_never) {
    __builtin_debugtrap();
  }
  if (gnu_builtin_never) {
    __builtin_trap();
  }
  if (gnu_builtin_never) {
    __builtin_unreachable();
  }
  return likely + probable + (assumed == storage);
}

static int gnu_builtin_sizes(void) {
  unsigned char object[24] = {};
  int           value      = 37;
  int          *address    = __builtin_addressof(value);
  uintptr_t     up         = __builtin_align_up((uintptr_t)65, 16);
  uintptr_t     down       = __builtin_align_down((uintptr_t)79, 16);
  return (int)__builtin_object_size(object, 0) +
         (int)__builtin_dynamic_object_size(object, 0) +
         (int)__builtin_offsetof(struct GNUBuiltinOffset, values[2]) +
         (address == &value) + (int)up + (int)down +
         __builtin_is_aligned(up, 16) + !__builtin_is_aligned(65, 16);
}

static int gnu_builtin_source(void) {
  return (__builtin_FUNCTION()[0] == 'g') + (__builtin_FILE()[0] != '\0') +
         (__builtin_LINE() > 0) + (__builtin_COLUMN() > 0);
}

int main(void) {
  printf("%d %d %d %d %d %d\n", gnu_builtin_compile_time(),
         gnu_builtin_control(), gnu_builtin_sizes(), gnu_builtin_source(),
         gnu_builtin_nonlocal(), gnu_builtin_addresses());
  return 0;
}
