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
// REWRITES-NEXT: /// increments a value and records the operation
// REWRITES-NEXT: /// stores the intermediate result
// REWRITES-NEXT: fn increment({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut next: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(next), {{arg[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         completed_count = (unsafe { completed_count }) + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return unsafe { std::ptr::read_volatile(std::ptr::addr_of!(next)) };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut holder: Holder = Holder {
// REWRITES-NEXT:         mode: Mode::MODE_OFF,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     holder = Holder {
// REWRITES-NEXT:         mode: Mode::MODE_ON,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = (holder.mode as u32) == {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = increment(1);
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (unsafe { completed_count }) == {{_v[0-9]+}};
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
