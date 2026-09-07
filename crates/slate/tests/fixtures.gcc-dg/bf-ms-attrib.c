/* bf-ms-attrib.c */
/* Adapted from Donn Terry <donnte@microsoft.com> testcase
   posted to GCC-patches
   http://gcc.gnu.org/ml/gcc-patches/2000-08/msg00577.html */

/* { dg-do run { target *-*-mingw* *-*-cygwin* } } */

/* We don't want the default "pedantic-errors" in this case, since we're
   testing nonstandard stuff to begin with. */
/* { dg-options "-ansi" } */

extern void abort(void);

struct one_gcc {
  int            d;
  unsigned char  a;
  unsigned short b : 7;
  char           c;
} __attribute__((__gcc_struct__));

struct one_ms {
  int            d;
  unsigned char  a;
  unsigned short b : 7;
  char           c;
} __attribute__((__ms_struct__));

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  /* As long as the sizes are as expected, we know attributes are working.
       bf-ms-layout.c makes sure the right thing happens when the attribute
       is on. */
  if (sizeof(struct one_ms) != 12)
    abort();
  if (sizeof(struct one_gcc) != 8)
    abort();
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = std::mem::size_of::<one_ms>() as u64;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = std::mem::size_of::<one_gcc>() as u64;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 8;
// LOWERING-DAG:         let {{__v[0-9]+}}: u64 = 8;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<one_ms>() as u64;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<one_gcc>() as u64;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 8;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = 8 != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
