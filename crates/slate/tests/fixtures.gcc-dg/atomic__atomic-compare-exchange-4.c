/* Test __atomic routines for existence and proper execution on 8 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_long_long_runtime } */
/* { dg-options "" } */
/* { dg-options "-march=pentium" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

/* Test the execution of __atomic_compare_exchange_n builtin for a long_long.  */

extern void abort(void);

long long v        = 0;
long long expected = 0;
long long max      = ~0;
long long desired  = ~0;
long long zero     = 0;

#define STRONG 0
#define WEAK   1

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {

  if (!__atomic_compare_exchange_n(&v, &expected, max, STRONG, __ATOMIC_RELAXED,
                                   __ATOMIC_RELAXED))
    abort();
  if (expected != 0)
    abort();

  if (__atomic_compare_exchange_n(&v, &expected, 0, STRONG, __ATOMIC_ACQUIRE,
                                  __ATOMIC_RELAXED))
    abort();
  if (expected != max)
    abort();

  if (!__atomic_compare_exchange_n(&v, &expected, 0, STRONG, __ATOMIC_RELEASE,
                                   __ATOMIC_ACQUIRE))
    abort();
  if (expected != max)
    abort();
  if (v != 0)
    abort();

  if (__atomic_compare_exchange_n(&v, &expected, desired, WEAK,
                                  __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE))
    abort();
  if (expected != 0)
    abort();

  if (!__atomic_compare_exchange_n(&v, &expected, desired, STRONG,
                                   __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    abort();
  if (expected != 0)
    abort();
  if (v != max)
    abort();

  /* Now test the generic version.  */

  v = 0;

  if (!__atomic_compare_exchange(&v, &expected, &max, STRONG, __ATOMIC_RELAXED,
                                 __ATOMIC_RELAXED))
    abort();
  if (expected != 0)
    abort();

  if (__atomic_compare_exchange(&v, &expected, &zero, STRONG, __ATOMIC_ACQUIRE,
                                __ATOMIC_RELAXED))
    abort();
  if (expected != max)
    abort();

  if (!__atomic_compare_exchange(&v, &expected, &zero, STRONG, __ATOMIC_RELEASE,
                                 __ATOMIC_ACQUIRE))
    abort();
  if (expected != max)
    abort();
  if (v != 0)
    abort();

  if (__atomic_compare_exchange(&v, &expected, &desired, WEAK, __ATOMIC_ACQ_REL,
                                __ATOMIC_ACQUIRE))
    abort();
  if (expected != 0)
    abort();

  if (!__atomic_compare_exchange(&v, &expected, &desired, STRONG,
                                 __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    abort();
  if (expected != 0)
    abort();
  if (v != max)
    abort();

  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// LOWERING-DAG:             Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:             Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 expected = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// LOWERING-DAG:             Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:             Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 expected = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Release,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// LOWERING-DAG:             Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:             Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 expected = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { v };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { desired };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::AcqRel,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// LOWERING-DAG:             Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:             Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 expected = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { desired };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::SeqCst,
// LOWERING-DAG:                 std::sync::atomic::Ordering::SeqCst,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// LOWERING-DAG:             Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:             Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 expected = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { v };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         v = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// LOWERING-DAG:             Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:             Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 expected = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { zero };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// LOWERING-DAG:             Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:             Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 expected = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { zero };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Release,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// LOWERING-DAG:             Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:             Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 expected = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { v };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { desired };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::AcqRel,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// LOWERING-DAG:             Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:             Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 expected = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { desired };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::SeqCst,
// LOWERING-DAG:                 std::sync::atomic::Ordering::SeqCst,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// LOWERING-DAG:             Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:             Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 expected = {{__v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { v };
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { max };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// REWRITES-DAG:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             expected = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// REWRITES-DAG:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             expected = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { max };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::Release,
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// REWRITES-DAG:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             expected = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { max };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { v };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { desired };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::AcqRel,
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// REWRITES-DAG:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             expected = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { desired };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-DAG:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// REWRITES-DAG:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             expected = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { v };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { max };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         v = {{__v[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { max };
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// REWRITES-DAG:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             expected = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { zero };
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// REWRITES-DAG:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             expected = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { max };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { zero };
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::Release,
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// REWRITES-DAG:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             expected = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { max };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { v };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { desired };
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::AcqRel,
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// REWRITES-DAG:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             expected = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { desired };
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i64, i64> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI64::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-DAG:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = match {{__v[0-9]+}} {
// REWRITES-DAG:         Ok({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:         Err({{__v[0-9]+}}) => {{__v[0-9]+}},
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_ok();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             expected = {{__v[0-9]+}};
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { expected };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { v };
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = unsafe { max };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
