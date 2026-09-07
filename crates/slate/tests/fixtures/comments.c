/** selects an operating mode */
enum Mode {
  /** disable processing */
  MODE_OFF = 0,
  /// enable processing
  MODE_ON  = 1,
};

/** stores a selected mode */
struct Holder {
  /** current mode value */
  enum Mode mode;
};

/** names holder records */
typedef struct Holder Holder;

/** counts completed operations */
static int completed_count = 1;

/** increments a value and records the operation */
static int increment(int value) {
  /** stores the intermediate result */
  volatile int next = value + 1;
  completed_count++;
  return next;
}

int main(void) {
  struct Holder holder = {MODE_ON};
  return holder.mode == MODE_ON && increment(1) == 2 && completed_count == 2
             ? 0
             : 1;
}

// LOWERING-DAG: /// selects an operating mode
// LOWERING-DAG: /// disable processing
// LOWERING-DAG: /// enable processing
// LOWERING-DAG: /// stores a selected mode
// LOWERING-DAG: /// names holder records
// LOWERING-DAG: /// current mode value
// LOWERING-DAG: /// counts completed operations
// LOWERING-DAG: /// increments a value and records the operation
// LOWERING-DAG: /// stores the intermediate result

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
// LOWERING-NEXT: /// selects an operating mode
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum Mode {
// LOWERING-NEXT:     /// disable processing
// LOWERING-NEXT:     MODE_OFF = 0,
// LOWERING-NEXT:     /// enable processing
// LOWERING-NEXT:     MODE_ON = 1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: /// stores a selected mode
// LOWERING-NEXT: /// names holder records
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Holder {
// LOWERING-NEXT:     /// current mode value
// LOWERING-NEXT:     mode: Mode,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: /// counts completed operations
// LOWERING-NEXT: static mut completed_count: i32 = 1;
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut holder: Holder = Holder {
// LOWERING-NEXT:         mode: Mode::MODE_OFF,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: Holder = Holder {
// LOWERING-NEXT:         mode: Mode::MODE_ON,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     holder = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = holder.mode as u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = increment({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { completed_count };
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: /// increments a value and records the operation
// LOWERING-NEXT: /// stores the intermediate result
// LOWERING-NEXT: fn increment({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut next: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(next), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { completed_count };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         completed_count = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(next)) };
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
// REWRITES-NEXT: /// selects an operating mode
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum Mode {
// REWRITES-NEXT:     /// disable processing
// REWRITES-NEXT:     MODE_OFF = 0,
// REWRITES-NEXT:     /// enable processing
// REWRITES-NEXT:     MODE_ON = 1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: /// stores a selected mode
// REWRITES-NEXT: /// names holder records
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Holder {
// REWRITES-NEXT:     /// current mode value
// REWRITES-NEXT:     mode: Mode,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: /// counts completed operations
// REWRITES-NEXT: static mut completed_count: i32 = 1;
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut holder: Holder = Holder {
// REWRITES-NEXT:         mode: Mode::MODE_OFF,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: Holder = Holder {
// REWRITES-NEXT:         mode: Mode::MODE_ON,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     holder = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = (holder.mode as u32) == 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = increment(1);
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == 2;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} && (unsafe { completed_count }) == 2 {
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: /// increments a value and records the operation
// REWRITES-NEXT: /// stores the intermediate result
// REWRITES-NEXT: fn increment({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut next: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + 1;
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(next), {{__v[0-9]+}}) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         completed_count = (unsafe { completed_count }) + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(next)) }
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
