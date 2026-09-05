/* Test atomic_is_lock_free for char8_t.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#include <stdatomic.h>
#include <stdint.h>

extern void abort(void);

_Atomic __CHAR8_TYPE__ ac8a;
atomic_char8_t         ac8t;

#define CHECK_TYPE(MACRO, V1, V2)                                              \
  do {                                                                         \
    int r1 = MACRO;                                                            \
    int r2 = atomic_is_lock_free(&V1);                                         \
    int r3 = atomic_is_lock_free(&V2);                                         \
    if (r1 != 0 && r1 != 1 && r1 != 2)                                         \
      abort();                                                                 \
    if (r2 != 0 && r2 != 1)                                                    \
      abort();                                                                 \
    if (r3 != 0 && r3 != 1)                                                    \
      abort();                                                                 \
    if (r1 == 2 && r2 != 1)                                                    \
      abort();                                                                 \
    if (r1 == 2 && r3 != 1)                                                    \
      abort();                                                                 \
    if (r1 == 0 && r2 != 0)                                                    \
      abort();                                                                 \
    if (r1 == 0 && r3 != 0)                                                    \
      abort();                                                                 \
  } while (0)

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  CHECK_TYPE(ATOMIC_CHAR8_T_LOCK_FREE, ac8a, ac8t);

  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         loop {
// LOWERING-DAG:             {
// LOWERING-DAG:                 let mut r1: i32 = 0;
// LOWERING-DAG:                 let mut r2: i32 = 0;
// LOWERING-DAG:                 let mut r3: i32 = 0;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                 r1 = {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:                 r2 = {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = true;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:                 r3 = {{__v[0-9]+}};
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = r1;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = r1;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = r1;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = r2;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = r2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = r3;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = r3;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = r1;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = r2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = r1;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = r3;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = r1;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = r2;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:                 {
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = r1;
// LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:                     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = r3;
// LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     } else {
// LOWERING-DAG:                         let {{__v[0-9]+}}: bool = false;
// LOWERING-DAG:                         {{__v[0-9]+}}
// LOWERING-DAG:                     };
// LOWERING-DAG:                     if {{__v[0-9]+}} {
// LOWERING-DAG:                         unsafe { abort() };
// LOWERING-DAG:                     }
// LOWERING-DAG:                 }
// LOWERING-DAG:             }
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:             if !{{__v[0-9]+}} {
// LOWERING-DAG:                 break;
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     loop {
// REWRITES-DAG:         let mut r1: i32 = 0;
// REWRITES-DAG:         let mut r2: i32 = 0;
// REWRITES-DAG:         let mut r3: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         r1 = {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:         r2 = {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = true;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:         r3 = {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = r1;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = r1;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = r1;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = r2;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = r2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = r3;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = r3;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = r1;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = r2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = r1;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = r3;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = r1;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = r2;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = r1;
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = r3;
// REWRITES-DAG:             let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         } else {
// REWRITES-DAG:             let {{__v[0-9]+}}: bool = false;
// REWRITES-DAG:             {{__v[0-9]+}}
// REWRITES-DAG:         };
// REWRITES-DAG:         if {{__v[0-9]+}} {
// REWRITES-DAG:             unsafe { abort() };
// REWRITES-DAG:         }
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:         if !{{__v[0-9]+}} {
// REWRITES-DAG:             break;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
