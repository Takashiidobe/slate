#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct __attribute__((packed)) GNUAttributePacked {
  unsigned char tag;
  uint32_t      value;
};

struct __attribute__((aligned(32))) GNUAttributeAligned {
  unsigned char value;
};

typedef int          GNUAttributeAliasInt __attribute__((may_alias));
typedef unsigned int GNUAttributeByte __attribute__((mode(QI)));
typedef unsigned int GNUAttributeVector __attribute__((vector_size(16)));

typedef union {
  int          integer;
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
    int value __attribute__((cleanup(gnu_attribute_cleanup)))  = 53;
    gnu_cleanup_total                                         += 1;
  }
  return gnu_cleanup_total;
}

static int gnu_attribute_layout(void) {
  struct GNUAttributePacked  packed       = {1, 59};
  struct GNUAttributeAligned aligned      = {61};
  GNUAttributeAliasInt       alias_value  = 67;
  GNUAttributeByte           byte_value   = 255;
  GNUAttributeVector         vector_value = {2, 3, 5, 7};
  return (int)sizeof(packed) + (int)offsetof(struct GNUAttributePacked, value) +
         (int)_Alignof(struct GNUAttributeAligned) + packed.tag +
         (int)packed.value + aligned.value + alias_value +
         (int)sizeof(byte_value) + (int)byte_value + (int)sizeof(vector_value) +
         (int)vector_value[0] + (int)vector_value[3];
}

