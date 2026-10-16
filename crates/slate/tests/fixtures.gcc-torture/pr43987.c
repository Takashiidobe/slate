char          B[256 * sizeof(void *)];
typedef void *FILE;
typedef struct globals {
  int   c;
  FILE *l;
} __attribute__((may_alias)) T;
void        add_input_file(FILE *file) { (*(T *)&B).l[0] = file; }
extern void abort(void);
int         main() {
  FILE x;
  (*(T *)&B).l = &x;
  add_input_file((void *)-1);
  if ((*(T *)&B).l[0] != (void *)-1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut B: __SlateAlign16<[i8; 2048]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct globals {
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     l: *mut *mut (),
// CHECK-NEXT: }
// CHECK-NEXT: fn add_input_file(mut file: *mut *mut ()) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*((unsafe { std::ptr::addr_of_mut!(B.0) }) as *mut globals)).l }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = file as *mut ();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*((unsafe { std::ptr::addr_of_mut!(B.0) }) as *mut globals)).l = std::ptr::addr_of_mut!(
// CHECK-NEXT:             x
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     add_input_file((-(1 as i32) as *mut ()) as *mut *mut ());
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*((unsafe { std::ptr::addr_of_mut!(B.0) }) as *mut globals)).l }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != (-(1 as i32) as *mut ())
// CHECK-NEXT:     {
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
