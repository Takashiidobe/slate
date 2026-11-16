int printf(const char *, ...);
struct cred;
#define is_signed_type(type) (((type)(-1)) < (type)1)
struct field { const char *name; int is_signed; int size; };
static struct field fields[] = {
  { "cred", is_signed_type(const struct cred *), sizeof(const struct cred *) },
  { "u32", is_signed_type(unsigned int), 4 },
  { "int", is_signed_type(int), 4 },
  { "eq", (void *)8 == (void *)8, 0 },
  { "ge", (char *)16 >= (char *)32, 0 },
  {} };
int main(void) {
  for (struct field *f = fields; f->name; f++) printf("%s %d %d\n", f->name, f->is_signed, f->size);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct cred {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct field {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     is_signed: i32,
// CHECK-NEXT:     size: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-NEXT: static mut fields: __SlateAlign16<[field; 6]> = __SlateAlign16([
// CHECK-NEXT:     field {
// CHECK-NEXT:         name: (b"cred\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         is_signed: false as i32,
// CHECK-NEXT:         size: (core::mem::size_of::<*const cred>() as u64) as i32,
// CHECK-NEXT:     },
// CHECK-NEXT:     field {
// CHECK-NEXT:         name: (b"u32\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         is_signed: (4294967295u32 < 1u32) as i32,
// CHECK-NEXT:         size: 4i32,
// CHECK-NEXT:     },
// CHECK-NEXT:     field {
// CHECK-NEXT:         name: (b"int\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         is_signed: (-1i32 < 1i32) as i32,
// CHECK-NEXT:         size: 4i32,
// CHECK-NEXT:     },
// CHECK-NEXT:     field {
// CHECK-NEXT:         name: (b"eq\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         is_signed: true as i32,
// CHECK-NEXT:         size: 0i32,
// CHECK-NEXT:     },
// CHECK-NEXT:     field {
// CHECK-NEXT:         name: (b"ge\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:         is_signed: false as i32,
// CHECK-NEXT:         size: 0i32,
// CHECK-NEXT:     },
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: field = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: *mut field = unsafe { core::ptr::addr_of_mut!(fields.0) as *mut field };
// CHECK-NEXT:     while !unsafe { (*f).name }.is_null() {
// CHECK-NEXT:         unsafe { printf(c"%s %d %d\n".as_ptr(), (*f).name, (*f).is_signed, (*f).size) };
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut field = f;
// CHECK-NEXT:         f = unsafe { [[V_0]].add(1) };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
