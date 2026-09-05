/* Test __atomic routines for existence and proper execution on 1 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

/* Test the execution of the __atomic_compare_exchange_n builtin for a char.  */

extern void abort(void);

char v        = 0;
char expected = 0;
char max      = ~0;
char desired  = ~0;
char zero     = 0;

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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Release,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { v };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { desired };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::AcqRel,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { desired };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::SeqCst,
// LOWERING-DAG:                 std::sync::atomic::Ordering::SeqCst,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { v };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i8 = 0;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         v = {{__v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { zero };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Relaxed,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { zero };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::Release,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { v };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { desired };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::AcqRel,
// LOWERING-DAG:                 std::sync::atomic::Ordering::Acquire,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { desired };
// LOWERING-DAG:         let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// LOWERING-DAG:             std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 {{__v[0-9]+}},
// LOWERING-DAG:                 std::sync::atomic::Ordering::SeqCst,
// LOWERING-DAG:                 std::sync::atomic::Ordering::SeqCst,
// LOWERING-DAG:             )
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { expected };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { v };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i8 = unsafe { max };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
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
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             unsafe { expected },
// REWRITES-DAG:             unsafe { max },
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { expected }) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             unsafe { expected },
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { expected }) as i32) != ((unsafe { max }) as i32);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             unsafe { expected },
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             std::sync::atomic::Ordering::Release,
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { expected }) as i32) != ((unsafe { max }) as i32);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { v }) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             unsafe { expected },
// REWRITES-DAG:             unsafe { desired },
// REWRITES-DAG:             std::sync::atomic::Ordering::AcqRel,
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { expected }) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             unsafe { expected },
// REWRITES-DAG:             unsafe { desired },
// REWRITES-DAG:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-DAG:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { expected }) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { v }) as i32) != ((unsafe { max }) as i32);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         v = 0;
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             unsafe { expected },
// REWRITES-DAG:             unsafe { max },
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { expected }) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             unsafe { expected },
// REWRITES-DAG:             unsafe { zero },
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:             std::sync::atomic::Ordering::Relaxed,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { expected }) as i32) != ((unsafe { max }) as i32);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             unsafe { expected },
// REWRITES-DAG:             unsafe { zero },
// REWRITES-DAG:             std::sync::atomic::Ordering::Release,
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { expected }) as i32) != ((unsafe { max }) as i32);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { v }) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             unsafe { expected },
// REWRITES-DAG:             unsafe { desired },
// REWRITES-DAG:             std::sync::atomic::Ordering::AcqRel,
// REWRITES-DAG:             std::sync::atomic::Ordering::Acquire,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { expected }) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: Result<i8, i8> = unsafe {
// REWRITES-DAG:         std::sync::atomic::AtomicI8::from_ptr(std::ptr::addr_of_mut!(v)).compare_exchange(
// REWRITES-DAG:             unsafe { expected },
// REWRITES-DAG:             unsafe { desired },
// REWRITES-DAG:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-DAG:             std::sync::atomic::Ordering::SeqCst,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i8 = match {{__v[0-9]+}} {
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
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { expected }) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ((unsafe { v }) as i32) != ((unsafe { max }) as i32);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
