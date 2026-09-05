/* { dg-do run } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-minline-all-stringops" { target { i?86-*-* x86_64-*-* } } } */

extern void abort (void);

#define MAX_OFFSET (sizeof (long long))
#define MAX_COPY (8 * sizeof (long long))
#define MAX_EXTRA (sizeof (long long))

#define MAX_LENGTH (MAX_OFFSET + MAX_COPY + MAX_EXTRA)

static union {
  char buf[MAX_LENGTH];
  long long align_int;
  long double align_fp;
} u;

char A[MAX_LENGTH];

// @lowering-fn-begin
// @rewrite-fn-begin
int
main ()
{
  int off, len, i;
  char *p, *q;

  for (i = 0; i < MAX_LENGTH; i++)
    A[i] = 'A';

  for (off = 0; off < MAX_OFFSET; off++)
    for (len = 1; len < MAX_COPY; len++)
      {
	for (i = 0; i < MAX_LENGTH; i++)
	  u.buf[i] = 'a';

	p = __builtin_memcpy (u.buf + off, A, len);
	if (p != u.buf + off)
	  abort ();

	q = u.buf;
	for (i = 0; i < off; i++, q++)
	  if (*q != 'a')
	    abort ();

	for (i = 0; i < len; i++, q++)
	  if (*q != 'A')
	    abort ();

	for (i = 0; i < MAX_EXTRA; i++, q++)
	  if (*q != 'a')
	    abort ();
      }

  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut off: i32 = 0;
// LOWERING-DAG:     let mut len: i32 = 0;
// LOWERING-DAG:     let mut i: i32 = 0;
// LOWERING-DAG:     let mut p: *mut i8 = std::ptr::null_mut();
// LOWERING-DAG:     let mut q: *mut i8 = std::ptr::null_mut();
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         i = {{_v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:             if !{{_v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{_v[0-9]+}}: i8 = 65;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-DAG:             unsafe {
// LOWERING-DAG:                 (*A)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:             i = {{_v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         off = {{_v[0-9]+}};
// LOWERING-DAG:         loop {
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = off;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-DAG:             let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:             if !{{_v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:             {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 len = {{_v[0-9]+}};
// LOWERING-DAG:                 loop {
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = len;
// LOWERING-DAG:                     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-DAG:                     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:                     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:                     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-DAG:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                     if !{{_v[0-9]+}} {
// LOWERING-DAG:                         break;
// LOWERING-DAG:                     }
// LOWERING-DAG:                     {
// LOWERING-DAG:                         {
// LOWERING-DAG:                             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                             i = {{_v[0-9]+}};
// LOWERING-DAG:                             loop {
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-DAG:                                 let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                                 let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                                 if !{{_v[0-9]+}} {
// LOWERING-DAG:                                     break;
// LOWERING-DAG:                                 }
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i8 = 97;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-DAG:                                 unsafe {
// LOWERING-DAG:                                     u.buf[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-DAG:                                 }
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:                                 i = {{_v[0-9]+}};
// LOWERING-DAG:                             }
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(u.buf) }) as *mut i8;
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = off;
// LOWERING-DAG:                         let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-DAG:                         let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:                         let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(A).cast::<i8>();
// LOWERING-DAG:                         let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:                         let {{_v[0-9]+}}: i32 = len;
// LOWERING-DAG:                         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-DAG:                         let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-DAG:                             memcpy(
// LOWERING-DAG:                                 {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:                                 {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:                                 {{_v[0-9]+}} as u64,
// LOWERING-DAG:                             )
// LOWERING-DAG:                         };
// LOWERING-DAG:                         let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-DAG:                         p = {{_v[0-9]+}};
// LOWERING-DAG:                         {
// LOWERING-DAG:                             let {{_v[0-9]+}}: *mut i8 = p;
// LOWERING-DAG:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-DAG:                                 (unsafe { std::ptr::addr_of_mut!(u.buf) }) as *mut i8;
// LOWERING-DAG:                             let {{_v[0-9]+}}: i32 = off;
// LOWERING-DAG:                             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-DAG:                             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                             if {{_v[0-9]+}} {
// LOWERING-DAG:                                 unsafe { abort() };
// LOWERING-DAG:                             }
// LOWERING-DAG:                         }
// LOWERING-DAG:                         let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(u.buf) }) as *mut i8;
// LOWERING-DAG:                         q = {{_v[0-9]+}};
// LOWERING-DAG:                         {
// LOWERING-DAG:                             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                             i = {{_v[0-9]+}};
// LOWERING-DAG:                             loop {
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = off;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                                 if !{{_v[0-9]+}} {
// LOWERING-DAG:                                     break;
// LOWERING-DAG:                                 }
// LOWERING-DAG:                                 {
// LOWERING-DAG:                                     let {{_v[0-9]+}}: *mut i8 = q;
// LOWERING-DAG:                                     let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG:                                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:                                     let {{_v[0-9]+}}: i32 = 97;
// LOWERING-DAG:                                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                                     if {{_v[0-9]+}} {
// LOWERING-DAG:                                         unsafe { abort() };
// LOWERING-DAG:                                     }
// LOWERING-DAG:                                 }
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:                                 i = {{_v[0-9]+}};
// LOWERING-DAG:                                 let {{_v[0-9]+}}: *mut i8 = q;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-DAG:                                 q = {{_v[0-9]+}};
// LOWERING-DAG:                             }
// LOWERING-DAG:                         }
// LOWERING-DAG:                         {
// LOWERING-DAG:                             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                             i = {{_v[0-9]+}};
// LOWERING-DAG:                             loop {
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = len;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                                 if !{{_v[0-9]+}} {
// LOWERING-DAG:                                     break;
// LOWERING-DAG:                                 }
// LOWERING-DAG:                                 {
// LOWERING-DAG:                                     let {{_v[0-9]+}}: *mut i8 = q;
// LOWERING-DAG:                                     let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG:                                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:                                     let {{_v[0-9]+}}: i32 = 65;
// LOWERING-DAG:                                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                                     if {{_v[0-9]+}} {
// LOWERING-DAG:                                         unsafe { abort() };
// LOWERING-DAG:                                     }
// LOWERING-DAG:                                 }
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:                                 i = {{_v[0-9]+}};
// LOWERING-DAG:                                 let {{_v[0-9]+}}: *mut i8 = q;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-DAG:                                 q = {{_v[0-9]+}};
// LOWERING-DAG:                             }
// LOWERING-DAG:                         }
// LOWERING-DAG:                         {
// LOWERING-DAG:                             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                             i = {{_v[0-9]+}};
// LOWERING-DAG:                             loop {
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: u64 = 8;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                                 if !{{_v[0-9]+}} {
// LOWERING-DAG:                                     break;
// LOWERING-DAG:                                 }
// LOWERING-DAG:                                 {
// LOWERING-DAG:                                     let {{_v[0-9]+}}: *mut i8 = q;
// LOWERING-DAG:                                     let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG:                                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG:                                     let {{_v[0-9]+}}: i32 = 97;
// LOWERING-DAG:                                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-DAG:                                     if {{_v[0-9]+}} {
// LOWERING-DAG:                                         unsafe { abort() };
// LOWERING-DAG:                                     }
// LOWERING-DAG:                                 }
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:                                 i = {{_v[0-9]+}};
// LOWERING-DAG:                                 let {{_v[0-9]+}}: *mut i8 = q;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                                 let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-DAG:                                 q = {{_v[0-9]+}};
// LOWERING-DAG:                             }
// LOWERING-DAG:                         }
// LOWERING-DAG:                     }
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = len;
// LOWERING-DAG:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:                     len = {{_v[0-9]+}};
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = off;
// LOWERING-DAG:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-DAG:             off = {{_v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut off: i32 = 0;
// REWRITES-DAG:     let mut len: i32 = 0;
// REWRITES-DAG:     let mut i: i32 = 0;
// REWRITES-DAG:     let mut p: *mut i8 = std::ptr::null_mut();
// REWRITES-DAG:     let mut q: *mut i8 = std::ptr::null_mut();
// REWRITES-DAG:     loop {
// REWRITES-DAG:         let {{_v[0-9]+}}: u64 = 8;
// REWRITES-DAG:         if !((i as u64) < 8 + 8 * {{_v[0-9]+}} + 8) {
// REWRITES-DAG:             break;
// REWRITES-DAG:         }
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             (*A)[((i as i64) as usize)] = 65;
// REWRITES-DAG:         }
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     off = 0;
// REWRITES-DAG:     while (off as u64) < 8 {
// REWRITES-DAG:         len = 1;
// REWRITES-DAG:         loop {
// REWRITES-DAG:             let {{_v[0-9]+}}: u64 = 8;
// REWRITES-DAG:             if !((len as u64) < 8 * {{_v[0-9]+}}) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:             i = 0;
// REWRITES-DAG:             loop {
// REWRITES-DAG:                 let {{_v[0-9]+}}: u64 = 8;
// REWRITES-DAG:                 if !((i as u64) < 8 + 8 * {{_v[0-9]+}} + 8) {
// REWRITES-DAG:                     break;
// REWRITES-DAG:                 }
// REWRITES-DAG:                 unsafe {
// REWRITES-DAG:                     u.buf[((i as i64) as usize)] = 97;
// REWRITES-DAG:                 }
// REWRITES-DAG:                 i += 1;
// REWRITES-DAG:             }
// REWRITES-DAG:             let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(u.buf) }) as *mut i8;
// REWRITES-DAG:             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset(off as isize) };
// REWRITES-DAG:             let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:             let {{_v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(A).cast::<i8>();
// REWRITES-DAG:             let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// REWRITES-DAG:                 {
// REWRITES-DAG:                     std::ptr::copy_nonoverlapping(
// REWRITES-DAG:                         {{_v[0-9]+}} as *const u8,
// REWRITES-DAG:                         {{_v[0-9]+}} as *mut u8,
// REWRITES-DAG:                         (len as u64) as usize,
// REWRITES-DAG:                     );
// REWRITES-DAG:                     {{_v[0-9]+}} as *mut core::ffi::c_void
// REWRITES-DAG:                 }
// REWRITES-DAG:             };
// REWRITES-DAG:             p = {{_v[0-9]+}} as *mut i8;
// REWRITES-DAG:             let {{_v[0-9]+}}: *mut i8 = p;
// REWRITES-DAG:             let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(u.buf) }) as *mut i8;
// REWRITES-DAG:             let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.offset(off as isize) };
// REWRITES-DAG:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// REWRITES-DAG:             if {{_v[0-9]+}} {
// REWRITES-DAG:                 unsafe { abort() };
// REWRITES-DAG:             }
// REWRITES-DAG:             let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!(u.buf) }) as *mut i8;
// REWRITES-DAG:             q = {{_v[0-9]+}};
// REWRITES-DAG:             i = 0;
// REWRITES-DAG:             while i < off {
// REWRITES-DAG:                 let {{_v[0-9]+}}: bool = ((unsafe { *q }) as i32) != 97;
// REWRITES-DAG:                 if {{_v[0-9]+}} {
// REWRITES-DAG:                     unsafe { abort() };
// REWRITES-DAG:                 }
// REWRITES-DAG:                 i += 1;
// REWRITES-DAG:                 let {{_v[0-9]+}}: *mut i8 = q;
// REWRITES-DAG:                 q = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-DAG:             }
// REWRITES-DAG:             i = 0;
// REWRITES-DAG:             while i < len {
// REWRITES-DAG:                 let {{_v[0-9]+}}: bool = ((unsafe { *q }) as i32) != 65;
// REWRITES-DAG:                 if {{_v[0-9]+}} {
// REWRITES-DAG:                     unsafe { abort() };
// REWRITES-DAG:                 }
// REWRITES-DAG:                 i += 1;
// REWRITES-DAG:                 let {{_v[0-9]+}}: *mut i8 = q;
// REWRITES-DAG:                 q = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-DAG:             }
// REWRITES-DAG:             i = 0;
// REWRITES-DAG:             while (i as u64) < 8 {
// REWRITES-DAG:                 let {{_v[0-9]+}}: bool = ((unsafe { *q }) as i32) != 97;
// REWRITES-DAG:                 if {{_v[0-9]+}} {
// REWRITES-DAG:                     unsafe { abort() };
// REWRITES-DAG:                 }
// REWRITES-DAG:                 i += 1;
// REWRITES-DAG:                 let {{_v[0-9]+}}: *mut i8 = q;
// REWRITES-DAG:                 q = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-DAG:             }
// REWRITES-DAG:             len += 1;
// REWRITES-DAG:         }
// REWRITES-DAG:         off += 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
