#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct __attribute__((packed)) GNUAttributePacked {
  unsigned char tag;
  uint32_t value;
};

struct __attribute__((aligned(32))) GNUAttributeAligned {
  unsigned char value;
};

typedef int GNUAttributeAliasInt __attribute__((may_alias));
typedef unsigned int GNUAttributeByte __attribute__((mode(QI)));
typedef unsigned int GNUAttributeVector __attribute__((vector_size(16)));

typedef union {
  int integer;
  unsigned int unsigned_integer;
} GNUAttributeTransparent __attribute__((transparent_union));

extern void *gnu_attribute_allocate(size_t, size_t)
    __attribute__((malloc, alloc_size(1), alloc_align(2), returns_nonnull));

static int gnu_cleanup_total;
static int gnu_constructor_total;
static int gnu_common_value __attribute__((common));
static int gnu_nocommon_value __attribute__((nocommon));
static int gnu_section_value
    __attribute__((used, retain, section(".data.gnu_attribute"))) = 3;

static void gnu_attribute_cleanup(int *value) { gnu_cleanup_total += *value; }

static void gnu_attribute_constructor(void) __attribute__((constructor));

static void gnu_attribute_constructor(void) { gnu_constructor_total = 5; }

static void gnu_attribute_destructor(void) __attribute__((destructor));

static void gnu_attribute_destructor(void) {
  printf("destructor %d\n", gnu_cleanup_total);
}

int gnu_attribute_target(int value) { return value + 7; }

extern int gnu_attribute_alias(int)
    __attribute__((alias("gnu_attribute_target")));

static int gnu_attribute_weakref(int)
    __attribute__((weakref("gnu_attribute_target")));

int gnu_attribute_weak(void) __attribute__((weak));

int gnu_attribute_weak(void) { return 11; }

__attribute__((noinline, cold, used, visibility("hidden"),
               section(".text.gnu_attribute"), no_sanitize("undefined"),
               no_stack_protector)) int
gnu_attribute_decorated(int value) {
  return value + 13;
}

static inline __attribute__((always_inline, artificial)) int
gnu_attribute_inline(int value) {
  return value + 17;
}

__attribute__((flatten, hot)) static int gnu_attribute_flatten(int value) {
  return gnu_attribute_inline(value);
}

__attribute__((const)) static int gnu_attribute_const(int value) {
  return value * 2;
}

__attribute__((pure)) static int gnu_attribute_pure(const int *value) {
  return *value + 19;
}

__attribute__((nonnull(1))) static int gnu_attribute_nonnull(const int *value) {
  return *value + 23;
}

__attribute__((warn_unused_result)) static int gnu_attribute_result(int value) {
  return value + 29;
}

__attribute__((deprecated("GNU deprecated attribute"))) static int
gnu_attribute_deprecated(int value) {
  return value + 31;
}

__attribute__((format(printf, 1, 2))) static int
gnu_attribute_format(const char *format, ...) {
  return format[0] == 'f';
}

__attribute__((format_arg(1))) static const char *
gnu_attribute_format_arg(const char *format) {
  return format;
}

__attribute__((sentinel)) static int gnu_attribute_sentinel(int value, ...) {
  return value + 37;
}

__attribute__((noreturn)) static void gnu_attribute_noreturn(void) { exit(99); }

__attribute__((returns_twice)) static int gnu_attribute_returns_twice(void) {
  return 41;
}

__attribute__((leaf)) static int gnu_attribute_leaf(int value) {
  return value + 43;
}

static int gnu_attribute_transparent(GNUAttributeTransparent value) {
  return value.integer + 47;
}

static int gnu_attribute_cleanup_scope(void) {
  {
    int value __attribute__((cleanup(gnu_attribute_cleanup))) = 53;
    gnu_cleanup_total += 1;
  }
  return gnu_cleanup_total;
}

static int gnu_attribute_layout(void) {
  struct GNUAttributePacked packed = {1, 59};
  struct GNUAttributeAligned aligned = {61};
  GNUAttributeAliasInt alias_value = 67;
  GNUAttributeByte byte_value = 255;
  GNUAttributeVector vector_value = {2, 3, 5, 7};
  return (int)sizeof(packed) + (int)offsetof(struct GNUAttributePacked, value) +
         (int)_Alignof(struct GNUAttributeAligned) + packed.tag +
         (int)packed.value + aligned.value + alias_value +
         (int)sizeof(byte_value) + (int)byte_value + (int)sizeof(vector_value) +
         (int)vector_value[0] + (int)vector_value[3];
}

int main(void) {
  int value = 71;
  int volatile never = 0;
  gnu_common_value = 73;
  gnu_nocommon_value = 79;
  if (never) {
    gnu_attribute_noreturn();
  }
  printf("%d %d %d %d %d %d %d %d %d %d\n", gnu_attribute_layout(),
         gnu_attribute_cleanup_scope(), gnu_constructor_total,
         gnu_attribute_alias(83), gnu_attribute_weakref(89),
         gnu_attribute_weak(),
         gnu_attribute_decorated(97) + gnu_attribute_flatten(101),
         gnu_attribute_const(103) + gnu_attribute_pure(&value) +
             gnu_attribute_nonnull(&value),
         gnu_attribute_result(107) + gnu_attribute_deprecated(109),
         gnu_attribute_sentinel(113, NULL) + gnu_attribute_returns_twice() +
             gnu_attribute_leaf(127) + gnu_attribute_transparent(131) +
             gnu_common_value + gnu_nocommon_value + gnu_section_value);
  printf("format %d %d\n", 137,
         gnu_attribute_format(gnu_attribute_format_arg("format %d"), 137));
  return 0;
}