int main(void) {
  int value          = 71;
  int volatile never = 0;
  gnu_common_value   = 73;
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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(used_with_arg)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_cleanup {
// CHECK-NEXT:     pub struct Guard<F: FnMut()>(pub F);
// CHECK-NEXT:     impl<F: FnMut()> Drop for Guard<F> {
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             (self.0)()
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn gnu_attribute_allocate([[ARG_0:arg[0-9]+]]: u64, [[ARG_1:arg[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct GNUAttributePacked {
// CHECK-NEXT:     tag: u8,
// CHECK-NEXT:     value: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct GNUAttributeAligned {
// CHECK-NEXT:     value: u8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateRecord_gnu_attributes_c_22 {
// CHECK-NEXT:     integer: i32,
// CHECK-NEXT:     unsigned_integer: u32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut gnu_cleanup_total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut gnu_constructor_total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut gnu_common_value: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut gnu_nocommon_value: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: #[used(linker)]
// CHECK-NEXT: #[unsafe(link_section = ".data.gnu_attribute")]
// CHECK-NEXT: static mut gnu_section_value: i32 = 3;
// CHECK-NEXT: use slate_support::exit;
// CHECK-NEXT: fn gnu_attribute_cleanup(value: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         gnu_cleanup_total = gnu_cleanup_total + *value;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_constructor() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         gnu_constructor_total = 5;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn __slate_init_gnu_attribute_constructor() {
// CHECK-NEXT:     gnu_attribute_constructor();
// CHECK-NEXT: }
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used]
// CHECK-NEXT: #[unsafe(link_section = ".init_array")]
// CHECK-NEXT: static __SLATE_INIT_gnu_attribute_constructor: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_init_gnu_attribute_constructor,
// CHECK-NEXT: );
// CHECK-NEXT: fn gnu_attribute_destructor() {
// CHECK-NEXT:     println!("destructor {}", unsafe { gnu_cleanup_total });
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn __slate_fini_gnu_attribute_destructor() {
// CHECK-NEXT:     gnu_attribute_destructor();
// CHECK-NEXT: }
// CHECK-NEXT: #[allow(non_upper_case_globals)]
// CHECK-NEXT: #[used]
// CHECK-NEXT: #[unsafe(link_section = ".fini_array")]
// CHECK-NEXT: static __SLATE_FINI_gnu_attribute_destructor: Option<unsafe extern "C-unwind" fn()> = Some(
// CHECK-NEXT:     __slate_fini_gnu_attribute_destructor,
// CHECK-NEXT: );
// CHECK-NEXT: fn gnu_attribute_target(value: i32) -> i32 {
// CHECK-NEXT:     value + 7
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_weak() -> i32 {
// CHECK-NEXT:     11
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.gnu_attribute")]
// CHECK-NEXT: fn gnu_attribute_decorated(value: i32) -> i32 {
// CHECK-NEXT:     value + 13
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_inline(value: i32) -> i32 {
// CHECK-NEXT:     value + 17
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_flatten(value: i32) -> i32 {
// CHECK-NEXT:     gnu_attribute_inline(value)
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_const(value: i32) -> i32 {
// CHECK-NEXT:     value * 2
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_pure(value: &i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(value as *const i32) }) + 19
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_nonnull(value: &i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(value as *const i32) }) + 23
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_result(value: i32) -> i32 {
// CHECK-NEXT:     value + 29
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_deprecated(value: i32) -> i32 {
// CHECK-NEXT:     value + 31
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn gnu_attribute_format(
// CHECK-NEXT:     format: *const i8,
// CHECK-NEXT:     __va_args: ...
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     (((unsafe { *format.add(0) }) as i32) == 102) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_format_arg(format: *const i8) -> *const i8 {
// CHECK-NEXT:     format
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn gnu_attribute_sentinel(value: i32, __va_args: ...) -> i32 {
// CHECK-NEXT:     value + 37
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_noreturn() {
// CHECK-NEXT:     unsafe { std::process::exit(99i32 as i32) };
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_returns_twice() -> i32 {
// CHECK-NEXT:     41
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_leaf(value: i32) -> i32 {
// CHECK-NEXT:     value + 43
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_transparent(value: __SlateRecord_gnu_attributes_c_22) -> i32 {
// CHECK-NEXT:     (unsafe { value.integer }) + 47
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_cleanup_scope() -> i32 {
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut value: i32 = 53;
// CHECK-NEXT:         let __slate_cleanup_ptr_0 = std::ptr::addr_of_mut!(value) as *mut _;
// CHECK-NEXT:         let __slate_cleanup_0 = __slate_cleanup::Guard(|| gnu_attribute_cleanup(
// CHECK-NEXT:             __slate_cleanup_ptr_0 as *mut _,
// CHECK-NEXT:         ));
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             gnu_cleanup_total = gnu_cleanup_total + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { gnu_cleanup_total }
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_attribute_layout() -> i32 {
// CHECK-NEXT:     let packed: GNUAttributePacked = GNUAttributePacked {
// CHECK-NEXT:         tag: 1,
// CHECK-NEXT:         value: 59,
// CHECK-NEXT:     };
// CHECK-NEXT:     let aligned: GNUAttributeAligned = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: GNUAttributeAligned = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].value = 61u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let alias_value: i32 = 67;
// CHECK-NEXT:     let byte_value: u8 = 255;
// CHECK-NEXT:     let vector_value: std::simd::Simd<u32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         2u32, 3u32, 5u32, 7u32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     (std::mem::size_of::<GNUAttributePacked>() as i32)
// CHECK-NEXT:         + (std::mem::offset_of!(GNUAttributePacked, value) as i32)
// CHECK-NEXT:         + (std::mem::align_of::<GNUAttributeAligned>() as i32) + (packed.tag as i32)
// CHECK-NEXT:         + (packed.value as i32) + (aligned.value as i32) + alias_value
// CHECK-NEXT:         + (std::mem::size_of::<u8>() as i32) + (byte_value as i32) + 16
// CHECK-NEXT:         + (vector_value[0] as i32) + (vector_value[3] as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 71;
// CHECK-NEXT:     let never: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         gnu_common_value = 73;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         gnu_nocommon_value = 79;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(never)) }) != 0 {
// CHECK-NEXT:         gnu_attribute_noreturn();
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {} {} {}", gnu_attribute_layout(),
// CHECK-NEXT:         gnu_attribute_cleanup_scope(), unsafe { gnu_constructor_total },
// CHECK-NEXT:         gnu_attribute_target(83i32), gnu_attribute_target(89i32), gnu_attribute_weak(),
// CHECK-NEXT:         gnu_attribute_decorated(97i32) + gnu_attribute_flatten(101i32),
// CHECK-NEXT:         gnu_attribute_const(103i32) + gnu_attribute_pure(unsafe { & (*
// CHECK-NEXT:         (std::ptr::addr_of_mut!(value) as * const i32)) }) + gnu_attribute_nonnull(unsafe
// CHECK-NEXT:         { & (* (std::ptr::addr_of_mut!(value) as * const i32)) }),
// CHECK-NEXT:         gnu_attribute_result(107i32) + gnu_attribute_deprecated(109i32), (unsafe {
// CHECK-NEXT:         gnu_attribute_sentinel(113i32, std::ptr::null_mut::< () > ()) }) +
// CHECK-NEXT:         gnu_attribute_returns_twice() + gnu_attribute_leaf(127i32) +
// CHECK-NEXT:         gnu_attribute_transparent(__SlateRecord_gnu_attributes_c_22 { integer : 131 }) +
// CHECK-NEXT:         unsafe { gnu_common_value } + unsafe { gnu_nocommon_value } + unsafe {
// CHECK-NEXT:         gnu_section_value }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "format {} {}", 137i32, unsafe {
// CHECK-NEXT:         gnu_attribute_format(gnu_attribute_format_arg(c"format %d".as_ptr()), 137i32) }
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
