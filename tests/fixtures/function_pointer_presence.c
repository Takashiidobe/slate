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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn maybe_apply({{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32>, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let mut op: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// LOWERING-DAG:     let mut value: i32 = 0;
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     op = {{arg[0-9]+}};
// LOWERING-DAG:     value = {{arg[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_some();
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = value;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// LOWERING-DAG:             __retval = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:             return {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = value;
// LOWERING-DAG:     __retval = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut op: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// LOWERING-DAG:     let mut total: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     __retval = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// LOWERING-DAG:     op = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = maybe_apply({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:     total = {{__v[0-9]+}};
// LOWERING-DAG:     op = unsafe {
// LOWERING-DAG:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// LOWERING-DAG:             bump as *const (),
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// LOWERING-DAG:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = total;
// LOWERING-DAG:             let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 5;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = maybe_apply({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:             total = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = op;
// LOWERING-DAG:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:             __retval = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:             std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = total;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 10;
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:     __retval = {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn maybe_apply(mut op: Option<unsafe extern "C-unwind" fn(i32) -> i32>, mut value: i32) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = op.is_some();
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         return unsafe { op.unwrap()(value) };
// REWRITES-DAG:     }
// REWRITES-DAG:     value
// REWRITES-DAG: }
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     let mut op: Option<unsafe extern "C-unwind" fn(i32) -> i32> = None;
// REWRITES-DAG:     let mut total: i32 = maybe_apply(op, 4);
// REWRITES-DAG:     op = unsafe {
// REWRITES-DAG:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// REWRITES-DAG:             bump as *const (),
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = op != None;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = total;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = maybe_apply(op, 5);
// REWRITES-DAG:         total = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = op == None;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         __retval = 2;
// REWRITES-DAG:         std::process::exit(__retval as i32);
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     __retval = if total == 10 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// REWRITES-DAG:     std::process::exit(__retval as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
