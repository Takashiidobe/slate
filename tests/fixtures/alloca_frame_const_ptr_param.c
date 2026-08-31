typedef struct {
  int windowLog;
} Params;

static void use_params(const Params *params, unsigned *out, const char *lo,
                        const char *hi) {
  long diff = hi - lo;
  *out = (unsigned)(params->windowLog + (int)diff);
}

int main(void) {
  Params p;
  p.windowLog = 5;
  char buf[8];
  unsigned out = 0;
  use_params(&p, &out, buf, buf + 4);
  return (int)out;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Params {
// LOWERING-NEXT:     windowLog: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: fn use_params({{arg[0-9]+}}: *mut Params, {{arg[0-9]+}}: *mut u32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{arg[0-9]+}}.offset_from({{arg[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).windowLog };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut p: Params = Params { windowLog: 0 };
// LOWERING-NEXT:     let mut buf: [i8; 8] = [0; 8];
// LOWERING-NEXT:     let mut out: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     p.windowLog = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     out = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(4) };
// LOWERING-NEXT:     use_params(std::ptr::addr_of_mut!(p), std::ptr::addr_of_mut!(out), {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = out;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Params {
// REWRITES-NEXT:     windowLog: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: fn use_params({{arg[0-9]+}}: &Params, {{arg[0-9]+}}: &mut u32, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) {
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{arg[0-9]+}}.offset_from({{arg[0-9]+}}) as i64 };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *({{arg[0-9]+}} as *mut u32) = ((unsafe { (*({{arg[0-9]+}} as *const Params)).windowLog }) + ({{_v[0-9]+}} as i32)) as u32;
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut p: Params = Params { windowLog: 0 };
// REWRITES-NEXT: let mut buf: [i8; 8] = [0; 8];
// REWRITES-NEXT: let mut out: u32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: p.windowLog = 5;
// REWRITES-NEXT: out = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(4) };
// REWRITES-NEXT: use_params(unsafe { &(*std::ptr::addr_of_mut!(p)) }, unsafe { &mut (*std::ptr::addr_of_mut!(out)) }, {{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-NEXT: std::process::exit((out as i32) as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
