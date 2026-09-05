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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C, packed)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct GNUPragmaPacked {
// COMMON-LOWERING-NEXT:     tag: u8,
// COMMON-LOWERING-NEXT:     value: u32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: core::arch::global_asm!(
// COMMON-LOWERING-NEXT:     ".weak gnu_pragma_weak_alias\n.set gnu_pragma_weak_alias, gnu_pragma_weak_target",
// COMMON-LOWERING-NEXT: );
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut gnu_pragma_inner_macro: i32 = 11;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut gnu_pragma_outer_macro: i32 = 7;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn gnu_pragma_weak_alias() -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_pragma_hidden({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 13;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[unsafe(no_mangle)]
// COMMON-LOWERING-NEXT: pub extern "C-unwind" fn gnu_pragma_weak_target() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 17;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_pragma_actual() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 19;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut packed: GNUPragmaPacked = GNUPragmaPacked { tag: 0, value: 0 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: GNUPragmaPacked = GNUPragmaPacked { tag: 29, value: 31 };
// COMMON-LOWERING-NEXT:     packed = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { gnu_pragma_inner_macro };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { gnu_pragma_outer_macro };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::offset_of!(GNUPragmaPacked, value) as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 37;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_pragma_hidden({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { gnu_pragma_weak_alias() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_pragma_actual();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_pragma_diagnostic();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = packed.tag;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = packed.value;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_pragma_diagnostic() -> i32 {
// COMMON-LOWERING-NEXT:     let mut ignored: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 23;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: #[cfg(target_arch = "x86_64")]
// LOWERING-X86_64-GNU-NEXT:     options(att_syntax, raw)
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: #[cfg(target_arch = "aarch64")]
// LOWERING-AARCH64-GNU-NEXT:     options(raw)
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C, packed)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct GNUPragmaPacked {
// COMMON-REWRITES-NEXT:     tag: u8,
// COMMON-REWRITES-NEXT:     value: u32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: core::arch::global_asm!(
// COMMON-REWRITES-NEXT:     ".weak gnu_pragma_weak_alias\n.set gnu_pragma_weak_alias, gnu_pragma_weak_target",
// COMMON-REWRITES-NEXT: );
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut gnu_pragma_inner_macro: i32 = 11;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut gnu_pragma_outer_macro: i32 = 7;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn gnu_pragma_weak_alias() -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_pragma_hidden({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + 13
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[unsafe(no_mangle)]
// COMMON-REWRITES-NEXT: pub extern "C-unwind" fn gnu_pragma_weak_target() -> i32 {
// COMMON-REWRITES-NEXT:     17
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_pragma_actual() -> i32 {
// COMMON-REWRITES-NEXT:     19
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut packed: GNUPragmaPacked = GNUPragmaPacked { tag: 29, value: 31 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { gnu_pragma_inner_macro };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { gnu_pragma_outer_macro };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = std::mem::offset_of!(GNUPragmaPacked, value) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = gnu_pragma_hidden(37);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { gnu_pragma_weak_alias() };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = gnu_pragma_actual();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = gnu_pragma_diagnostic();
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             5 as i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             5 as i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} + {{__v[0-9]+}} + (packed.tag as i32) + (packed.value as i32),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_pragma_diagnostic() -> i32 {
// COMMON-REWRITES-NEXT:     23
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: #[cfg(target_arch = "x86_64")]
// REWRITES-X86_64-GNU-NEXT:     options(att_syntax, raw)
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d %d %d %d %d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: #[cfg(target_arch = "aarch64")]
// REWRITES-AARCH64-GNU-NEXT:     options(raw)
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d %d %d %d %d %d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
