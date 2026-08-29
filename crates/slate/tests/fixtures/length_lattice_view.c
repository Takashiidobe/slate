static int sum_n(int *p, int n) {
  int s = 0;
  for (int i = 0; i < n; i++) {
    s += p[i];
  }
  return s;
}

int main(void) {
  int a[5] = {1,2,3,4,5};
  return sum_n(a, 5);
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum_n(arg0: *mut i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut s: i32 = 0;
// LOWERING-NEXT:     p = arg0;
// LOWERING-NEXT:     n = arg1;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     s = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = i;
// LOWERING-NEXT:             let _v3: i32 = n;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v5: i32 = i;
// LOWERING-NEXT:                 let _v6: i64 = _v5 as i64;
// LOWERING-NEXT:                 let _v7: *mut i32 = p;
// LOWERING-NEXT:                 let _v8: *mut i32 = unsafe { _v7.offset(_v6 as isize) };
// LOWERING-NEXT:                 let _v9: i32 = unsafe { *_v8 };
// LOWERING-NEXT:                 let _v10: i32 = s;
// LOWERING-NEXT:                 let _v11: i32 = _v10 + _v9;
// LOWERING-NEXT:                 s = _v11;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v12: i32 = i;
// LOWERING-NEXT:             let _v13: i32 = _v12 + 1;
// LOWERING-NEXT:             i = _v13;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v14: i32 = s;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: i32 = __retval;
// LOWERING-NEXT:     return _v15;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     *a = [1, 2, 3, 4, 5];
// LOWERING-NEXT:     let _v1: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let _v2: i32 = 5;
// LOWERING-NEXT:     let _v3: i32 = sum_n(_v1, _v2);
// LOWERING-NEXT:     __retval = _v3;
// LOWERING-NEXT:     let _v4: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v4 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum_n(arg0: &[i32]) -> i32 {
// REWRITES-NEXT: let mut p: *mut i32 = arg0.as_ptr() as *mut i32;
// REWRITES-NEXT: let mut n: i32 = arg0.len() as i32;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut s: i32 = 0;
// REWRITES-NEXT: s = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i < n) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v7: *mut i32 = p;
// REWRITES-NEXT:                                     let _v8: *mut i32 = unsafe { _v7.offset((i as i64) as isize) };
// REWRITES-NEXT:                                     s = s + unsafe { *_v8 };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = s;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: *a = [1, 2, 3, 4, 5];
// REWRITES-NEXT: let _v1: *mut i32 = a.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v2: i32 = 5;
// REWRITES-NEXT: __retval = sum_n(unsafe { std::slice::from_raw_parts(_v1 as *const i32, _v2 as usize) });
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
