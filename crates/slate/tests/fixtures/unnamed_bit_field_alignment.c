int printf(const char *, ...);

struct plain { char c; int : 4; };
struct __attribute__((packed)) packed { char c; int : 4; };
struct wide { char c; long long : 4; };
struct zero_width { char c; int : 0; char d; };
#pragma pack(4)
struct __attribute__((packed)) packed_pragma { char c; int : 4; };
struct pragma { char c; long long : 4; };
#pragma pack()
struct named_after { char c; int : 4; short s; };
union unnamed_union { int : 1; };
union named_union { int x : 1; };
union mixed_union { int : 20; char c; };
#pragma pack(1)
union pragma_union { int x : 20; };
#pragma pack()

#define SHOW(T) printf("%s %zu %zu\n", #T, sizeof(struct T), _Alignof(struct T))
#define SHOW_UNION(T) printf("%s %zu %zu\n", #T, sizeof(union T), _Alignof(union T))

int main(void) {
  SHOW(plain);
  SHOW(packed);
  SHOW(wide);
  SHOW(zero_width);
  SHOW(packed_pragma);
  SHOW(pragma);
  SHOW(named_after);
  SHOW_UNION(unnamed_union);
  SHOW_UNION(named_union);
  SHOW_UNION(mixed_union);
  SHOW_UNION(pragma_union);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %zu %zu\n".as_ptr(), c"plain".as_ptr() as *mut i8, 2u64, 1u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %zu %zu\n".as_ptr(), c"packed".as_ptr() as *mut i8, 2u64, 1u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { printf(c"%s %zu %zu\n".as_ptr(), c"wide".as_ptr() as *mut i8, 2u64, 1u64) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %zu %zu\n".as_ptr(), c"zero_width".as_ptr() as *mut i8, 5u64, 1u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"packed_pragma".as_ptr() as *mut i8,
// CHECK-NEXT:             2u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %zu %zu\n".as_ptr(), c"pragma".as_ptr() as *mut i8, 2u64, 1u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %zu %zu\n".as_ptr(), c"named_after".as_ptr() as *mut i8, 4u64, 2u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"unnamed_union".as_ptr() as *mut i8,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %zu %zu\n".as_ptr(), c"named_union".as_ptr() as *mut i8, 4u64, 4u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %zu %zu\n".as_ptr(), c"mixed_union".as_ptr() as *mut i8, 3u64, 1u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %zu %zu\n".as_ptr(), c"pragma_union".as_ptr() as *mut i8, 3u64, 1u64)
// CHECK-NEXT:     };
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
