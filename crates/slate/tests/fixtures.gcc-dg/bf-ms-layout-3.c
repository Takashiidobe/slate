/* Test for MS bitfield layout */
/* { dg-do run { target *-*-mingw* *-*-cygwin* i?86-*-* x86_64-*-* } } */

extern void abort();

struct s1_t {
  char a;
  char b __attribute__((aligned(16)));
} __attribute__((ms_struct));
struct s1_t s1;

struct s2_t {
  char a;
  char b;
} __attribute__((ms_struct));
struct s2_t s2;

struct s3_t {
  __extension__ char a : 6;
  char               b __attribute__((aligned(16)));
} __attribute__((ms_struct));
struct s3_t s3;

struct s4_t {
  __extension__ char a : 6;
  char               b __attribute__((aligned(2)));
} __attribute__((ms_struct));
struct s4_t s4;

struct s5_t {
  __extension__ char a : 6;
  char               b __attribute__((aligned(1)));
} __attribute__((ms_struct));
struct s5_t s5;

__extension__ static __PTRDIFF_TYPE__ offs(const void *a, const void *b) {
  return (__PTRDIFF_TYPE__)((const char *)a - (const char *)b);
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  if (offs(&s1.b, &s1) != 16 || offs(&s2.b, &s2) != 1 ||
      offs(&s3.b, &s3) != 16 || offs(&s4.b, &s4) != 2 || offs(&s5.b, &s5) != 1)
    abort();
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG:             (unsafe { std::ptr::addr_of_mut!((*s1).b) }) as *mut core::ffi::c_void;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG:             (unsafe { std::ptr::addr_of_mut!(*s1) }) as *mut core::ffi::c_void;
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = offs(
// LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:         );
// LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 16;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG:                 (unsafe { std::ptr::addr_of_mut!(s2.b) }) as *mut core::ffi::c_void;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(s2) as *mut core::ffi::c_void;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = offs(
// LOWERING-DAG:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             );
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG:                 (unsafe { std::ptr::addr_of_mut!((*s3).__bitfield_2) }) as *mut core::ffi::c_void;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG:                 (unsafe { std::ptr::addr_of_mut!(*s3) }) as *mut core::ffi::c_void;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = offs(
// LOWERING-DAG:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             );
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 16;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG:                 (unsafe { std::ptr::addr_of_mut!((*s4).__bitfield_2) }) as *mut core::ffi::c_void;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG:                 (unsafe { std::ptr::addr_of_mut!(*s4) }) as *mut core::ffi::c_void;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = offs(
// LOWERING-DAG:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             );
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 2;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG:                 (unsafe { std::ptr::addr_of_mut!(s5.__bitfield_1) }) as *mut core::ffi::c_void;
// LOWERING-DAG:             let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-DAG:                 std::ptr::addr_of_mut!(s5) as *mut core::ffi::c_void;
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = offs(
// LOWERING-DAG:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             );
// LOWERING-DAG:             let {{__v[0-9]+}}: i64 = 1;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:             {{__v[0-9]+}}
// LOWERING-DAG:         };
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
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-DAG:         (unsafe { std::ptr::addr_of_mut!((*s1).b) }) as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-DAG:         (unsafe { std::ptr::addr_of_mut!(*s1) }) as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = offs(
// REWRITES-DAG:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:     );
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 16;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-DAG:             (unsafe { std::ptr::addr_of_mut!(s2.b) }) as *mut core::ffi::c_void;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(s2) as *mut core::ffi::c_void;
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = offs(
// REWRITES-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:         );
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 1;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-DAG:             (unsafe { std::ptr::addr_of_mut!((*s3).__bitfield_2) }) as *mut core::ffi::c_void;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-DAG:             (unsafe { std::ptr::addr_of_mut!(*s3) }) as *mut core::ffi::c_void;
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = offs(
// REWRITES-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:         );
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 16;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-DAG:             (unsafe { std::ptr::addr_of_mut!((*s4).__bitfield_2) }) as *mut core::ffi::c_void;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-DAG:             (unsafe { std::ptr::addr_of_mut!(*s4) }) as *mut core::ffi::c_void;
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = offs(
// REWRITES-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:         );
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 2;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     } else {
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-DAG:             (unsafe { std::ptr::addr_of_mut!(s5.__bitfield_1) }) as *mut core::ffi::c_void;
// REWRITES-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(s5) as *mut core::ffi::c_void;
// REWRITES-DAG:         let {{__v[0-9]+}}: i64 = offs(
// REWRITES-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:         );
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 1;
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     };
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
