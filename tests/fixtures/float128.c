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
// LOWERING-NEXT: fn add(arg0: f128, arg1: f128) -> f128 {
// LOWERING-NEXT:     let mut a: f128 = 0.0f128;
// LOWERING-NEXT:     let mut b: f128 = 0.0f128;
// LOWERING-NEXT:     let mut __retval: f128 = 0.0f128;
// LOWERING-NEXT:     a = arg0;
// LOWERING-NEXT:     b = arg1;
// LOWERING-NEXT:     let _v0: f128 = a;
// LOWERING-NEXT:     let _v1: f128 = b;
// LOWERING-NEXT:     let _v2: f128 = _v0 + _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: f128 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut one: f128 = 0.0f128;
// LOWERING-NEXT:     let mut tiny: f128 = 0.0f128;
// LOWERING-NEXT:     let mut sum: f128 = 0.0f128;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: f128 = 1.000000e+00f128;
// LOWERING-NEXT:     one = _v1;
// LOWERING-NEXT:     let _v2: f128 = 7.88860905221011805411728565282786229E-31f128;
// LOWERING-NEXT:     tiny = _v2;
// LOWERING-NEXT:     let _v3: f128 = one;
// LOWERING-NEXT:     let _v4: f128 = tiny;
// LOWERING-NEXT:     let _v5: f128 = add(_v3, _v4);
// LOWERING-NEXT:     sum = _v5;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v6: f128 = sum;
// LOWERING-NEXT:         let _v7: f128 = one;
// LOWERING-NEXT:         let _v8: bool = _v6 == _v7;
// LOWERING-NEXT:         if _v8 {
// LOWERING-NEXT:             let _v9: i32 = 1;
// LOWERING-NEXT:             __retval = _v9;
// LOWERING-NEXT:             let _v10: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v10 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v11: f128 = sum;
// LOWERING-NEXT:         let _v12: f128 = one;
// LOWERING-NEXT:         let _v13: f128 = _v11 - _v12;
// LOWERING-NEXT:         let _v14: f128 = tiny;
// LOWERING-NEXT:         let _v15: bool = _v13 != _v14;
// LOWERING-NEXT:         if _v15 {
// LOWERING-NEXT:             let _v16: i32 = 2;
// LOWERING-NEXT:             __retval = _v16;
// LOWERING-NEXT:             let _v17: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v17 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v18: i32 = 42;
// LOWERING-NEXT:         let _v19: f128 = _v18 as f128;
// LOWERING-NEXT:         let _v20: f128 = 4.200000e+01f128;
// LOWERING-NEXT:         let _v21: bool = _v19 != _v20;
// LOWERING-NEXT:         if _v21 {
// LOWERING-NEXT:             let _v22: i32 = 3;
// LOWERING-NEXT:             __retval = _v22;
// LOWERING-NEXT:             let _v23: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v23 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v24: f128 = 4.275000e+01f128;
// LOWERING-NEXT:         let _v25: i32 = _v24 as i32;
// LOWERING-NEXT:         let _v26: i32 = 42;
// LOWERING-NEXT:         let _v27: bool = _v25 != _v26;
// LOWERING-NEXT:         if _v27 {
// LOWERING-NEXT:             let _v28: i32 = 4;
// LOWERING-NEXT:             __retval = _v28;
// LOWERING-NEXT:             let _v29: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v29 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v30: f128 = 1.500000e+00f128;
// LOWERING-NEXT:         let _v31: f64 = _v30 as f64;
// LOWERING-NEXT:         let _v32: f64 = 1.5;
// LOWERING-NEXT:         let _v33: bool = _v31 != _v32;
// LOWERING-NEXT:         if _v33 {
// LOWERING-NEXT:             let _v34: i32 = 5;
// LOWERING-NEXT:             __retval = _v34;
// LOWERING-NEXT:             let _v35: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v35 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v36: u64 = 16;
// LOWERING-NEXT:         let _v37: u64 = 16;
// LOWERING-NEXT:         let _v38: bool = _v36 != _v37;
// LOWERING-NEXT:         if _v38 {
// LOWERING-NEXT:             let _v39: i32 = 6;
// LOWERING-NEXT:             __retval = _v39;
// LOWERING-NEXT:             let _v40: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v40 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v41: u64 = 16;
// LOWERING-NEXT:         let _v42: u64 = 16;
// LOWERING-NEXT:         let _v43: bool = _v41 != _v42;
// LOWERING-NEXT:         if _v43 {
// LOWERING-NEXT:             let _v44: i32 = 7;
// LOWERING-NEXT:             __retval = _v44;
// LOWERING-NEXT:             let _v45: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v45 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v46: i32 = 0;
// LOWERING-NEXT:     __retval = _v46;
// LOWERING-NEXT:     let _v47: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v47 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(f128)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add(arg0: f128, arg1: f128) -> f128 {
// REWRITES-NEXT: let mut a: f128 = 0.0f128;
// REWRITES-NEXT: let mut b: f128 = 0.0f128;
// REWRITES-NEXT: let mut __retval: f128 = 0.0f128;
// REWRITES-NEXT: a = arg0;
// REWRITES-NEXT: b = arg1;
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
// REWRITES-NEXT: let _v1: f128 = 1.000000e+00f128;
// REWRITES-NEXT: one = _v1;
// REWRITES-NEXT: let _v2: f128 = 7.88860905221011805411728565282786229E-31f128;
// REWRITES-NEXT: tiny = _v2;
// REWRITES-NEXT: sum = add(one, tiny);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v8: bool = sum == one;
// REWRITES-NEXT:         if _v8 {
// REWRITES-NEXT:                     __retval = 1;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v15: bool = sum - one != tiny;
// REWRITES-NEXT:         if _v15 {
// REWRITES-NEXT:                     __retval = 2;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v18: i32 = 42;
// REWRITES-NEXT:         let _v19: f128 = _v18 as f128;
// REWRITES-NEXT:         let _v20: f128 = 4.200000e+01f128;
// REWRITES-NEXT:         let _v21: bool = _v19 != _v20;
// REWRITES-NEXT:         if _v21 {
// REWRITES-NEXT:                     __retval = 3;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v24: f128 = 4.275000e+01f128;
// REWRITES-NEXT:         let _v26: i32 = 42;
// REWRITES-NEXT:         let _v27: bool = (_v24 as i32) != _v26;
// REWRITES-NEXT:         if _v27 {
// REWRITES-NEXT:                     __retval = 4;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v30: f128 = 1.500000e+00f128;
// REWRITES-NEXT:         let _v32: f64 = 1.5;
// REWRITES-NEXT:         let _v33: bool = (_v30 as f64) != _v32;
// REWRITES-NEXT:         if _v33 {
// REWRITES-NEXT:                     __retval = 5;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v36: u64 = 16;
// REWRITES-NEXT:         let _v37: u64 = 16;
// REWRITES-NEXT:         let _v38: bool = _v36 != _v37;
// REWRITES-NEXT:         if _v38 {
// REWRITES-NEXT:                     __retval = 6;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v41: u64 = 16;
// REWRITES-NEXT:         let _v42: u64 = 16;
// REWRITES-NEXT:         let _v43: bool = _v41 != _v42;
// REWRITES-NEXT:         if _v43 {
// REWRITES-NEXT:                     __retval = 7;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
