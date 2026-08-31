static int bump(int value) { return value + 1; }

static int maybe_apply(int (*op)(int), int value) {
  if (op) {
    return op(value);
  }
  return value;
}

int main(void) {
  int (*op)(int) = 0;
  int total      = maybe_apply(op, 4);
  op             = bump;
  if (op != 0) {
    total = total + maybe_apply(op, 5);
  }
  if (op == 0) {
    return 2;
  }
  return total == 10 ? 0 : 1;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: fn maybe_apply({{arg[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32>, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut op: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     op = {{arg[0-9]+}};
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = op;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.is_some();
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = op;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             return {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn bump({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut op: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:     op = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = op;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = maybe_apply({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     op = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(bump as *const ()) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = op;
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = op;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = maybe_apply({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             total = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = op;
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = None;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: .is_some()
// REWRITES-NOT: != None
// REWRITES-NOT: == None
// REWRITES-NOT: std::ptr::null_mut()
// REWRITES-NOT: let _v{{[0-9]+}}: Option<fn(i32) -> i32> = op;
