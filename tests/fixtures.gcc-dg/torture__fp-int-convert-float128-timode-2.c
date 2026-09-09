/* Test floating-point conversions.  __float128 type with TImode: bug
   53317.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do run } */
/* { dg-require-effective-target __float128 } */
/* { dg-require-effective-target base_quadfloat_support } */
/* { dg-require-effective-target int128 } */
/* { dg-options "" } */
/* { dg-add-options __float128 } */

extern void abort(void);
extern void exit(int);

int
// @lowering-fn-begin
// @rewrite-fn-begin
main(void) {
  volatile unsigned long long a = 0x1000000000000ULL;
  volatile unsigned long long b = 0xffffffffffffffffULL;
  unsigned __int128           c = (((unsigned __int128)a) << 64) | b;
  __float128                  d = c;
  if (d != 0x1.000000000000ffffffffffffffffp112q)
    abort();
  exit(0);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     let mut a: u64 = 0;
// REWRITES-DAG:     let mut b: u64 = 0;
// REWRITES-DAG:     let mut d: f128 = 0.0f128;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 281474976710656u64;
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), {{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 18446744073709551615u64;
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), {{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 64;
// REWRITES-DAG:     let {{__v[0-9]+}}: u128 = ({{__v[0-9]+}} as u128) << {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     d = ({{__v[0-9]+}} | ({{__v[0-9]+}} as u128)) as f128;
// REWRITES-DAG:     let {{__v[0-9]+}}: f128 = d;
// REWRITES-DAG:     let {{__v[0-9]+}}: f128 = f128::from_bits(0x406f000000000000ffffffffffffffff);
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: fn main() -> std::process::ExitCode {
// LOWERING-X86_64-GNU-DAG:     let mut a: u64 = 0;
// LOWERING-X86_64-GNU-DAG:     let mut b: u64 = 0;
// LOWERING-X86_64-GNU-DAG:     let mut d: f128 = 0.0f128;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u64 = 281474976710656u64;
// LOWERING-X86_64-GNU-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u64 = 18446744073709551615u64;
// LOWERING-X86_64-GNU-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u128 = {{__v[0-9]+}} as u128;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 64;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u128 = {{__v[0-9]+}} << {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u128 = {{__v[0-9]+}} as u128;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: u128 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} as f128;
// LOWERING-X86_64-GNU-DAG:     d = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: f128 = d;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: f128 = f128::from_bits(0x406f000000000000ffffffffffffffff);
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:         if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-DAG:             unsafe { abort() };
// LOWERING-X86_64-GNU-DAG:         }
// LOWERING-X86_64-GNU-DAG:     }
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// LOWERING-X86_64-GNU-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering
