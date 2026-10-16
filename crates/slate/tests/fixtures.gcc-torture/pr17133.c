extern void abort(void);

int          foo = 0;
void        *bar = 0;
unsigned int baz = 100;

void *pure_alloc() {
  void *res;

  while (1) {
    res  = (void *)((((unsigned int)(foo + bar))) & ~1);
    foo += 2;
    if (foo < baz)
      return res;
    foo = 0;
  }
}

int main() {
  pure_alloc();
  if (!foo)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut foo: i32 = 0 as i32;
// CHECK-NEXT: static mut bar: *mut () = std::ptr::null_mut::<()>();
// CHECK-NEXT: static mut baz: u32 = (100 as i32) as u32;
// CHECK-NEXT: fn pure_alloc() -> *mut () {
// CHECK-NEXT:     let mut res: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         res = ((((unsafe {
// CHECK-NEXT:             ((unsafe { bar }) as *const u8).offset((unsafe { foo }) as isize)
// CHECK-NEXT:         }) as *mut ()) as u32) & (!(1 as i32) as u32)) as *mut ();
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { foo };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (2 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             foo = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if ((unsafe { foo }) as u32) < unsafe { baz } {
// CHECK-NEXT:             return res;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             foo = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     pure_alloc();
// CHECK-NEXT:     if !((unsafe { foo }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
