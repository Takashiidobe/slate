__float128 add(__float128 a, __float128 b) { return a + b; }

int main(void) {
  __float128 one  = 1.0Q;
  __float128 tiny = 0x1p-100Q;
  __float128 sum  = add(one, tiny);
  if (sum == one)
    return 1;
  if (sum - one != tiny)
    return 2;
  if ((__float128)42 != 42.0Q)
    return 3;
  if ((int)42.75Q != 42)
    return 4;
  if ((double)1.5Q != 1.5)
    return 5;
  if (sizeof(__float128) != 16)
    return 6;
  if (_Alignof(__float128) != 16)
    return 7;
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(f128)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// LOWERING-NEXT:     let mut a: f128 = 0.0f128;
// LOWERING-NEXT:     let mut b: f128 = 0.0f128;
// LOWERING-NEXT:     let mut __retval: f128 = 0.0f128;
// LOWERING-NEXT:     a = {{arg[0-9]+}};
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f128 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f128 = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f128 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f128 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut one: f128 = 0.0f128;
// LOWERING-NEXT:     let mut tiny: f128 = 0.0f128;
// LOWERING-NEXT:     let mut sum: f128 = 0.0f128;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f128 = 1.000000e+00f128;
// LOWERING-NEXT:     one = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f128 = 7.88860905221011805411728565282786229E-31f128;
// LOWERING-NEXT:     tiny = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f128 = one;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f128 = tiny;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f128 = add({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     sum = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f128 = sum;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f128 = one;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f128 = sum;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f128 = one;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f128 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: f128 = tiny;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f128 = {{_v[0-9]+}} as f128;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f128 = 4.200000e+01f128;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f128 = 4.275000e+01f128;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f128 = 1.500000e+00f128;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 1.5;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(f128)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add({{arg[0-9]+}}: f128, {{arg[0-9]+}}: f128) -> f128 {
// REWRITES-NEXT: let mut a: f128 = 0.0f128;
// REWRITES-NEXT: let mut b: f128 = 0.0f128;
// REWRITES-NEXT: let mut __retval: f128 = 0.0f128;
// REWRITES-NEXT: a = {{arg[0-9]+}};
// REWRITES-NEXT: b = {{arg[0-9]+}};
// REWRITES-NEXT: __retval = a + b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut one: f128 = 0.0f128;
// REWRITES-NEXT: let mut tiny: f128 = 0.0f128;
// REWRITES-NEXT: let mut sum: f128 = 0.0f128;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f128 = 1.000000e+00f128;
// REWRITES-NEXT: one = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: f128 = 7.88860905221011805411728565282786229E-31f128;
// REWRITES-NEXT: tiny = {{_v[0-9]+}};
// REWRITES-NEXT: sum = add(one, tiny);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = sum == one;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     __retval = 1;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = sum - one != tiny;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     __retval = 2;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 42;
// REWRITES-NEXT:         let {{_v[0-9]+}}: f128 = {{_v[0-9]+}} as f128;
// REWRITES-NEXT:         let {{_v[0-9]+}}: f128 = 4.200000e+01f128;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     __retval = 3;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: f128 = 4.275000e+01f128;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 42;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = ({{_v[0-9]+}} as i32) != {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     __retval = 4;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: f128 = 1.500000e+00f128;
// REWRITES-NEXT:         let {{_v[0-9]+}}: f64 = 1.5;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = ({{_v[0-9]+}} as f64) != {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     __retval = 5;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     __retval = 6;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     __retval = 7;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
