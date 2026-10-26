/* { dg-options "/Zc:preprocessor /Gd /std:c11" } */
void abort(void);

int WinMain(void *instance, void *previous, char *line, int show) {
  return show + (line[0] == 'x');
}

int DllMain(void *module, unsigned long reason, void *reserved) {
  return (int)reason * 2;
}

int main(void) {
  int(__stdcall *win)(void *, void *, char *, int) = &WinMain;
  int(__stdcall *dll)(void *, unsigned long, void *) = &DllMain;
  if (win(0, 0, "x", 4) != 5)
    abort();
  if (dll(0, 3, 0) != 6)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor /Gd /std:c11" }
// CHECK-NEXT: extern "stdcall-unwind" fn WinMain(
// CHECK-NEXT:     instance: *mut (),
// CHECK-NEXT:     previous: *mut (),
// CHECK-NEXT:     line: *mut i8,
// CHECK-NEXT:     show: i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     show + ((((unsafe { *line.add(0) }) as i32) == 120) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "stdcall-unwind" fn DllMain(module: *mut (), reason: u32, reserved: *mut ()) -> i32 {
// CHECK-NEXT:     (reason as i32) * 2
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let win: Option<unsafe extern "stdcall-unwind" fn(*mut (), *mut (), *mut i8, i32) -> i32> =
// CHECK-NEXT:         Some(WinMain);
// CHECK-NEXT:     let dll: Option<unsafe extern "stdcall-unwind" fn(*mut (), u32, *mut ()) -> i32> =
// CHECK-NEXT:         Some(DllMain);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         win.unwrap()(
// CHECK-NEXT:             std::ptr::null_mut::<()>(),
// CHECK-NEXT:             std::ptr::null_mut::<()>(),
// CHECK-NEXT:             c"x".as_ptr() as *mut i8,
// CHECK-NEXT:             4i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != 5
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { dll.unwrap()(std::ptr::null_mut::<()>(), 3u32, std::ptr::null_mut::<()>()) }) != 6
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
