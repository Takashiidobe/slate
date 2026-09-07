// { dg-do run }
// { dg-shouldfail "asan" }

int *bar(int *x, int *y) { return y; }

int foo(void) {
  char *p;
  {
    char a = 0;
    p      = &a;
  }

  if (*p)
    return 1;
  else
    return 0;
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  char *ptr;
  {
    char my_char[9];
    ptr = &my_char[0];
  }

  int  a[16];
  int *p, *q = a;
  {
    int b[16];
    p = bar(a, b);
  }
  bar(a, q);
  {
    int c[16];
    q = bar(a, c);
  }
  int v = *bar(a, q);
  return v;
}
// @rewrite-fn-end
// @lowering-fn-end

// { dg-output "ERROR: AddressSanitizer: stack-use-after-scope on address.*(\n|\r\n|\r)" }
// { dg-output "READ of size 4 at.*" }
// { dg-output ".*'c' \\(line 37\\) <== Memory access at offset \[0-9\]* is inside this variable.*" }

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-X86_64-GNU-DAG:     let mut ptr: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:     let mut a: aligned::Aligned<aligned::A16, [i32; 16]> = aligned::Aligned([0; 16]);
// LOWERING-AARCH64-GNU-DAG:     let mut ptr: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:     let mut a: [i32; 16] = [0; 16];
// LOWERING-DAG:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG:     let mut q: *mut i32 = std::ptr::null_mut();
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let mut my_char: [i8; 9] = [0; 9];
// LOWERING-AARCH64-GNU-DAG:         let mut my_char: [u8; 9] = [0; 9];
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         ptr = std::ptr::addr_of_mut!(my_char[({{__v[0-9]+}} as usize)]);
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-DAG:     q = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let mut b: aligned::Aligned<aligned::A16, [i32; 16]> = aligned::Aligned([0; 16]);
// LOWERING-AARCH64-GNU-DAG:         let mut b: [i32; 16] = [0; 16];
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = b.as_mut_ptr() as *mut i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = bar({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:         p = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = q;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = bar({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let mut c: aligned::Aligned<aligned::A16, [i32; 16]> = aligned::Aligned([0; 16]);
// LOWERING-AARCH64-GNU-DAG:         let mut c: [i32; 16] = [0; 16];
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = c.as_mut_ptr() as *mut i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut i32 = bar({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:         q = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = q;
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i32 = bar({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG:     return std::process::ExitCode::from({{__v[0-9]+}} as u8);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-X86_64-GNU-DAG:     let mut ptr: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     let mut a: aligned::Aligned<aligned::A16, [i32; 16]> = aligned::Aligned([0; 16]);
// REWRITES-AARCH64-GNU-DAG:     let mut ptr: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     let mut a: [i32; 16] = [0; 16];
// REWRITES-DAG:     let mut p: *mut i32 = std::ptr::null_mut();
// REWRITES-DAG:     let mut q: *mut i32 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     let mut my_char: [i8; 9] = [0; 9];
// REWRITES-AARCH64-GNU-DAG:     let mut my_char: [u8; 9] = [0; 9];
// REWRITES-DAG:     ptr = std::ptr::addr_of_mut!(my_char[0]);
// REWRITES-DAG:     q = a.as_mut_ptr() as *mut i32;
// REWRITES-X86_64-GNU-DAG:     let mut b: aligned::Aligned<aligned::A16, [i32; 16]> = aligned::Aligned([0; 16]);
// REWRITES-AARCH64-GNU-DAG:     let mut b: [i32; 16] = [0; 16];
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = b.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     p = bar({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:     bar(a.as_mut_ptr() as *mut i32, q);
// REWRITES-X86_64-GNU-DAG:     let mut c: aligned::Aligned<aligned::A16, [i32; 16]> = aligned::Aligned([0; 16]);
// REWRITES-AARCH64-GNU-DAG:     let mut c: [i32; 16] = [0; 16];
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = c.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     q = bar({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = a.as_mut_ptr() as *mut i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i32 = bar({{__v[0-9]+}}, q);
// REWRITES-DAG:     return std::process::ExitCode::from((unsafe { *{{__v[0-9]+}} }) as u8);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
