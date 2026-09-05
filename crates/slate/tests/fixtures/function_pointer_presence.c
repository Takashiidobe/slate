static int bump(int value) { return value + 1; }

// @lowering-fn-begin
// @rewrite-fn-begin
static int maybe_apply(int (*op)(int), int value) {
  if (op) {
    return op(value);
  }
  return value;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
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
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn maybe_apply({{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32>, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-DAG:     let mut op: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// COMMON-LOWERING-DAG:     let mut value: i32 = 0;
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     op = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     value = {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_some();
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:             return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-DAG:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     let mut op: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// COMMON-LOWERING-DAG:     let mut total: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// COMMON-LOWERING-DAG:     op = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = maybe_apply({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-DAG:     total = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     op = unsafe {
// COMMON-LOWERING-DAG:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-LOWERING-DAG:             bump as *const (),
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = maybe_apply({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             total = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:             std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-DAG:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn maybe_apply(mut op: Option<unsafe extern "C-unwind" fn(i32) -> i32>, mut value: i32) -> i32 {
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = op.is_some();
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         return unsafe { op.unwrap()(value) };
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     value
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut __retval: i32 = 0;
// COMMON-REWRITES-DAG:     let mut op: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// COMMON-REWRITES-DAG:     let mut total: i32 = maybe_apply(op, 4);
// COMMON-REWRITES-DAG:     op = unsafe {
// COMMON-REWRITES-DAG:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-REWRITES-DAG:             bump as *const (),
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = op != None;
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: i32 = total;
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: i32 = maybe_apply(op, 5);
// COMMON-REWRITES-DAG:         total = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = op == None;
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         __retval = 2;
// COMMON-REWRITES-DAG:         std::process::exit(__retval as i32);
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-DAG:     __retval = if total == 10 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-REWRITES-DAG:     std::process::exit(__retval as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
