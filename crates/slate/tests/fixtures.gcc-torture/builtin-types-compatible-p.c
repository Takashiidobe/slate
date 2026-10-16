void abort(void);
void exit(int);

int    i;
double d;

/* Make sure we return a constant.  */
float rootbeer[__builtin_types_compatible_p(int, typeof(i))];

typedef enum { hot, dog, poo, bear } dingos;
typedef enum { janette, laura, amanda } cranberry;

typedef float same1;
typedef float same2;

int main(void);

int main(void) {
  /* Compatible types.  */
  if (!(__builtin_types_compatible_p(int, const int) &&
        __builtin_types_compatible_p(typeof(hot), int) &&
        __builtin_types_compatible_p(typeof(hot), typeof(laura)) &&
        __builtin_types_compatible_p(int[5], int[]) &&
        __builtin_types_compatible_p(same1, same2)))
    abort();

  /* Incompatible types.  */
  if (__builtin_types_compatible_p(char *, int) ||
      __builtin_types_compatible_p(char *, const char *) ||
      __builtin_types_compatible_p(long double, double) ||
      __builtin_types_compatible_p(typeof(i), typeof(d)) ||
      __builtin_types_compatible_p(typeof(dingos), typeof(cranberry)) ||
      __builtin_types_compatible_p(char, int) ||
      __builtin_types_compatible_p(char *, char **))
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: f64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: /// Make sure we return a constant.
// CHECK-NEXT: static mut rootbeer: [f32; 1] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // Compatible types.
// CHECK-NEXT:     if !((1 as i32) != (0 as i32)
// CHECK-NEXT:         && (1 as i32) != (0 as i32)
// CHECK-NEXT:         && (1 as i32) != (0 as i32)
// CHECK-NEXT:         && (1 as i32) != (0 as i32)
// CHECK-NEXT:         && (1 as i32) != (0 as i32))
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // Incompatible types.
// CHECK-NEXT:     if (0 as i32) != (0 as i32)
// CHECK-NEXT:         || (0 as i32) != (0 as i32)
// CHECK-NEXT:         || (0 as i32) != (0 as i32)
// CHECK-NEXT:         || (0 as i32) != (0 as i32)
// CHECK-NEXT:         || (0 as i32) != (0 as i32)
// CHECK-NEXT:         || (0 as i32) != (0 as i32)
// CHECK-NEXT:         || (0 as i32) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
