static int sum_n(int *p, int n) {
  int s = 0;
  for (int i = 0; i < n; i++) {
    s += p[i];
  }
  return s;
}

int main(void) {
  int a[5] = {1, 2, 3, 4, 5};
  return sum_n(a, 5);
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-AARCH64-GNU-NEXT:     let mut a: [i32; 5] = [0; 5];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 5] = [1, 2, 3, 4, 5];
// LOWERING-X86_64-GNU-NEXT:     *a = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     a = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sum_n({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum_n({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut s: i32 = 0;
// LOWERING-NEXT:     p = {{arg[0-9]+}};
// LOWERING-NEXT:     n = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     s = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = n;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = s;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                 s = {{__v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = s;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-X86_64-GNU-NEXT:     *a = [1, 2, 3, 4, 5];
// REWRITES-AARCH64-GNU-NEXT:     let mut a: [i32; 5] = [0; 5];
// REWRITES-AARCH64-GNU-NEXT:     a = [1, 2, 3, 4, 5];
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     std::process::exit(sum_n(unsafe {
// REWRITES-NEXT:         std::slice::from_raw_parts({{__v[0-9]+}} as *const i32, (5 as i32) as usize)
// REWRITES-NEXT:     }) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum_n({{arg[0-9]+}}: &[i32]) -> i32 {
// REWRITES-NEXT:     let mut p: *mut i32 = {{arg[0-9]+}}.as_ptr() as *mut i32;
// REWRITES-NEXT:     let mut n: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     let mut s: i32 = 0;
// REWRITES-NEXT:     for i in 0..n {
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = p;
// REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:         s += unsafe { *{{__v[0-9]+}} };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     s
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
