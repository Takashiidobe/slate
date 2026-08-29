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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct GNUPragmaPacked {
// LOWERING-NEXT:     tag: u8,
// LOWERING-NEXT:     value: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[cfg(target_arch = "x86_64")]
// LOWERING-NEXT: core::arch::global_asm!(".weak gnu_pragma_weak_alias\n.set gnu_pragma_weak_alias, gnu_pragma_weak_target", options(att_syntax, raw));
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut gnu_pragma_inner_macro: i32 = 11;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut gnu_pragma_outer_macro: i32 = 7;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn gnu_pragma_weak_alias() -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_pragma_hidden(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     value = arg0;
// LOWERING-NEXT:     let _v0: i32 = value;
// LOWERING-NEXT:     let _v1: i32 = 13;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[unsafe(no_mangle)]
// LOWERING-NEXT: pub extern "C" fn gnu_pragma_weak_target() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 17;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_pragma_actual() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 19;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_pragma_diagnostic() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut ignored: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 23;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut packed: GNUPragmaPacked = GNUPragmaPacked { tag: 0, value: 0 };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     packed = GNUPragmaPacked { tag: 29, value: 31 };
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 5;
// LOWERING-NEXT:     let _v3: i32 = unsafe { gnu_pragma_inner_macro };
// LOWERING-NEXT:     let _v4: i32 = unsafe { gnu_pragma_outer_macro };
// LOWERING-NEXT:     let _v5: i32 = 5;
// LOWERING-NEXT:     let _v6: i32 = std::mem::offset_of!(GNUPragmaPacked, value) as i32;
// LOWERING-NEXT:     let _v7: i32 = 37;
// LOWERING-NEXT:     let _v8: i32 = gnu_pragma_hidden(_v7);
// LOWERING-NEXT:     let _v9: i32 = unsafe { gnu_pragma_weak_alias() };
// LOWERING-NEXT:     let _v10: i32 = gnu_pragma_actual();
// LOWERING-NEXT:     let _v11: i32 = gnu_pragma_diagnostic();
// LOWERING-NEXT:     let _v12: i32 = _v10 + _v11;
// LOWERING-NEXT:     let _v13: u8 = packed.tag;
// LOWERING-NEXT:     let _v14: i32 = _v13 as i32;
// LOWERING-NEXT:     let _v15: i32 = _v12 + _v14;
// LOWERING-NEXT:     let _v16: u32 = packed.value;
// LOWERING-NEXT:     let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:     let _v18: i32 = _v15 + _v17;
// LOWERING-NEXT:     let _v19: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5, _v6, _v8, _v9, _v18) };
// LOWERING-NEXT:     let _v20: i32 = 0;
// LOWERING-NEXT:     __retval = _v20;
// LOWERING-NEXT:     let _v21: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v21 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct GNUPragmaPacked {
// REWRITES-NEXT:     tag: u8,
// REWRITES-NEXT:     value: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[cfg(target_arch = "x86_64")]
// REWRITES-NEXT: core::arch::global_asm!(".weak gnu_pragma_weak_alias\n.set gnu_pragma_weak_alias, gnu_pragma_weak_target", options(att_syntax, raw));
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut gnu_pragma_inner_macro: i32 = 11;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut gnu_pragma_outer_macro: i32 = 7;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn gnu_pragma_weak_alias() -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_pragma_hidden(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v1: i32 = 13;
// REWRITES-NEXT: __retval = value + _v1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[unsafe(no_mangle)]
// REWRITES-NEXT: pub extern "C" fn gnu_pragma_weak_target() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 17;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_pragma_actual() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 19;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_pragma_diagnostic() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut ignored: i32 = 0;
// REWRITES-NEXT: __retval = 23;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut packed: GNUPragmaPacked = GNUPragmaPacked { tag: 0, value: 0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: packed = GNUPragmaPacked { tag: 29, value: 31 };
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 5;
// REWRITES-NEXT: let _v3: i32 = unsafe { gnu_pragma_inner_macro };
// REWRITES-NEXT: let _v4: i32 = unsafe { gnu_pragma_outer_macro };
// REWRITES-NEXT: let _v5: i32 = 5;
// REWRITES-NEXT: let _v6: i32 = std::mem::offset_of!(GNUPragmaPacked, value) as i32;
// REWRITES-NEXT: let _v7: i32 = 37;
// REWRITES-NEXT: let _v8: i32 = gnu_pragma_hidden(_v7);
// REWRITES-NEXT: let _v9: i32 = unsafe { gnu_pragma_weak_alias() };
// REWRITES-NEXT: let _v10: i32 = gnu_pragma_actual();
// REWRITES-NEXT: let _v11: i32 = gnu_pragma_diagnostic();
// REWRITES-NEXT: let _v19: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5, _v6, _v8, _v9, _v10 + _v11 + (packed.tag as i32) + (packed.value as i32)) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
