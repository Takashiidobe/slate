typedef struct {
  int windowLog;
} Params;

static void use_params(const Params *params, unsigned *out, const char *lo,
                       const char *hi) {
  long diff = hi - lo;
  *out      = (unsigned)(params->windowLog + (int)diff);
}

int main(void) {
  Params p;
  p.windowLog = 5;
  char     buf[8];
  unsigned out = 0;
  use_params(&p, &out, buf, buf + 4);
  return (int)out;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Params {
// COMMON-LOWERING-NEXT:     windowLog: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut p: Params = Params { windowLog: 0 };
// COMMON-LOWERING-NEXT:     let mut out: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     p.windowLog = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     out = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     use_params(
// COMMON-LOWERING-NEXT:         std::ptr::addr_of_mut!(p),
// COMMON-LOWERING-NEXT:         std::ptr::addr_of_mut!(out),
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = out;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{arg[0-9]+}}.offset_from({{arg[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).windowLog };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut buf: [i8; 8] = [0; 8];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(4) };
// LOWERING-X86_64-GNU-NEXT: fn use_params({{arg[0-9]+}}: *mut Params, {{arg[0-9]+}}: *mut u32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) {
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut buf: [u8; 8] = [0; 8];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(4) };
// LOWERING-AARCH64-GNU-NEXT: fn use_params({{arg[0-9]+}}: *mut Params, {{arg[0-9]+}}: *mut u32, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) {
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Params {
// COMMON-REWRITES-NEXT:     windowLog: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut p: Params = Params { windowLog: 0 };
// COMMON-REWRITES-NEXT:     let mut out: u32 = 0;
// COMMON-REWRITES-NEXT:     p.windowLog = 5;
// COMMON-REWRITES-NEXT:     use_params(
// COMMON-REWRITES-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(p)) },
// COMMON-REWRITES-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(out)) },
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         unsafe { {{__v[0-9]+}}.add(4) },
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     std::process::exit(out as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{arg[0-9]+}}.offset_from({{arg[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *({{arg[0-9]+}} as *mut u32) =
// COMMON-REWRITES-NEXT:             ((unsafe { (*({{arg[0-9]+}} as *const Params)).windowLog }) + ({{__v[0-9]+}} as i32)) as u32;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut buf: [i8; 8] = [0; 8];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT: fn use_params({{arg[0-9]+}}: &Params, {{arg[0-9]+}}: &mut u32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) {
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut buf: [u8; 8] = [0; 8];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = buf.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT: fn use_params({{arg[0-9]+}}: &Params, {{arg[0-9]+}}: &mut u32, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) {
// SLATE-FILECHECK-END rewrites-aarch64-gnu
