#include <stddef.h>
#include <stdio.h>

static int notify(void *fence, unsigned state) { return (int)state; }
static int other(void *fence, unsigned state) { return 1; }

#define BUILD_CHECK(cond)                                                      \
  do {                                                                         \
    if (cond)                                                                  \
      puts("unreachable");                                                     \
  } while (0)

#define fence_init(fn) BUILD_CHECK((fn) == NULL)

int main(void) {
  int (*fp)(void *, unsigned) = other;
  fence_init(notify);
  printf("%d %d %d %d\n", notify == NULL, notify != NULL, NULL == notify,
         fp == notify);
  if (notify == NULL)
    return 1;
  return notify(NULL, 0);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! BUILD_CHECK {
// CHECK-NEXT:     ($cond:expr) => {
// CHECK-NEXT:         loop { if $cond { let _ = std::io::Write::flush(& mut std::io::stdout()); unsafe
// CHECK-NEXT:         { slate_support::puts(c"unreachable".as_ptr()) }; unsafe {
// CHECK-NEXT:         fflush(core::ptr::null_mut()) }; } if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! fence_init {
// CHECK-NEXT:     ($fn:expr) => {
// CHECK-NEXT:         BUILD_CHECK!(({ let __slate_function : Option < unsafe extern "C-unwind" fn (*
// CHECK-NEXT:         mut (), u32) -> i32 > = $fn; __slate_function }) == None);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.notify")]
// CHECK-NEXT: extern "C-unwind" fn notify(fence: *mut (), state: u32) -> i32 {
// CHECK-NEXT:     state as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.other")]
// CHECK-NEXT: extern "C-unwind" fn other(fence: *mut (), state: u32) -> i32 {
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let fp: Option<unsafe extern "C-unwind" fn(*mut (), u32) -> i32> = Some(other);
// CHECK-NEXT:     fence_init!(Some(notify));
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", (({ let __slate_function : Option < unsafe extern "C-unwind" fn (*
// CHECK-NEXT:         mut (), u32) -> i32 > = Some(notify); __slate_function }) == None) as i32, (({
// CHECK-NEXT:         let __slate_function : Option < unsafe extern "C-unwind" fn (* mut (), u32) ->
// CHECK-NEXT:         i32 > = Some(notify); __slate_function }) != None) as i32, unsafe {
// CHECK-NEXT:         core::mem::transmute::< Option < unsafe extern "C-unwind" fn (* mut (), u32) ->
// CHECK-NEXT:         i32 >, * mut () > (Some(notify)) } .is_null() as i32, (fp == Some(notify)) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     if ({
// CHECK-NEXT:         let __slate_function: Option<unsafe extern "C-unwind" fn(*mut (), u32) -> i32> = Some(
// CHECK-NEXT:             notify,
// CHECK-NEXT:         );
// CHECK-NEXT:         __slate_function
// CHECK-NEXT:     }) == None
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     notify(core::ptr::null_mut::<()>(), 0u32)
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
