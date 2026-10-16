extern void abort(void);

static int max;

static void __attribute__((noinline)) storemax(int i) {
  if (i > max)
    max = i;
}

static int CallFunctionRec(int (*fun)(int depth), int depth) {
  if (!fun(depth)) {
    return 0;
  }
  if (depth < 10) {
    CallFunctionRec(fun, depth + 1);
  }
  return 1;
}

static int CallFunction(int (*fun)(int depth)) {
  return CallFunctionRec(fun, 1) && !fun(0);
}

static int callback(int depth) {
  storemax(depth);
  return depth != 0;
}

int main() {
  CallFunction(callback);
  if (max != 10)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut max: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn storemax(mut i: i32) {
// CHECK-NEXT:     if i > unsafe { max } {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             max = i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn CallFunctionRec(
// CHECK-NEXT:     mut fun: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:     mut depth: i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     if !((unsafe { fun.unwrap()(depth) }) != (0 as i32)) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if depth < (10 as i32) {
// CHECK-NEXT:         CallFunctionRec(fun, depth + (1 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn CallFunction(mut fun: Option<unsafe extern "C-unwind" fn(i32) -> i32>) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if CallFunctionRec(fun, 1 as i32) != (0 as i32) {
// CHECK-NEXT:         [[V_0]] = !((unsafe { fun.unwrap()(0 as i32) }) != (0 as i32));
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]] as i32
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn callback(mut depth: i32) -> i32 {
// CHECK-NEXT:     storemax(depth);
// CHECK-NEXT:     (depth != (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     CallFunction(Some(callback));
// CHECK-NEXT:     if (unsafe { max }) != (10 as i32) {
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
