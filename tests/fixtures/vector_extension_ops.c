#include <stdio.h>

typedef int v4si __attribute__((vector_size(16)));

int main(void) {
  v4si a = {1, 2, 3, 4};
  v4si b = {5, 6, 7, 8};
  v4si c = a + b;
  c[1]   = 20;
  v4si d = __builtin_shufflevector(c, c, 3, 2, 1, 0);
  printf("%d %d %d\n", c[0], c[1], d[0]);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut b: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut c: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut d: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: [i32; 4] = [1, 2, 3, 4];
// LOWERING-NEXT:     *a = _v1;
// LOWERING-NEXT:     let _v2: [i32; 4] = [5, 6, 7, 8];
// LOWERING-NEXT:     *b = _v2;
// LOWERING-NEXT:     let _v3: [i32; 4] = *a;
// LOWERING-NEXT:     let _v4: [i32; 4] = *b;
// LOWERING-NEXT:     let _v5: [i32; 4] = [_v3[0usize] + _v4[0usize], _v3[1usize] + _v4[1usize], _v3[2usize] + _v4[2usize], _v3[3usize] + _v4[3usize]];
// LOWERING-NEXT:     *c = _v5;
// LOWERING-NEXT:     let _v6: i32 = 20;
// LOWERING-NEXT:     let _v7: i32 = 1;
// LOWERING-NEXT:     let _v8: [i32; 4] = *c;
// LOWERING-NEXT:     let _v9: [i32; 4] = [_v8[0usize], _v6, _v8[2usize], _v8[3usize]];
// LOWERING-NEXT:     *c = _v9;
// LOWERING-NEXT:     let _v10: [i32; 4] = *c;
// LOWERING-NEXT:     let _v11: [i32; 4] = *c;
// LOWERING-NEXT:     let _v12: [i32; 4] = [_v10[3usize], _v10[2usize], _v10[1usize], _v10[0usize]];
// LOWERING-NEXT:     *d = _v12;
// LOWERING-NEXT:     let _v13: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: [i32; 4] = *c;
// LOWERING-NEXT:     let _v15: i32 = 0;
// LOWERING-NEXT:     let _v16: i32 = _v14[(_v15 as usize)];
// LOWERING-NEXT:     let _v17: [i32; 4] = *c;
// LOWERING-NEXT:     let _v18: i32 = 1;
// LOWERING-NEXT:     let _v19: i32 = _v17[(_v18 as usize)];
// LOWERING-NEXT:     let _v20: [i32; 4] = *d;
// LOWERING-NEXT:     let _v21: i32 = 0;
// LOWERING-NEXT:     let _v22: i32 = _v20[(_v21 as usize)];
// LOWERING-NEXT:     let _v23: i32 = unsafe { printf(_v13 as *const i8, _v16, _v19, _v22) };
// LOWERING-NEXT:     let _v24: i32 = 0;
// LOWERING-NEXT:     __retval = _v24;
// LOWERING-NEXT:     let _v25: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v25 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut a: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut b: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut c: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut d: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: *a = [1, 2, 3, 4];
// REWRITES-NEXT: *b = [5, 6, 7, 8];
// REWRITES-NEXT: let _v3: [i32; 4] = *a;
// REWRITES-NEXT: let _v4: [i32; 4] = *b;
// REWRITES-NEXT: *c = [_v3[0usize] + _v4[0usize], _v3[1usize] + _v4[1usize], _v3[2usize] + _v4[2usize], _v3[3usize] + _v4[3usize]];
// REWRITES-NEXT: let _v6: i32 = 20;
// REWRITES-NEXT: let _v7: i32 = 1;
// REWRITES-NEXT: let _v8: [i32; 4] = *c;
// REWRITES-NEXT: *c = [_v8[0usize], _v6, _v8[2usize], _v8[3usize]];
// REWRITES-NEXT: let _v10: [i32; 4] = *c;
// REWRITES-NEXT: let _v11: [i32; 4] = *c;
// REWRITES-NEXT: *d = [_v10[3usize], _v10[2usize], _v10[1usize], _v10[0usize]];
// REWRITES-NEXT: let _v13: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: i32 = 0;
// REWRITES-NEXT: let _v18: i32 = 1;
// REWRITES-NEXT: let _v21: i32 = 0;
// REWRITES-NEXT: let _v23: i32 = unsafe { printf(_v13 as *const i8, (*c)[(_v15 as usize)], (*c)[(_v18 as usize)], (*d)[(_v21 as usize)]) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
