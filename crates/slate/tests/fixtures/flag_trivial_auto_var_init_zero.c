/* { dg-options "-ftrivial-auto-var-init=zero" } */
int printf(const char *, ...);

struct padded {
  char c;
  int i;
  double d[2];
};

static void dump(const char *name, const void *p, unsigned size) {
  const unsigned char *bytes = p;
  unsigned sum = 0;
  for (unsigned i = 0; i < size; i++)
    sum |= bytes[i];
  printf("%s: %u bytes, or=%02x\n", name, size, sum);
}

int main(void) {
  int i;
  float f;
  void *p;
  _Bool b;
  int arr[64];
  struct padded s;
  struct padded many[4];
  dump("int", &i, sizeof i);
  dump("float", &f, sizeof f);
  dump("pointer", &p, sizeof p);
  dump("bool", &b, sizeof b);
  dump("array", arr, sizeof arr);
  dump("padded", &s, sizeof s);
  dump("padded[]", many, sizeof many);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-ftrivial-auto-var-init=zero" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct padded {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     d: [f64; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn dump(name: *const i8, p: *const (), size: u32) {
// CHECK-NEXT:     let bytes: *const u8 = p as *const u8;
// CHECK-NEXT:     let mut sum: u32 = 0;
// CHECK-NEXT:     for i in 0..size {
// CHECK-NEXT:         sum |= (((unsafe { *bytes.add(i as usize) }) as u32) as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { printf(c"%s: %u bytes, or=%02x\n".as_ptr(), name, size, sum) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     let mut f: f32 = 0 as f32;
// CHECK-NEXT:     let mut p: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: bool = false;
// CHECK-NEXT:     let mut arr: __SlateAlign16<[i32; 64]> = __SlateAlign16([0i32; 64]);
// CHECK-NEXT:     let mut s: padded = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(s) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 24usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut many: __SlateAlign16<[padded; 4]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let __slate_auto_bytes = (std::ptr::addr_of_mut!(many.0) as *mut _) as *mut u8;
// CHECK-NEXT:         std::ptr::write_bytes(__slate_auto_bytes.add(0usize), 0u8, 96usize);
// CHECK-NEXT:     }
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"int".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(i) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"float".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(f) as *const (),
// CHECK-NEXT:         4u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"pointer".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(p) as *const (),
// CHECK-NEXT:         8u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"bool".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(b) as *const (),
// CHECK-NEXT:         1u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(c"array".as_ptr(), arr.0.as_mut_ptr() as *const (), 256u32);
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"padded".as_ptr(),
// CHECK-NEXT:         std::ptr::addr_of_mut!(s) as *const (),
// CHECK-NEXT:         24u32,
// CHECK-NEXT:     );
// CHECK-NEXT:     dump(
// CHECK-NEXT:         c"padded[]".as_ptr(),
// CHECK-NEXT:         many.0.as_mut_ptr() as *const (),
// CHECK-NEXT:         96u32,
// CHECK-NEXT:     );
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
