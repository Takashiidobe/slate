#include <stddef.h>
#include <stdio.h>

#include "gnu_pragmas_once.h"

#pragma message("GNU pragma message probe")
#pragma GCC warning "GNU pragma warning probe"

#define GNU_PRAGMA_MACRO 7
#pragma push_macro("GNU_PRAGMA_MACRO")
#undef GNU_PRAGMA_MACRO
#define GNU_PRAGMA_MACRO 11
static int gnu_pragma_inner_macro = GNU_PRAGMA_MACRO;
#pragma pop_macro("GNU_PRAGMA_MACRO")
static int gnu_pragma_outer_macro = GNU_PRAGMA_MACRO;

#pragma pack(push, 1)
struct GNUPragmaPacked {
  unsigned char tag;
  unsigned int  value;
};
#pragma pack(pop)

#pragma GCC visibility push(hidden)
int                    gnu_pragma_hidden(int value) { return value + 13; }
#pragma GCC visibility pop

int gnu_pragma_weak_target(void) { return 17; }

#pragma weak gnu_pragma_weak_alias = gnu_pragma_weak_target
extern int   gnu_pragma_weak_alias(void);

#pragma redefine_extname gnu_pragma_renamed gnu_pragma_actual
int gnu_pragma_renamed(void) { return 19; }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static int gnu_pragma_diagnostic(void) {
  int ignored;
  return 23;
}
#pragma GCC diagnostic pop

#pragma GCC poison gnu_pragma_poisoned_identifier

int main(void) {
  struct GNUPragmaPacked packed = {29, 31};
  printf("%d %d %d %d %d %d %d %d\n", GNU_PRAGMA_ONCE_VALUE,
         gnu_pragma_inner_macro, gnu_pragma_outer_macro, (int)sizeof(packed),
         (int)offsetof(struct GNUPragmaPacked, value), gnu_pragma_hidden(37),
         gnu_pragma_weak_alias(),
         gnu_pragma_renamed() + gnu_pragma_diagnostic() + packed.tag +
             (int)packed.value);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: static mut gnu_pragma_inner_macro: i32 = 11;
// CHECK-NEXT: static mut gnu_pragma_outer_macro: i32 = 7;
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct GNUPragmaPacked {
// CHECK-NEXT:     tag: u8,
// CHECK-NEXT:     value: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_pragma_hidden(value: i32) -> i32 {
// CHECK-NEXT:     value + 13
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_pragma_weak_target() -> i32 {
// CHECK-NEXT:     17
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_pragma_renamed() -> i32 {
// CHECK-NEXT:     19
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_pragma_diagnostic() -> i32 {
// CHECK-NEXT:     23
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let packed: GNUPragmaPacked = GNUPragmaPacked {
// CHECK-NEXT:         tag: 29,
// CHECK-NEXT:         value: 31,
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {}", 5i32, unsafe { gnu_pragma_inner_macro }, unsafe {
// CHECK-NEXT:         gnu_pragma_outer_macro }, std::mem::size_of::< GNUPragmaPacked > () as i32,
// CHECK-NEXT:         std::mem::offset_of!(GNUPragmaPacked, value) as i32, gnu_pragma_hidden(37i32),
// CHECK-NEXT:         gnu_pragma_weak_target(), gnu_pragma_renamed() + gnu_pragma_diagnostic() +
// CHECK-NEXT:         (packed.tag as i32) + (packed.value as i32)
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
